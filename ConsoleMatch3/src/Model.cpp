#include "Model.h"

namespace RBW {
    Model::Model() :
                    _g{_rd()} {
        _distro = {std::uniform_int_distribution<int>{0, COUNT_OF_CRYSTALLS - 1}};
        _map.resize(HEIGHT);
        for (auto& row : _map) {
            row.resize(WIDTH);
        }
    }
    
    void Model::Init() {
        fillMapRandomly();
        performTick();
    }
    
    void Model::Move(Point from, Point to) {
        std::swap(_map[from.y][from.x], _map[to.y][to.x]);
        performTick();
        if (lookForMatches().size() == 0) {
            std::swap(_map[from.y][from.x], _map[to.y][to.x]);
            performTick();
            return;
        }
        updateField();
    }
    
    void Model::Dump() {
        while (!_ticks.empty()) {
            _ticks.pop();
        }
    }
    
    std::queue<std::vector<std::vector<Crystall>>> Model::Tick() {
        return _ticks;
    }
    
    void Model::Mix() {
        std::shuffle(_map.begin(), _map.end(), _g);
        
        for (auto& row : _map) {
            std::shuffle(row.begin(), row.end(), _g);
        }
        
        performTick();
        
        if (!lookForPossibleMoves() || lookForMatches().size() != 0) {
            Mix();
        }
    }
    
    void Model::performTick() {
        _ticks.push(_map);
    }
    
    void Model::fillMapRandomly() {
        for (int row = 0; row < HEIGHT; ++row) {
            for (int column = 0; column < WIDTH; ++column) {
                char type = 65 + _distro(_random_engine);
                Crystall crystall = {type, '0'};
                _map[row][column] = crystall;
            }
        }
        
        if (lookForMatches().size() != 0 || !lookForPossibleMoves()) {
            fillMapRandomly();
        }
    }
    
    void Model::updateField() {
        auto matches = lookForMatches();
        if (matches.size() == 0)
            return;
        
        removeMatches(matches);
        
        for (int i = 0; i < matches.size(); ++i) {
            for (int j = 0; j < matches[i].size(); ++j) {
                affectAbove(matches[i][j]);
            }
        }
        
        fillEmptyCells();
        
        if (lookForMatches().size() != 0) {
            updateField();
        }
        
        if (!lookForPossibleMoves()) {
            Mix();
        }
    }
    
    void Model::removeMatches(std::vector<std::vector<Point>> matches) {
        for (int i = 0; i < matches.size(); ++i) {
            for (int j = 0; j < matches[i].size(); ++j) {
                auto x = matches[i][j].x;
                auto y = matches[i][j].y;
                deleteCrystall({x, y});
                performTick();
            }
        }
    }
    
    void Model::deleteCrystall(Point point) {
        _map[point.y][point.x].OnDestroy();
        performTick();
    }
    
    void Model::affectAbove(Point point) {
        for (int row = point.y; row > 0; --row) {
            if (_map[row][point.x].type == ' ') {
                _map[row][point.x] = _map[row - 1][point.x];
                _map[row - 1][point.x].type = ' ';
                performTick();
            }
        }
    }
    
    void Model::fillEmptyCells() {
        for (int row = 0; row < HEIGHT; ++row) {
            for (int column = 0; column < WIDTH; ++column) {
                if (_map[row][column].type == ' ') {
                    char type = 65 + _distro(_random_engine);
                    Crystall crystall = {type, '0'};
                    _map[row][column] = crystall;
                    performTick();
                }
            }
        }
    }
    
    std::vector<std::vector<Point>> Model::lookForMatches() const {
        std::vector<std::vector<Point>> matchList;
        for (int row = 0; row < _map.size(); ++row) {
            for (int column = 0; column < _map[row].size() - 2; ++column) {
                auto match = getMatchHorizontal({column, row});
                if (match.size() > 2) {
                    matchList.push_back(match);
                    column += match.size() - 1;
                }
            }
        }
        
        for (int row = 0; row < _map.size(); ++row) {
            for (int column = 0; column < _map[row].size(); ++column) {
                auto match = getMatchVertical({column, row});
                if (match.size() > 2) {
                    matchList.push_back(match);
                    row += match.size() - 1;
                }
            }
        }
        
        return matchList;
    }

    bool Model::lookForPossibleMoves() const {
        for (int row = 0; row < _map.size(); ++row) {
            for (int column = 0; column < _map[row].size(); ++column) {
                if (matchPattern({column, row},
                                 { {0, 1} },
                                 { {-2, 0}, {-1, -1}, {-1, 1}, {2, -1}, {2, 1}, {3, 0} })) {
                    return true;
                }
                
                if (matchPattern({column, row},
                                 { {2, 0} },
                                 { {1, -1}, {1, 1} })) {
                    return true;
                }
                
                if (matchPattern({column, row},
                                 { {0, 1} },
                                 { {0, -2}, {-1, -1}, {1, -1}, {-1, 2}, {1, 2}, {0, 3} })) {
                    return true;
                }
                
                if (matchPattern({column, row},
                                 { {0, 2} },
                                 { {-1, 1}, {1, 1} })) {
                    return true;
                }
            }
        }
        
        return false;
    }

    bool Model::matchPattern(Point point, std::vector<std::vector<int>> mustHave, std::vector<std::vector<int>> needOne) const {
        const auto &[x, y] = point;
        auto type = _map[y][x].type;
        
        for (int i = 0; i < mustHave.size(); ++i) {
            if (!matchType({x + mustHave[i][0], y + mustHave[i][1]}, type)) {
                return false;
            }
        }
        
        for (int i = 0; i < needOne.size(); ++i) {
            if (matchType({x + needOne[i][0], y + needOne[i][1]}, type)) {
                return true;
            }
        }
        
        return false;
    }

    bool Model::matchType(Point point, char type) const {
        auto [x, y] = point;
        if ((x < 0) || (x >= _map[0].size()) || (y < 0) || (y >= _map.size())) {
            return false;
        }
        return _map[y][x].type == type;
    }

    std::vector<Point> Model::getMatchHorizontal(Point point) const  {
        std::vector<Point> match;
        match.emplace_back(point);
        auto [x, y] = point;
        for (int i = 1; x + i < _map[y].size(); ++i) {
            if (_map[y][x].type == _map[y][x + i].type) {
                Point matchPoint {x + i, y};
                match.emplace_back(matchPoint);
            }
            else {
                return match;
            }
        }
        return match;
    }

    std::vector<Point> Model::getMatchVertical(Point point) const {
        std::vector<Point> match;
        match.emplace_back(point);
        auto [x, y] = point;
        for (int i = 1; point.y + i < _map.size(); ++i) {
            if (_map[y][x].type == _map[y + i][x].type) {
                Point matchPoint { x, y + i};
                match.emplace_back(matchPoint);
            }
            else {
                return match;
            }
        }
        return match;
    }

}
