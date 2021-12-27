#include "Game.h"


Robot::Robot(int x, int y, directions new_dir, const std::vector<std::vector<objects>>& new_map) {
	if ((new_map.size() < 3) || (new_map[0].size() < 3))
		throw std::invalid_argument("Incorrect map");
	if ((x < 0) || (y < 0) || (x >=new_map[0].size()) || (y >= new_map.size()))
		throw std::invalid_argument("Incorrect coordinates.");
	if ((new_map[y][x] == WALL) || (new_map[y][x] == EXIT))
		throw std::invalid_argument("Incorrect start point for robot.");
	x_coord = x, y_coord = y;
	my_map = new_map;
	direct = new_dir;
}

int Robot::wall() const {
	int before_wall = 0, y_sz = my_map.size(), x_sz = my_map[0].size(), diff = 0;
	switch (direct) {
	case NORTH:
		diff = y_coord;
		for (; (before_wall < diff) && (my_map[y_coord - before_wall][x_coord] == EMPTY); before_wall++);
		break;
	case EAST:
		diff = x_sz - x_coord - 1;
		for (; (before_wall < diff) && (my_map[y_coord][x_coord + before_wall] == EMPTY); before_wall++);
		break;
	case WEST:
		diff = x_coord;
		for (; (before_wall < diff) && (my_map[y_coord][x_coord - before_wall] == EMPTY); before_wall++);
		break;
	case SOUTH:
		diff = y_sz - y_coord - 1;
		for (; (before_wall < diff) && (my_map[y_coord + before_wall][x_coord] == EMPTY); before_wall++);
		break;
	}
	before_wall--;
	return before_wall;
}

bool Robot::exit() const {
	int y_sz = my_map.size(), x_sz = my_map[0].size(), diff = 0;
	switch (direct) {
	case NORTH:
		diff = y_coord;
		for (int i = 0; (i < diff); i++)
			if (my_map[y_coord - i][x_coord] == EXIT)
				return true;
		break;
	case WEST:
		diff = x_coord;
		for (int i = 0; (i < diff); i++)
			if (my_map[y_coord][x_coord - i] == EXIT)
				return true;
		break;
	case EAST:
		diff = x_sz - x_coord - 1;
		for (int i = 0; (i < diff); i++)
			if (my_map[y_coord][x_coord + i] == EXIT)
				return true;
		break;
	case SOUTH:
		diff = y_sz - y_coord - 1;
		for (int i = 0; (i < diff); i++)
			if (my_map[y_coord + i][x_coord] == EXIT)
				return true;
		break;
	}
	return false;
}

bool Robot::move(int num) {
	int y_sz = my_map.size(), x_sz = my_map[0].size(), diff = 0;
	switch (direct)
	{
	case NORTH:
		if (num > y_coord)
			return false;
		for (int i = 0; i <= num; i++)
			if (my_map[y_coord - i][x_coord] == WALL)
				return false;
		y_coord -= num;
		break;
	case EAST:
		if (num > x_sz - x_coord - 1)
			return false;
		for (int i = 0; i <= num; i++)
			if (my_map[y_coord][x_coord + i] == WALL)
				return false;
		x_coord += num;
		break;
	case WEST:
		if (num > x_coord)
			return false;
		for (int i = 0; i <= num; i++)
			if (my_map[y_coord][x_coord - i] == WALL)
				return false;
		x_coord -= num;
		break;
	case SOUTH:
		if (num > y_sz - y_coord - 1)
			return false;
		for (int i = 0; i < num; i++)
			if (my_map[y_coord + i][x_coord] == WALL)
				return false;
		y_coord += num;
		break;
	}
	return true;
}