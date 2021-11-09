#pragma once

#include "Model.h"
#include <iostream>

namespace RBW {
	Model::Model() :
		_game(std::make_unique<Game>()), 
		_visualizer(std::make_unique<Visualizer>()) {
	}

	void Model::init() {
		_game->init();
		_game->fillMapRandomly();
		tick();
		run();
	}

	void Model::run() {
		while (!_game->isClosed()) {
			if (getInputAndTryProcess()) {
				tick();
			}
		}
	}

	void Model::tick() {
		_visualizer->draw(_game->getMap());
	}

	void Model::move(Point from, Point to) {
		_game->move(from, to);
	}

	void Model::mix() {

	}

	bool Model::getInputAndTryProcess() {
		Command command;
		if (std::cin >>
			command.type >>
			command.x >>
			command.y >>
			command.direction) {
			if (!tryProcessCommand(command)) {
				std::cout << "ERROR: Wrong input!" << std::endl;
				std::cin.clear();
				return false;
			}
		}
		else {
			std::cout << "ERROR: Wrong input!" << std::endl;
			std::cin.clear();
			return false;
		}
		return true;
	}

	bool Model::tryProcessCommand(Command command) {
		auto it = std::find(_commands.begin(), _commands.end(), command.type);
		if (it == _commands.end())
			return false;

		if (command.x >= WIDTH || command.y >= HEIGHT ||
			command.x < 0 || command.y < 0)
			return false;

		processCommand(command);
		return true;
	}

	void Model::processCommand(Command command) {
		Point from{ command.x, command.y };
		Point to;
		switch (command.direction) {
		case 'l':
			to.x = from.x - 1;
			to.y = from.y;
			if (to.x < 0)
				return;
			break;
		case 'r':
			to.x = from.x + 1;
			to.y = from.y;
			if (to.x >= WIDTH)
				return;
			break;
		case 'u':
			to.x = from.x;
			to.y = from.y - 1;
			if (to.y < 0)
				return;
			break;
		case 'd':
			to.x = from.x;
			to.y = from.y + 1;
			if (to.y >= HEIGHT)
				return;
			break;
		}
		move(from, to);
	}
}