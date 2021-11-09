#pragma once
#include <array>
#include <iostream>

namespace RBW {
	class Visualizer {
	public:
		Visualizer();

		template<typename T, size_t size>
		void draw(const std::array<T, size>& map) const {
			std::cout << "    ";
			for (int i = 0; i < map.size(); ++i) {
				std::cout << i << " ";
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
