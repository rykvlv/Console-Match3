#pragma once

#include "Visualizer.h"
#include <memory>
#include <vector>
#include <random>

namespace RBW {
	const int COUNT_OF_CRYSTALLS = 6;
	const int WIDTH = 4;
	const int HEIGHT = 4;
	const int COUNT_OF_COMMANDS = 1;
	const int COUNT_OF_DIRECTIONS = 4;

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

	class Model {
	public:
		Model();

		void Init();
		void Tick();
		void Move(Point from, Point to);
		void Mix();
		void Dump();
		void Run();
	private:
		const std::array<std::string, COUNT_OF_CRYSTALLS> _crystalls = { "A", "B", "C", "D", "E", "F" };

		Map _map;

		bool _isClosed;

		void fillMapRandomly();

		bool getInputAndTryProcess();
		bool tryProcessCommand(Command command);
		void processCommand(Command command);
		void run();

		void findAndRemoveMatches();
		std::vector<std::vector<Point>> lookForMatches();
		std::vector<Point> getMatchHorizontal(int x, int y);
		std::vector<Point> getMatchVertical(int x, int y);
		void affectAbove(Point point);
		void addNewPieces();

		const std::array<char, COUNT_OF_COMMANDS> _commands = { 'm' };
		const std::array<char, COUNT_OF_DIRECTIONS> _directions = { 'l', 'r', 'u', 'd' };

		std::string _command;
		std::unique_ptr<Visualizer> _visualizer;

		std::default_random_engine _random_engine;
		std::uniform_int_distribution<int> _distro;
	};
}