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
#include <map>
#include <set>
#include <functional>

struct sCoord {
	int x, y;
	int pX = -1;
	int pY = -1;
	int cost = 0;
	int turns = 0;

	sCoord() {}
	sCoord(int x, int y) : x(x), y(y) {}
	sCoord(int x, int y, int pX, int pY) : x(x), y(y), pX(pX), pY(pY) {}
	sCoord(int x, int y, int pX, int pY, int cost, int turns) : x(x), y(y), pX(pX), pY(pY), cost(cost), turns(turns) {}
	friend std::ostream &operator<<(std::ostream &os, const sCoord &coord) { return os << "[" << coord.x << "," << coord.y << "]"; }
	sCoord rotate() {
		if (y == -1) y = 1;
		else if (y == 1) y = -1;
		x ^= y;
		y ^= x;
		x ^= y;
		return sCoord(x, y);
	}
	void set(int x, int y) {
		this->x = x;
		this->y = y;
	};
	sCoord dir() const {
		if (pX != -1 && pY != -1) { return sCoord(x - pX, y - pY); }
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

inline bool isInGrid(const sCoord &coord, const std::vector<std::string> &grid) {
	return coord.x >= 0 && coord.y >= 0 && coord.x < grid[0].size() && coord.y < grid.size();
}

int getIndex(const std::vector<sCoord> &v, const std::function<bool(const sCoord &)> &predicate) {
	auto it = std::find_if(v.begin(), v.end(), predicate);
	if (it != v.end()) return static_cast<int>(std::distance(v.begin(), it));
	else return -1;
}

void readFile(const std::string &file, std::vector<std::string> &arr) {
	std::ifstream inputFile(file);
	if (inputFile.is_open()) {
		std::string line;
		while (std::getline(inputFile, line)) { arr.emplace_back(line); }
		inputFile.close();
	}
}

void printGrid(const std::vector<std::string> &arr) {
	for (size_t y = 0; y < arr.size(); ++y) {
		for (size_t x = 0; x < arr[y].size(); ++x) {
			if (arr[y][x] == '#') std::cout << "\x1b[48;2;155;155;155m";
			std::cout << "" << arr[y][x] << "\x1b[0m";
		}
		std::cout << std::endl;
	}
}
void printGrid(const std::vector<std::string> &arr, const std::vector<sCoord> &hl, const sCoord &endPos = sCoord(), const bool &replace = false) {
	if (replace) { std::cout << "\033[" << arr.size() << "A"; }
	for (size_t y = 0; y < arr.size(); ++y) {
		for (size_t x = 0; x < arr[y].size(); ++x) {
			if (y == endPos.y && x == endPos.x) std::cout << "\x1b[48;2;0;0;250m\x1b[38;2;0;0;250m";
			else if (arr[y][x] == '#') std::cout << "\x1b[48;2;0;0;0m\x1b[38;2;0;0;0m";
			else if (arr[y][x] == 'x') std::cout << "\x1b[48;2;155;155;0m";
			else std::cout << "\x1b[48;2;155;155;155m";
			for (size_t i = 0; i < hl.size(); ++i) {
				if (hl[i].x == x && hl[i].y == y) {
					std::cout << "\x1b[48;2;0;255;0m";
					break;
				}
			}
			std::cout << " " << arr[y][x] << "\x1b[0m";
		}
		std::cout << std::endl;
	}
}
std::map<int, std::string> hls;
void printGrid(const std::vector<std::string> &arr, std::vector<sCoord> hl, std::vector<std::vector<sCoord>> arr2 = std::vector<std::vector<sCoord>>()) {
	for (size_t y = 0; y < arr.size(); ++y) {
		for (size_t x = 0; x < arr[y].size(); ++x) {
			if (arr[y][x] == '#') std::cout << "\x1b[48;2;155;155;155m";
			for (size_t i = 0; i < hl.size(); ++i) {
				if (hl[i].x == x && hl[i].y == y) std::cout << "\x1b[48;2;155;255;155m";
			}
			if (arr[y][x] == '.') std::cout << "\x1b[48;2;255;0;0m";
			if (arr[y][x] == '.') std::cout << "\x1b[38;2;255;0;0m";
			if (arr2.size() > y && arr2[y][x].turns > 0) {
				if (!hls.count(arr2[y][x].turns)) hls[arr2[y][x].turns] = generateRandomColor();
				std::cout << hls[arr2[y][x].turns];
			}
			std::cout << "" << arr[y][x] << "\x1b[0m";
		}
		std::cout << std::endl;
	}
}

std::vector<sCoord> getShortestPath(const std::vector<sCoord> &processed, const sCoord &startPos, const sCoord &endPos) {
	sCoord curr;
	std::vector<sCoord> projectedPath;
	int index = getIndex(processed, [&endPos](const sCoord &coord) -> bool { return endPos.x == coord.x && endPos.y == coord.y; });
	curr = processed[index];
	projectedPath.emplace_back(curr);
	while (true) {
		index = getIndex(processed, [&curr](const sCoord &coord) -> bool { return curr.pX == coord.x && curr.pY == coord.y; });
		if (index == -1) { break; }
		curr = processed[index];
		projectedPath.emplace_back(curr);
		if (startPos.x == curr.x && startPos.y == curr.y) break;
	}
	return projectedPath;
}

int32_t main(int argc, char *argv[]) {
	auto t0 = std::chrono::high_resolution_clock::now();
	bool printmanystuff = (argc > 1 && argv[1][0] == 'a') ? true : false;
	bool printstuff = (argc > 1) ? true : false;
	bool printed = false;
	int part1 = 0;
	int part2 = 0;
	std::vector<std::string> arr;
	readFile("data/day16.txt", arr);
	sCoord gridSize(arr[0].size(), arr.size());
	sCoord startPos;
	sCoord endPos;
	for (size_t y = 0; y < arr.size(); ++y) {
		for (size_t x = 0; x < arr[y].size(); ++x) {
			if (arr[y][x] == 'S') {
				startPos.set(x, y);
				startPos.pX = x - 1;
				startPos.pY = y;
			}
			if (arr[y][x] == 'E') { endPos.set(x, y); }
		}
	}
	std::vector<std::string> gridChecked(arr);
	std::vector<sCoord> queue({startPos});
	std::vector<sCoord> processed;
	sCoord curr;
	std::vector<std::vector<sCoord>> grid(arr.size(), std::vector<sCoord>(arr[0].size(), sCoord())); /// for part2
	std::vector<sCoord> dirs{{-1, 0}, {0, 1}, {0, -1}, {1, 0}};
	while (queue.size() > 0) {
		std::sort(queue.begin(), queue.end(), [](const auto &a, const auto &b) { return a.cost > b.cost; });
		curr = queue.back();
		queue.pop_back();
		processed.emplace_back(curr);
		grid[processed.back().y][processed.back().x] = processed.back(); /// for part2
		gridChecked[curr.y][curr.x] = 'x';
		if (false && curr == endPos) break; /// false because part2
		if (curr.pX != -1 && curr.pY != -1) {
			dirs.clear();
			sCoord dir = curr.dir();
			dirs.emplace_back(dir);
			dirs.emplace_back(dir.rotate());
			dir.rotate();
			dirs.emplace_back(dir.rotate());
		}
		for (unsigned int i = 0; i < dirs.size(); i++) {
			const sCoord dir = curr + dirs[i];
			if (!isInGrid(dir, arr)) continue;
			if (gridChecked[dir.y][dir.x] == 'x' || gridChecked[dir.y][dir.x] == '#') continue;
			gridChecked[dir.y][dir.x] = 'x';
			queue.emplace_back(sCoord(dir.x, dir.y, curr.x, curr.y, curr.cost + 1, curr.turns));
			if (dirs[i] != curr.dir()) {
				queue.back().cost += 1000;
				queue.back().turns += 1;
			}
		}
		if (printmanystuff)
			if (abs(queue.back().x - curr.x) > 15 || abs(queue.back().y - curr.y) > 15) {
				std::vector<sCoord> shortestPath = getShortestPath(processed, startPos, processed.back());
				if (!printed) { system("clear"); }
				printGrid(gridChecked, shortestPath, endPos, printed);
				printed = true;
			}
	}
	std::vector<sCoord> projectedPath = getShortestPath(processed, startPos, endPos);
	std::reverse(projectedPath.begin(), projectedPath.end());
	for (size_t i = 1; i < projectedPath.size(); ++i) {
		sCoord &path = projectedPath[i];
		sCoord &p1 = projectedPath[i - 1];
		sCoord &p2 = projectedPath[i];
		if (p1 == p2) continue;
		part1++;
		if ((p1.dir().x == 0 && p1.dir().y == 0) || (p2.dir().x == 0 && p2.dir().y == 0)) continue;
		if (!(p1.dir().x == 0 && p1.dir().y == 0) || !(p2.dir().x == 0 && p2.dir().y == 0))
			if (p1.dir() != p2.dir()) { part1 += 1000; }
	}

	/// "part 2"
	std::vector<sCoord> anotherQueue({projectedPath.back()});
	std::set<sCoord> bestSpots;
	while (anotherQueue.size() > 0) {
		curr = anotherQueue.back();
		anotherQueue.pop_back();
		bestSpots.insert(curr);
		std::vector<sCoord> costs;
		sCoord dir = curr.dir();
		if (grid[curr.y + dir.y][curr.x + dir.x].cost > 0) { costs.push_back(grid[curr.y + dir.y][curr.x + dir.x]); }
		dir.rotate();
		if (grid[curr.y + dir.y][curr.x + dir.x].cost > 0) { costs.push_back(grid[curr.y + dir.y][curr.x + dir.x]); }
		dir.rotate();
		if (grid[curr.y + dir.y][curr.x + dir.x].cost > 0) { costs.push_back(grid[curr.y + dir.y][curr.x + dir.x]); }
		dir.rotate();
		if (grid[curr.y + dir.y][curr.x + dir.x].cost > 0) { costs.push_back(grid[curr.y + dir.y][curr.x + dir.x]); }
		for (size_t i = 0; i < costs.size(); ++i) {
			int thecost = costs[i].cost;
			if (curr.turns != costs[i].turns && curr != endPos) { thecost += 1000 * (curr.turns - costs[i].turns); }
			const bool is_in = bestSpots.find(costs[i]) != bestSpots.end();
			if (!is_in && thecost + 1 == curr.cost) { anotherQueue.push_back(costs[i]); }

		}
	}
	if (printstuff) printGrid(gridChecked, std::vector<sCoord>(bestSpots.begin(), bestSpots.end()), grid);
	part2 = bestSpots.size() + 1;

	std::cout << DEBUG_INFO_S << duration_str(t0) << "  part1=\x1b[1m\x1b[38;2;155;255;15m" << part1 << "\x1b[0m part2=\x1b[1m\x1b[38;2;155;255;15m" << part2 << "\x1b[0m" << std::endl;
	return 0;
}
