#include "Game.h"

namespace RBW {

	Game::Game() : 
		_isClosed{ false } {
		_distro = std::uniform_int_distribution<int>{0, COUNT_OF_CRYSTALLS-1};
	}

	void Game::init() {

	}

	void Game::move(Point from, Point to) {
		std::swap(_map[from.y][from.x], _map[to.y][to.x]);
	}

	void Game::fillMapRandomly() {
		for (auto& row : _map) {
			for (auto& elem : row) {
				elem = _crystalls[_distro(_random_engine)];
			}
		}
	}

	const Map& Game::getMap() const noexcept {
		return _map;
	}
}