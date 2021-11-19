#pragma once

#include "Model.h"
#include "Visualizer.h"

#include <memory>
#include <optional>

namespace RBW {
    struct Command {
        char type;
        int x;
        int y;
        char direction;
    };
    
    class Controller {
    public:
        Controller();
        
        void Run();
    private:
        std::unique_ptr<Model> _model;
        std::unique_ptr<Visualizer> _visualizer;
        
        std::vector<std::vector<char>> getTransformedMap(const std::vector<std::vector<Crystall>>& map);
        
        std::optional<Command> getInputCommand() const;
        bool checkCommand(Command command) const;
        void processCommand(Command command);
        
        std::vector<char> _commands;
        std::vector<char> _directions;
        bool _isClosed;
    };
}
