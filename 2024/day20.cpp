#include "headerfile.h"
#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <ostream>
#include <string>
#include <unistd.h>
#include <vector>
#include <unordered_set>

struct sCoord {
	int x, y;
	int pX = -1;
	int pY = -1;
	int distanceLeft = 0;
	int distanceTravelled = 0;
	int cost = 0;
	bool checked = false;
	bool wall = false;
	char chr = '.';
	sCoord() {}
	sCoord(int x, int y) : x(x), y(y) {}
	sCoord(int x, int y, char chr) : x(x), y(y), chr(chr) {}
	sCoord(int x, int y, int pX, int pY) : x(x), y(y), pX(pX), pY(pY) {}
	sCoord(int x, int y, int pX, int pY, int distanceTravelled) : x(x), y(y), pX(pX), pY(pY), distanceTravelled(distanceTravelled) {}
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
	sCoord dir() {
		if (pX != -1 && pY != -1) return sCoord(x - pX, y - pY);
		return sCoord(0, 0);
	}
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

void readFile(const std::string &filename, std::vector<std::string> &arr) {
	std::ifstream file(filename);
	if (!file.is_open()) { throw std::runtime_error("Unable to open file"); }
	std::string line;
	while (std::getline(file, line)) { arr.emplace_back(line); }
}

std::vector<sCoord> solve(std::vector<std::vector<sCoord>> &grid, const sCoord &gridSize, const sCoord &startPos, const sCoord &endPos) {
	std::vector<sCoord> queue({endPos});
	std::vector<sCoord> processed;
	std::vector<sCoord> dirs{{-1, 0}, {0, 1}, {0, -1}, {1, 0}};
	sCoord curr;
	grid[endPos.y][endPos.x].checked = true;
	while (queue.size() > 0) {
		std::sort(queue.begin(), queue.end(), [](const auto &a, const auto &b) { return a.distanceLeft > b.distanceLeft; });
		curr = queue.back();
		queue.pop_back();
		processed.emplace_back(curr);
		if (curr == startPos) { break; }
		for (unsigned int i = 0; i < dirs.size(); i++) {
			const sCoord dir = curr + dirs[i];
			if (!isInGrid(dir, gridSize)) continue;
			if (grid[dir.y][dir.x].checked || grid[dir.y][dir.x].wall) continue;
			grid[dir.y][dir.x].checked = true;
			grid[dir.y][dir.x].distanceLeft = curr.distanceLeft + 1;
			queue.emplace_back(sCoord(dir.x, dir.y, curr.x, curr.y, curr.distanceTravelled + 1));
			queue.back().distanceLeft = curr.distanceLeft + 1;
		}
	}
	return processed;
}

int32_t main() {
	auto t0 = std::chrono::high_resolution_clock::now();
	int part1 = 0;
	int part2 = 0;
	std::vector<std::string> arr;
	readFile("data/day20.txt", arr);
	sCoord gridSize(arr[0].size() - 0, arr.size() - 0);
	std::vector<std::vector<sCoord>> grid(gridSize.y, std::vector<sCoord>(gridSize.x, sCoord(-1, -1)));
	sCoord startPos;
	sCoord endPos;
	for (size_t y = 0; y < arr.size(); ++y) {
		for (size_t x = 0; x < arr[y].size(); ++x) {
			const char &chr = arr[y][x];
			grid[y][x].x = x;
			grid[y][x].y = y;
			grid[y][x].chr = chr;
			if (chr == '#') grid[y][x].wall = true;
			switch (arr[y][x]) {
			case 'S':
				startPos.set(x, y);
				break;
			case 'E':
				endPos.set(x, y);
				grid[y][x].distanceLeft = 0;
				break;
			}
		}
	}

	auto processed = solve(grid, gridSize, startPos, endPos);
	for (size_t y = 0; y < grid.size(); ++y) {
		for (size_t x = 0; x < grid[y].size(); ++x) {
			sCoord c = grid[y][x];
			auto checkPart1 = [&c, &grid, &gridSize, &part1, &part2](int dx, int dy) {
				if (!isInGrid(sCoord(c.x - dx, c.y - dy), gridSize)) return 0;
				if (!isInGrid(sCoord(c.x + dx, c.y + dy), gridSize)) return 0;
				sCoord &c1 = grid[c.y - dy][c.x - dx];
				sCoord &c2 = grid[c.y + dy][c.x + dx];
				if (c1.wall || c2.wall) return 0;
				int diff = abs(c1.distanceLeft - c2.distanceLeft) - 2;
				return (diff >= 100) ? 1 : 0;
			};
			if (c.wall) {
				part1 += checkPart1(0, 1);
				part1 += checkPart1(1, 0);
			}
		}
	}

	int minSave = (gridSize.x == 15 && gridSize.y == 15) ? 50 : 100;
	std::unordered_set<uint32_t> pairs;
	uint32_t compressed;
	int diffXY;
	int x1, y1, x2, y2;
	for (int y = 0; y < grid.size(); ++y) {
		for (int x = 0; x < grid[y].size(); ++x) {
			if (grid[y][x].wall) continue;
			int fX = (x < 20) ? 0 : (x - 20);
			int fY = (y < 20) ? 0 : (y - 20);
			int tX = (x > grid[y].size() - 1 - 20) ? (grid[y].size() - 1) : (x + 20);
			int tY = (y > grid.size() - 1 - 20) ? (grid.size() - 1) : (y + 20);
			for (int yy = fY; yy <= tY; ++yy) {
				for (int xx = fX; xx <= tX; ++xx) {
					const sCoord &c1 = grid[y][x];
					const sCoord &c2 = grid[yy][xx];
					if (x == xx && y == yy) continue;
					if (c2.wall) continue;
					diffXY = (abs(x - xx) + abs(y - yy));
					if (diffXY > 20) continue;
					compressed = (xx << 24) | (yy << 16) | (x << 8) | y;
					if (pairs.find(compressed) != pairs.end()) continue;
					pairs.insert(compressed);
					compressed = (x << 24) | (y << 16) | (xx << 8) | yy;
					if (pairs.find(compressed) != pairs.end()) continue;
					pairs.insert(compressed);
					int diff = abs(c2.distanceLeft - c1.distanceLeft) - diffXY;
					if (diff >= minSave) ++part2;
				}
			}
		}
	}

	std::cout << DEBUG_INFO_S << duration_str(t0) << "  part1=\x1b[1m\x1b[38;2;155;255;15m" << part1 << "\x1b[0m part2=\x1b[1m\x1b[38;2;155;255;15m" << part2 << "\x1b[0m" << std::endl;
	return 0;
}
