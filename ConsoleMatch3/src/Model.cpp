#pragma once

#include "Model.h"
#include <iostream>
#include <thread>

using namespace std::chrono_literals;

namespace RBW {
	Model::Model() :
		_visualizer(std::make_unique<Visualizer>()),
		_distro { std::uniform_int_distribution<int>{ 0, COUNT_OF_CRYSTALLS - 1 }} {

	}

	void Model::Init() {
		_isClosed = false;
		fillMapRandomly();
		Dump();
		Run();
	}

	void Model::Run() {
		while (!_isClosed) {
			if (getInputAndTryProcess()) {
				findAndRemoveMatches();
				Tick();
			}
		}
	}

	void Model::Tick() {
		Dump();
	}

	void Model::Dump() {
		_visualizer->draw(_map);
	}

	void Model::Move(Point from, Point to) {
		std::swap(_map[from.y][from.x], _map[to.y][to.x]);
	}

	void Model::Mix() {

	}

	void Model::fillMapRandomly() {
		for (auto& row : _map) {
			for (auto& elem : row) {
				elem = _crystalls[_distro(_random_engine)];
			}
		}
	}

	void Model::findAndRemoveMatches() {
		auto matches = lookForMatches();
		for (int i = 0; i < matches.size(); ++i) {
			for (int j = 0; j < matches[i].size(); ++j) {
				auto x = matches[i][j].x;
				auto y = matches[i][j].y;
				_map[y][x] = " ";
				std::this_thread::sleep_for(100ms);
				Tick();
			}
		}

		for (int i = 0; i < matches.size(); ++i) {
			for (int j = 0; j < matches[i].size(); ++j) {
				affectAbove(matches[i][j]);
			}
		}

		addNewPieces();
	}

	std::vector<std::vector<Point>> Model::lookForMatches() {
		std::vector<std::vector<Point>> matchList;

		for (int i = 0; i < HEIGHT; ++i) {
			for (int j = 0; j < WIDTH - 2; ++j) {
				auto match = getMatchHorizontal(j, i);
				if (match.size() > 2) {
					matchList.push_back(match);
					j += match.size() - 1;
				}
			}
		}

		for (int i = 0; i < WIDTH; ++i) {
			for (int j = 0; j < HEIGHT; ++j) {
				auto match = getMatchVertical(j, i);
				if (match.size() > 2) {
					matchList.push_back(match);
					j += match.size() - 1;
				}
			}
		}

		return matchList;
	}

	std::vector<Point> Model::getMatchHorizontal(int x, int y) {
		std::vector<Point> match;
		match.push_back({ x, y });
		for (int i = 1; x + i < WIDTH; ++i) {
			if (_map[y][x] == _map[y][x + i]) {
				match.push_back({ x + i, y });
			}
			else {
				return match;
			}
		}
		return match;
	}

	std::vector<Point> Model::getMatchVertical(int x, int y) {
		std::vector<Point> match;
		match.push_back({ x, y });
		for (int i = 1; y + i < HEIGHT; ++i) {
			if (_map[y][x] == _map[y + i][x]) {
				match.push_back({ x, y + i });
			}
			else {
				return match;
			}
		}
		return match;
	}

	void Model::affectAbove(Point point) {
		for (int row = point.y; row > 0; --row) {
			if (_map[row][point.x] == " ") {
				_map[row][point.x] = _map[row - 1][point.x];
				_map[row - 1][point.x] = " ";
				std::this_thread::sleep_for(1s);
				Tick();
			}
		}
	}

	void Model::addNewPieces() {
		for (int i = 0; i < WIDTH; ++i) {
			for (int j = 0; j < HEIGHT; ++j) {
				if (_map[j][i] == " ") {
					_map[j][i] = _crystalls[_distro(_random_engine)];
				}
			}
		}
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
		Point to{ 0, 0 };
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
		Move(from, to);
	}
}