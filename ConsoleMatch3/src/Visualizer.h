#pragma once

#include <vector>

namespace RBW {
    class Visualizer {
    public:
        Visualizer();
        
        void Draw(std::vector<std::vector<char>> map) const;
    };
}
