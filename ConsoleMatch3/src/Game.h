#pragma once

#include <array>
#include <random>

namespace RBW {
	const int COUNT_OF_CRYSTALLS = 6;
	const int WIDTH = 10;
	const int HEIGHT = 10;

	using Map = std::array<std::array<std::string, WIDTH>, HEIGHT>;


	struct Point {
		int x;
		int y;
	};

	class Game {
	public:
		Game();

		void init();
		void run();

		const Map& getMap() const noexcept;

		void fillMapRandomly();
	private:
		std::default_random_engine _random_engine;
		std::uniform_int_distribution<int> _distro;

		std::array<std::string, COUNT_OF_CRYSTALLS> _crystalls;
		Map _map;
	};
}