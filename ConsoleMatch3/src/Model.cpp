#pragma once

#include "Model.h"

namespace RBW {
	Model::Model() :
		_game(std::make_unique<Game>()), 
		_visualizer(std::make_unique<Visualizer>()) {

	}

	void Model::init() {
		_game->init();
		_game->fillMapRandomly();
		tick();
	}

	void Model::tick() {
		_visualizer->draw(_game->getMap());
	}

	void Model::move(Point from, Point to) {

	}

	void Model::mix() {

	}
}