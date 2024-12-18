#include "headerfile.h"
#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <unistd.h>
#include <cmath>
#include <vector>

struct sCoord {
	int x, y;
	int distanceTravelled = 0;
	int cost = 0;
	sCoord() {}
	sCoord(int x, int y) : x(x), y(y) {}
	sCoord(int x, int y, int distanceTravelled, int distanceLeft) : x(x), y(y), distanceTravelled(distanceTravelled), cost(distanceTravelled + distanceLeft) {}
	friend std::ostream &operator<<(std::ostream &os, const sCoord &coord) { return os << "[" << coord.x << "," << coord.y << "]"; }
	sCoord rotate() {
		if (y != 0) y = -y;
		x ^= y;
		y ^= x;
		x ^= y;
		return *this;
	}
	void set(int x, int y) {
		this->x = x;
		this->y = y;
	};
	sCoord operator+=(const sCoord &other) {
		x += other.x;
		y += other.y;
		return *this;
	}
	sCoord operator-=(const sCoord &other) {
		x -= other.x;
		y -= other.y;
		return *this;
	}
	bool operator<(const sCoord &other) const {
		if (x == other.x) { return y < other.y; }
		return x < other.x;
	}
	bool operator>(const sCoord &other) const {
		if (x == other.x) { return y > other.y; }
		return x > other.x;
	}
	bool operator==(const sCoord &other) const { return x == other.x && y == other.y; }
	bool operator!=(const sCoord &other) const { return x != other.x || y != other.y; }
};
inline sCoord operator+(const sCoord &a, const sCoord &b) {
	return sCoord{a.x + b.x, a.y + b.y};
}
inline sCoord operator*(const sCoord &a, int d) {
	return sCoord{a.x * d, a.y * d};
}

inline bool isInGrid(const sCoord &coord, const sCoord &gridSize) {
	return coord.x >= 0 && coord.y >= 0 && coord.x < gridSize.x && coord.y < gridSize.y;
}

void readFile(const std::string &filename, std::vector<sCoord> &arr) {
	std::ifstream file(filename);
	if (!file.is_open()) { throw std::runtime_error("Unable to open file"); }
	std::string line;
	while (std::getline(file, line)) { arr.emplace_back(std::stoi(line.substr(0, line.find(','))), std::stoi(line.substr(line.find(',') + 1))); }
}

int solve(std::vector<std::vector<char>> gridChecked, const sCoord &gridSize, const sCoord &startPos, const sCoord &endPos, const bool &p2) {
	std::vector<sCoord> queue({startPos});
	std::vector<sCoord> dirs{{-1, 0}, {0, 1}, {0, -1}, {1, 0}};
	sCoord curr;
	int distanceTravelled = 0;
	while (queue.size() > 0) {
		if (!p2) std::sort(queue.begin(), queue.end(), [](const auto &a, const auto &b) { return a.cost > b.cost; });
		curr = queue.back();
		queue.pop_back();
		if (curr == endPos) {
			distanceTravelled = curr.distanceTravelled;
			break;
		}
		for (unsigned int i = 0; i < dirs.size(); i++) {
			const sCoord dir = curr + dirs[i];
			if (!isInGrid(dir, gridSize)) continue;
			if (gridChecked[dir.y][dir.x] == 'x' || gridChecked[dir.y][dir.x] == '#') continue;
			gridChecked[dir.y][dir.x] = 'x';
			queue.emplace_back(sCoord(dir.x, dir.y, curr.distanceTravelled + 1, ((endPos.x - dir.x) + (endPos.y - dir.y))));
		}
	}
	return distanceTravelled;
}

int32_t main(int32_t argc, char *argv[]) {
	auto t0 = std::chrono::high_resolution_clock::now();
	int part1 = 0;
	sCoord part2;
	std::vector<sCoord> coords;
	readFile("data/day18.txt", coords);
	sCoord gridSize(70 + 1, 70 + 1);
	sCoord startPos(0, 0);
	sCoord endPos(gridSize.x - 1, gridSize.y - 1);

	std::vector<std::vector<char>> gridChecked(gridSize.y, std::vector<char>(gridSize.x, '.'));
	for (size_t i = 0; i < coords.size(); ++i) {
		gridChecked[coords[i].y][coords[i].x] = '#';
		if (i >= 1024 - 1) {
			int distanceTravelled = solve(gridChecked, gridSize, startPos, endPos, ((part1 == 0) ? false : true));
			if (i == 1024 - 1) part1 = distanceTravelled;
			if (distanceTravelled == 0) {
				part2 = coords[i];
				break;
			}
		}
	}

	std::cout << DEBUG_INFO_S << duration_str(t0) << "  part1=\x1b[1m\x1b[38;2;155;255;15m" << part1 << "\x1b[0m part2=\x1b[1m\x1b[38;2;155;255;15m" << part2 << "\x1b[0m" << std::endl;
	return 0;
}
