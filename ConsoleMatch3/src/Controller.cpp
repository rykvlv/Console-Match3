#include "Controller.h"

#include <iostream>
#include <thread>

using namespace std::chrono_literals;

namespace RBW {
    Controller::Controller() {
        _model = std::make_unique<Model>();
        _visualizer = std::make_unique<Visualizer>();
        _isClosed = false;
        
        _commands = {'m', 'q', 's'};
        _directions = {'l', 'r', 'u', 'd'};
    }
    
    void Controller::Run() {
        _model->Init();
        auto ticks = _model->Tick();
        _visualizer->Draw(getTransformedMap(ticks.front()));
        _model->Dump();
        std::optional<Command> command;
        while (!_isClosed) {
            command = getInputCommand();
            if (command.has_value()) {
                if (checkCommand(*command)) {
                    processCommand(*command);
                    auto ticks = _model->Tick();
                    while (!ticks.empty()) {
                        _visualizer->Draw(getTransformedMap(ticks.front()));
                        ticks.pop();
                        std::this_thread::sleep_for(50ms);
                    }
                    _model->Dump();
                } else {
                    std::cin.clear();
                    std::cout << "Wrong command! Try again." << std::endl;
                }
            } else {
                std::cin.clear();
                std::cout << "Wrong command! Try again." << std::endl;
            }
        }
    }
    
    std::vector<std::vector<char>> Controller::getTransformedMap(const std::vector<std::vector<Crystall>>& map) {
        std::vector<std::vector<char>> transformedMap;
        transformedMap.resize(HEIGHT);
        for (auto& row : transformedMap) {
            row.resize(WIDTH);
        }
        
        for (int row = 0; row < HEIGHT; ++row) {
            for (int column = 0; column < WIDTH; ++column) {
                transformedMap[row][column] = map[row][column].type;
            }
        }
        
        return transformedMap;
    }
    
    std::optional<Command> Controller::getInputCommand() const {
        Command command;
        
        if (std::cin >> command.type) {
            if (command.type == 'q' || command.type == 's') {
                command.x = 0;
                command.y = 0;
                command.direction = 'u';
                return command;
            }
        } else {
            return {};
        }
        
        if (std::cin >> command.x
                     >> command.y
                     >> command.direction) {
            return command;
        }
        
        return {};
    }
    
    bool Controller::checkCommand(Command command) const {
        
        auto commandIt = std::find(_commands.begin(), _commands.end(), command.type);
        auto directionIt = std::find(_directions.begin(), _directions.end(), command.direction);
        
        if (commandIt == _commands.end())
            return false;
        
        if (directionIt == _directions.end())
            return false;

        if (command.x >= WIDTH || command.y >= HEIGHT ||
            command.x < 0 || command.y < 0)
            return false;
        
        return true;
    }
    
    void Controller::processCommand(Command command) {
        Point from{ command.x, command.y };
        Point to{ 0, 0 };
        if (command.type == 'q') {
            _isClosed = true;
        }
        if (command.type == 's') {
            _model->Mix();
        }
        if (command.type == 'm') {
            switch (command.direction) {
            case 'l':
                to.x = from.x - 1;
                to.y = from.y;
                if (to.x < 0)
                    return;
                break;
            case 'r':
                to.x = from.x + 1;
                to.y = from.y;
                if (to.x >= WIDTH)
                    return;
                break;
            case 'u':
                to.x = from.x;
                to.y = from.y - 1;
                if (to.y < 0)
                    return;
                break;
            case 'd':
                to.x = from.x;
                to.y = from.y + 1;
                if (to.y >= HEIGHT)
                    return;
                break;
            }
            _model->Move(from, to);
        }
    }
}
