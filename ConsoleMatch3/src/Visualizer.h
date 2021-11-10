#pragma once
#include <array>
#include <iostream>

#if defined _WIN32
#define CLEAR_SCREEN system("cls");
#elif defined (__LINUX__) || defined (__gnu_linux__) || defined(__linux)
#define CLEAR_SCREEN system("clear");
#elif defined (__APPLE__)
#define CLEAR_SCREEN system("clear");
#endif

namespace RBW {
	class Visualizer {
	public:
		Visualizer();

		template<typename T, size_t size>
		void Draw(const std::array<T, size>& map) const {
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
				std::copy(row.begin(), row.end(), std::ostream_iterator<std::string>{std::cout, " "});
				std::cout << std::endl;
			}
		}
	private:

	};
}
