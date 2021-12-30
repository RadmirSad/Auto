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
		diff = y_coord + 1;
		for (; (before_wall < diff) && (my_map[y_coord - before_wall][x_coord] != WALL); before_wall++);
		break;
	case EAST:
		diff = x_sz - x_coord;
		for (; (before_wall < diff) && (my_map[y_coord][x_coord + before_wall] != WALL); before_wall++);
		break;
	case WEST:
		diff = x_coord + 1;
		for (; (before_wall < diff) && (my_map[y_coord][x_coord - before_wall] != WALL); before_wall++);
		break;
	case SOUTH:
		diff = y_sz - y_coord;
		for (; (before_wall < diff) && (my_map[y_coord + before_wall][x_coord] != WALL); before_wall++);
		break;
	}
	before_wall--;
	return before_wall;
}

bool Robot::exit() const {
	int y_sz = my_map.size(), x_sz = my_map[0].size(), diff = 0;
	switch (direct) {
	case NORTH:
		diff = y_coord + 1;
		for (int i = 0; (i < diff); i++) {
			objects obj = my_map[y_coord - i][x_coord];
			if (obj == EXIT)
				return true;
			else if(obj == WALL)
				return false;
		}
		break;
	case WEST:
		diff = x_coord + 1;
		for (int i = 0; (i < diff); i++) {
			objects obj = my_map[y_coord][x_coord - i];
			if (obj == EXIT)
				return true;
			else if(obj == WALL)
				return false;
		}
		break;
	case EAST:
		diff = x_sz - x_coord;
		for (int i = 0; (i < diff); i++) {
			objects obj = my_map[y_coord][x_coord + i];
			if (obj == EXIT)
				return true;
			else if(obj == WALL)
				return false;
		}
		break;
	case SOUTH:
		diff = y_sz - y_coord;
		for (int i = 0; (i < diff); i++) {
			objects obj = my_map[y_coord + i][x_coord];
			if (obj == EXIT)
				return true;
			else if(obj == WALL)
				return false;
		}
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
		for (int i = 1; i <= num; i++)
			if (my_map[y_coord - i][x_coord] == WALL)
				return false;
		for(int i = 0; i < num; i++) {
			y_coord--;
			print_map();
		}
		break;
	case EAST:
		if (num > x_sz - x_coord - 1)
			return false;
		for (int i = 1; i <= num; i++)
			if (my_map[y_coord][x_coord + i] == WALL)
				return false;
		for(int i = 0; i < num; i++) {
			x_coord++;
			print_map();
		}
		break;
	case WEST:
		if (num > x_coord)
			return false;
		for (int i = 1; i <= num; i++)
			if (my_map[y_coord][x_coord - i] == WALL)
				return false;
		for(int i = 0; i < num; i++) {
			x_coord--;
			print_map();
		}
		break;
	case SOUTH:
		if (num > y_sz - y_coord - 1)
			return false;
		for (int i = 1; i <= num; i++)
			if (my_map[y_coord + i][x_coord] == WALL)
				return false;
		for(int i = 0; i < num; i++) {
			y_coord++;
			print_map();
		}
		break;
	}
	return true;
}

void Robot::print_map() const {
	int sz_str = my_map.size(), sz_col = my_map[0].size();
	std::this_thread::sleep_for(std::chrono::milliseconds(50));
	std::system("clear");
	std::cout << std::endl;
	for (int i = 0; i <= sz_col; i++)
		if (i == 0) std::cout << "   ";
		else {
			if (i - 1 < 10) std::cout << " " << i - 1 << " ";
			else std::cout << " " << i - 1;
		}
	std::cout << " X " <<std::endl;
	for(int i = 0; i < sz_str; i++) {
		if (i < 10)
			std::cout << " " << i << " ";
		else std::cout << " " << i;
		for(int j = 0; j < sz_col; j++) {
			if((j == x_coord) && (i == y_coord)) {
				std::cout << "[";
				switch(direct) {
					case NORTH:
						std::cout << "^";
						break;
					case EAST:
						std::cout << ">";
						break;
					case WEST:
						std::cout << "<";
						break;
					case SOUTH:
						std::cout << "v";
						break;
				}
				std::cout << "]";
			}
			else {
				switch(my_map[i][j]) {
					case WALL:
						std::cout << " # ";
						break;
					case EMPTY:
						std::cout << "   ";
						break;
					case EXIT:
						std::cout << " | ";
						break;
				}
			}
			if (j + 1 == sz_col)
				std::cout << std::endl;
		}
	}
	std::cout << " Y " << std::endl << std::endl;
	std::this_thread::sleep_for(std::chrono::milliseconds(50));
}
