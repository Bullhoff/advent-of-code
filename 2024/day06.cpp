#include "headerfile.h"
#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <unistd.h>
#include <utility>
#include <cmath>
#include <vector>
#include <map>
#include <set>

inline void update_dir(std::pair<int, int> &dir) {
	int &dirX = dir.first;
	int &dirY = dir.second;
	if (dirX == -1) {
		dirX = 0;
		dirY = -1;
	} else if (dirY == -1) {
		dirY = 0;
		dirX = 1;
	} else if (dir.first == 1) {
		dirX = 0;
		dirY = 1;
	} else if (dir.second == 1) {
		dirY = 0;
		dirX = -1;
	}
}

inline bool check_path_with_new_wall(const std::vector<std::string> &grid, std::pair<int, int> dir, std::pair<int, int> pos) {
	const std::pair<int, int> startPos = pos;
	const std::pair<int, int> newWall = pos;
	int &posX = pos.first;
	int &posY = pos.second;
	int &dirX = dir.first;
	int &dirY = dir.second;
	posX -= dirX;
	posY -= dirY;
	std::map<std::pair<int, int>, std::set<std::pair<int, int>>> dirs{{pos, {dir}}};
	for (;;) {
		posX += dirX;
		posY += dirY;
		if (posX < 0 || posY < 0 || posX >= grid[0].size() - 0 || posY >= grid.size() - 0) break;
		if (grid[posY][posX] == '#' || (posX == newWall.first && posY == newWall.second)) {
			posX -= dirX;
			posY -= dirY;
			update_dir(dir);
		} else {
			if (!dirs.count(pos)) {
				dirs[pos] = {dir};
			} else if (dirs[pos].count(dir)) {
				return true;
			} else {
				dirs[pos].insert(dir);
			}
		}
	}
	return false;
}

int main() {
	auto t0 = std::chrono::high_resolution_clock::now();
	int part1 = 0;
	int part2 = 0;

	std::vector<std::string> grid;
	std::pair<int, int> pos;
	std::ifstream inputFile("data/day06.txt");
	if (inputFile.is_open()) {
		std::string line;
		int y = 0;
		while (std::getline(inputFile, line)) {
			for (size_t x = 0; x < line.size(); ++x) {
				if (line[x] == '^') { pos = std::make_pair(x, y); }
			}
			grid.push_back(line);
			y++;
		}
		inputFile.close();
	}

	const std::pair<int, int> startPos = pos;
	int &posX = pos.first;
	int &posY = pos.second;
	std::pair<int, int> dir = std::make_pair(0, -1);
	int &dirX = dir.first;
	int &dirY = dir.second;
	std::map<std::pair<int, int>, std::set<std::pair<int, int>>> dirs{{pos, {dir}}};
	for (;;) {
		posX += dirX;
		posY += dirY;
		if (posX < 0 || posY < 0 || posX >= grid[0].size() - 0 || posY >= grid.size() - 0) break;
		if (grid[posY][posX] == '#') {
			posX -= dirX;
			posY -= dirY;
			update_dir(dir);
		} else {
			if (!dirs.count(pos)) {
				dirs[pos] = {dir};
				if (posX != startPos.first || posY != startPos.second)
					if (check_path_with_new_wall(grid, dir, pos)) { part2 += 1; }
			} else {
				dirs[pos].insert(dir);
			}
		}
	}
	part1 = dirs.size();
	std::cout << DEBUG_INFO_S << duration_str(t0) << "  part1=\x1b[1m\x1b[38;2;155;255;15m" << part1 << "\x1b[0m part2=\x1b[1m\x1b[38;2;155;255;15m" << part2 << "\x1b[0m" << std::endl;
	return 0;
}
