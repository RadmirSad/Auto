#pragma once
#include <vector>
#include <iostream>

enum directions {
	NORTH,
	SOUTH,
	EAST,
	WEST
};

class Robot {
private:
	std::pair<int, int> coord;
	char direct = NORTH;

	std::vector<std::vector<int>> my_map;
public:
	Robot() {};
	Robot(int x, int y, directions new_dir): coord(x, y), direct(new_dir) {};

	char get_dir() const { return direct; }
	void set_map(const std::vector<std::vector<int>>& new_map) { my_map = new_map; }

	int wall() const;
	bool exit() const;

	bool move(int num);

	void left();
	void right();
};