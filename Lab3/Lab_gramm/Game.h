#pragma once
#include <vector>
#include <iostream>

enum directions {
	NORTH,
	EAST,
	SOUTH,
	WEST
};

enum objects {
	WALL,
	EXIT,
	EMPTY
};

class Robot {
private:
	int x_coord = 0;
	int y_coord = 0;
	char direct = NORTH;

	std::vector<std::vector<objects>> my_map;
public:
	Robot() {};
	Robot(int x, int y, directions new_dir, const std::vector<std::vector<objects>>& new_map);

	char get_dir() const { return direct; }
	void set_map(const std::vector<std::vector<objects>>& new_map) { my_map = new_map; }

	int wall() const;
	bool exit() const;

	bool move(int num);

	void left() { direct = (direct + 3) % 4; }
	void right() { direct = (direct + 1) % 4; }
};