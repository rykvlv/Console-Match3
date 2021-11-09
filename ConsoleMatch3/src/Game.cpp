#include "Game.h"

namespace RBW {
	Game::Game() {
		_distro = std::uniform_int_distribution<int>{0, COUNT_OF_CRYSTALLS-1};
		_crystalls = { "A", "B", "C", "D", "E", "F" };
	}

	void Game::init() {

	}

	void Game::run() {

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