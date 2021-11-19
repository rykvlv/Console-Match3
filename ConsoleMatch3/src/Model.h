#pragma once

#include <memory>
#include <optional>
#include <random>
#include <vector>
#include <queue>

namespace RBW {
    const unsigned WIDTH = 10;
    const unsigned HEIGHT = 10;
    const unsigned COUNT_OF_CRYSTALLS = 6;
    
    struct Point {
        int x;
        int y;
    };
    
    struct Crystall {
        char type;
        char bonus;
        
        void OnDestroy() {
            Destroy(bonus);
        }
        
        void Destroy(char bonus) {
            switch (bonus) {
            case '0':
                type = ' ';
                break;
            }
        }
    };
    
    class Model {
    public:
        Model();
        
        void Init();
        std::queue<std::vector<std::vector<Crystall>>> Tick();
        void Dump();
        void Mix();
        void Move(Point from, Point to);
    private:
        void performTick();
        void fillMapRandomly();
        
        void updateField();
        void removeMatches(std::vector<std::vector<Point>> matches);
        void deleteCrystall(Point point);
        void affectAbove(Point point);
        void fillEmptyCells();
        
        std::vector<std::vector<Point>> lookForMatches() const;
        bool lookForPossibleMoves() const;
        bool matchPattern(Point point, std::vector<std::vector<int>> mustHave, std::vector<std::vector<int>> needOne) const;
        bool matchType(Point point, char type) const;
        std::vector<Point> getMatchHorizontal(Point point) const;
        std::vector<Point> getMatchVertical(Point point) const;

        std::vector<std::vector<Crystall>> _map;
        
        std::queue<std::vector<std::vector<Crystall>> > _ticks;
        
        std::default_random_engine _random_engine;
        std::uniform_int_distribution<int> _distro;
        
        std::random_device _rd;
        std::mt19937 _g;
    };
}
