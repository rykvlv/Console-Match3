#pragma once

#include "Model.h"
#include <iostream>
#include <thread>

using namespace std::chrono_literals;

namespace RBW {
	Model::Model() :
		_visualizer(std::make_unique<Visualizer>()),
		_distro { std::uniform_int_distribution<int>{ 0, COUNT_OF_CRYSTALLS - 1 }},
		_g{_rd()},
		_ticker{ 0 } {

	}

	void Model::Init() {
		_isClosed = false;
		fillMapRandomly();
		Run();
	}

	void Model::Run() {
		Tick();
		while (!_isClosed) {
			if (getInputAndTryProcess()) {
				findAndRemoveMatches();
				if (!lookForPossibleMoves()) {
					Mix();
				}
				Tick();
			}
			Dump();
		}
	}

	void Model::Tick() {
		std::this_thread::sleep_for(50ms);
		_visualizer->Draw(_map);
		++_ticker;
	}

	void Model::Dump() {
		_ticker = 0;
	}

	void Model::Move(Point from, Point to) {
		std::swap(_map[from.y][from.x], _map[to.y][to.x]);
		if (lookForMatches().size() == 0) {
			std::swap(_map[from.y][from.x], _map[to.y][to.x]);
		}
	}

	void Model::Mix() {
		std::shuffle(_map.begin(), _map.end(), _g);
		for (auto& row : _map) {
			std::shuffle(row.begin(), row.end(), _g);
		}
		if (!lookForPossibleMoves() && lookForMatches().size() == 0) {
			Mix();
		}
	}

	void Model::fillMapRandomly() {
		for (auto& row : _map) {
			for (auto& elem : row) {
				elem = _crystalls[_distro(_random_engine)];
			}
		}

		while (lookForMatches().size() != 0 || !lookForPossibleMoves()) {
			fillMapRandomly();
		}
	}

	bool Model::lookForPossibleMoves() {
		for (int i = 0; i < WIDTH; ++i) {
			for (int j = 0; j < HEIGHT; ++j) {
				if (matchPattern(i, j,
					{ {0, 1} },
					{ {-2, 0}, {-1, -1}, {-1, 1}, {2, -1}, {2, 1}, {3, 0} })) {
					return true;
				}

				if (matchPattern(i, j, 
					{ {2, 0} },
					{ {1, -1}, {1, 1} })) {
					return true;
				}

				if (matchPattern(i, j,
					{ {0,1} },
					{ {0, -2}, {-1, -1}, {1, -1}, {-1, 2}, {1, 2}, {0, 3} })) {
					return true;
				}

				if (matchPattern(i, j,
					{ {0, 2} },
					{ {-1, 1}, {1, 1} })) {
					return true;
				}
			}
		}
		return false;
	}

	bool Model::matchPattern(int x, int y, std::vector<std::vector<int>> mustHave, std::vector <std::vector<int>> needOne) {
		auto type = _map[y][x];

		for (int i = 0; i < mustHave.size(); ++i) {
			if (!matchType(x + mustHave[i][0], y + mustHave[i][1], type)) {
				return false;
			}
		}

		for (int i = 0; i < needOne.size(); ++i) {
			if (matchType(x + needOne[i][0], y + needOne[i][1], type)) {
				return true;
			}
		}
		return false;
	}

	bool Model::matchType(int x, int y, std::string type) {
		if ((x < 0) || (x >= WIDTH) || (y < 0) || (y >= HEIGHT)) {
			return false;
		}
		return (_map[y][x] == type);
	}

	void Model::findAndRemoveMatches() {
		auto matches = lookForMatches();
		for (int i = 0; i < matches.size(); ++i) {
			for (int j = 0; j < matches[i].size(); ++j) {
				auto x = matches[i][j].x;
				auto y = matches[i][j].y;
				deleteCrystall(x, y);
				Tick();
			}
		}

		for (int i = 0; i < matches.size(); ++i) {
			for (int j = 0; j < matches[i].size(); ++j) {
				affectAbove(matches[i][j]);
			}
		}

		addNewCrystalls();

		if (lookForMatches().size() != 0) {
			findAndRemoveMatches();
		}
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
				Tick();
			}
		}
	}

	void Model::addNewCrystalls() {
		for (int i = 0; i < WIDTH; ++i) {
			for (int j = 0; j < HEIGHT; ++j) {
				if (_map[j][i] == " ") {
					_map[j][i] = _crystalls[_distro(_random_engine)];
					Tick();
				}
			}
		}
	}

	void Model::deleteCrystall(int x, int y) {
		/*
		* If we want to see crystalls with special effects, we need to implement crystall 
		* as individual class with methods such as performBonusCrystallAction()
		* if (_map[y][x].isSpecial) {
		*	_map[y][x].performBonusCrystallAction();
		* } 
		*/
		_map[y][x] = " ";
		Tick();
	}

	bool Model::getInputAndTryProcess() {
		Command command;

		if (std::cin >> command.type) {
			if (command.type == 'q') {
				_isClosed = true;
				return true;
			}
		}
		else {
			std::cout << "ERROR: Wrong input!" << std::endl;
			std::cin.clear();
			return false;
		}

		if (std::cin >>
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