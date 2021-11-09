#pragma once

#include "Game.h"
#include "Visualizer.h"
#include <memory>

namespace RBW {
	const int COUNT_OF_COMMANDS = 1;
	const int COUNT_OF_DIRECTIONS = 4;

	class Model {
	public:
		Model();

		void init();
		void tick();
		void move(Point from, Point to);
		void mix();
	private:
		bool getInputAndTryProcess();
		bool tryProcessCommand(Command command);
		void processCommand(Command command);
		void run();

		const std::array<char, COUNT_OF_COMMANDS> _commands = { 'm' };
		const std::array<char, COUNT_OF_DIRECTIONS> _directions = { 'l', 'r', 'u', 'd' };

		std::string _command;

		std::unique_ptr<Game> _game;
		std::unique_ptr<Visualizer> _visualizer;
	};
}