#include "Visualizer.h"

#include <iostream>

#if defined _WIN32
#define CLEAR_SCREEN system("cls");
#elif defined (__LINUX__) || defined (__gnu_linux__) || defined(__linux)
#define CLEAR_SCREEN system("clear");
#elif defined (__APPLE__)
#define CLEAR_SCREEN system("clear");
#endif

namespace RBW {
    Visualizer::Visualizer() {

    }
    
    void Visualizer::Draw(std::vector<std::vector<char>> map) const {
        CLEAR_SCREEN
        std::cout << "    ";
        for (int i = 0; i < map.size(); ++i) {
            std::cout << i << " ";
        }
        std::cout << std::endl;

        std::cout << "   ";
        for (int i = 0; i < map.size(); ++i) {
            std::cout << "--";
        }
        std::cout << std::endl;

        for (int i = 0; i < map.size(); ++i) {
            auto& row = map[i];
            std::cout << i << " | ";
            std::copy(row.begin(), row.end(), std::ostream_iterator<char>{std::cout, " "});
            std::cout << std::endl;
        }
    }
}
