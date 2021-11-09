#pragma once

#include "Game.h"
#include "Visualizer.h"
#include <memory>

namespace RBW {
	class Model {
	public:
		Model();

		void init();
		void tick();
		void move(Point from, Point to);
		void mix();
	private:
		std::unique_ptr<Game> _game;
		std::unique_ptr<Visualizer> _visualizer;
	};
}