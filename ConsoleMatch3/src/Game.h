#pragma once

#include <array>
#include <random>
#include <functional>

namespace RBW {
	const int COUNT_OF_CRYSTALLS = 6;
	const int WIDTH = 10;
	const int HEIGHT = 10;

	using Map = std::array<std::array<std::string, WIDTH>, HEIGHT>;

	struct Point {
		int x;
		int y;
	};

	struct Command {
		char type;
		int x;
		int y;
		char direction;
	};

	class Game {
	public:
		Game();

		void init();
		
		void fillMapRandomly();
		const Map& getMap() const noexcept;

		void move(Point from, Point to);

		inline bool isClosed() { return _isClosed; }
	private:
		bool _isClosed;

		const std::array<std::string, COUNT_OF_CRYSTALLS> _crystalls = { "A", "B", "C", "D", "E", "F" };
		

		Map _map;

		std::default_random_engine _random_engine;
		std::uniform_int_distribution<int> _distro;
	};
}