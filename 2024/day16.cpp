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
#include <set>
#include <thread>
#include <csignal>

#define 🍕 true
bool PRINTMANYIT = false;
bool PRINTIT = false;
#define DELAY 0
#define GRID_COLUMN_WIDTH 2
#define COLOREXPLORED "\x1b[48;2;85;85;0m"
#define COLORPATH "\x1b[48;2;0;255;0m"
#define COLORWALL ""
#define COLOREMPTY "\x1b[8m"
#define COLORSTART "\x1b[38;2;0;128;128m\x1b[48;2;0;128;128m"
#define COLOREND "\x1b[38;2;255;0;0m\x1b[48;2;255;0;0m"
void exitHandler() {
	if (PRINTMANYIT) std::cout << "\x1b[?25h" << std::flush; /// show cursor
	if (PRINTMANYIT) std::cout << "\x1b[?7h" << std::flush; /// enable wrapping
	if (PRINTMANYIT) std::cout << "\x1b[9999999;1H" << std::flush; /// move cursor to end
}
void signalHandler(int signum) {
	std::exit(signum);
}

struct sCoord {
	int x, y;
	int pX = -1;
	int pY = -1;
	int cost = 0;
	int turns = 0;
	int steps = 0;
	bool pp = false;
	char chr;
	sCoord() {}
	sCoord(int x, int y) : x(x), y(y) {}
	sCoord(int x, int y, int pX, int pY) : x(x), y(y), pX(pX), pY(pY) {}
	sCoord(int x, int y, int pX, int pY, int cost, int turns) : x(x), y(y), pX(pX), pY(pY), cost(cost), turns(turns) {}
	friend std::ostream &operator<<(std::ostream &os, const sCoord &coord) { return os << "[" << coord.x << "," << coord.y << "]"; }
	sCoord rotate(bool r = false) {
		if (!r && y != 0) y = -y; 
		else if (r && x != 0) x = -x; 
		x ^= y;
		y ^= x;
		x ^= y;
		return *this;
	}
	void set(int x, int y) {
		this->x = x;
		this->y = y;
	};
	sCoord dir() const {
		if (pX != -1 && pY != -1) { return sCoord(x - pX, y - pY); }
		return sCoord(0, 0);
	}
	bool operator<(const sCoord &other) const {
		if (x == other.x) { return y < other.y; }
		return x < other.x;
	}
	bool operator==(const sCoord &other) const { return x == other.x && y == other.y; }
	bool operator!=(const sCoord &other) const { return x != other.x || y != other.y; }
};
inline sCoord operator+(const sCoord &a, const sCoord &b) {
	return sCoord{a.x + b.x, a.y + b.y};
}

inline bool isInGrid(const sCoord &coord, const std::vector<std::string> &grid) {
	return coord.x >= 0 && coord.y >= 0 && coord.x < grid[0].size() && coord.y < grid.size();
}

void readFile(const std::string &file, std::vector<std::string> &arr) {
	std::ifstream inputFile(file);
	if (inputFile.is_open()) {
		std::string line;
		while (std::getline(inputFile, line)) { arr.emplace_back(line); }
		inputFile.close();
	}
}

void printGrid(const std::vector<std::string> &grid, const std::set<sCoord> hls = std::set<sCoord>(), int cellWidth = GRID_COLUMN_WIDTH) {
	std::ostringstream oss;
	for (size_t y = 0; y < grid.size(); ++y) {
		for (size_t x = 0; x < grid[y].size(); ++x) {
			char chr = grid[y][x];
			if (chr == '.') oss << COLOREMPTY;
			else if (chr == '#') oss << COLORWALL;
			else if (chr == 'S') oss << COLORSTART;
			else if (chr == 'E') oss << COLOREND;
			for (const sCoord &p : hls) {
				if (p.x == x && p.y == y) oss << COLORPATH;
			}
			for (size_t i = 0; i < cellWidth; ++i) { oss << "" << grid[y][x] << ""; }
			oss << "\x1b[0m";
		}
		if (y != grid.size() - 1) oss << std::endl;
	}
	std::cout << oss.str() << std::flush;
}

std::vector<sCoord> getShortestPath(std::vector<std::vector<sCoord>> &grid, const sCoord &startPos, const sCoord &endPos, bool rec = true) {
	std::vector<sCoord> projectedPath;
	sCoord curr = grid[endPos.y][endPos.x];
	projectedPath.emplace_back(curr);
	while (true) {
		if (rec) grid[curr.x][curr.x].pp = true;
		projectedPath.emplace_back(curr);
		if (startPos.x == curr.x && startPos.y == curr.y) break;
		curr = grid[curr.pY][curr.pX];
	}
	return projectedPath;
}

int32_t main(int argc, char *argv[]) {
	if (argc > 1) PRINTIT = true;
	if ((argc > 1 && argv[1][0] == 'a') || (argc > 1 && strlen(argv[1]) > 1)) PRINTMANYIT = true;
	std::atexit(exitHandler);
	std::signal(SIGINT, signalHandler);
	std::signal(SIGTERM, signalHandler);
	std::signal(SIGABRT, signalHandler);
	auto t0 = std::chrono::high_resolution_clock::now();
	int part1 = 0;
	int part2 = 0;
	std::vector<std::string> arr;
	readFile("data/day16.txt", arr);
	std::vector<std::vector<sCoord>> grid(arr.size(), std::vector<sCoord>(arr[0].size(), sCoord())); 
	std::vector<std::string> gridChecked(arr);
	sCoord gridSize(arr[0].size(), arr.size());
	sCoord startPos;
	sCoord endPos;
	for (size_t y = 0; y < arr.size(); ++y) {
		for (size_t x = 0; x < arr[y].size(); ++x) {
			grid[y][x].chr = arr[y][x];
			grid[y][x].y = y;
			grid[y][x].x = x;
			if (arr[y][x] == 'S') {
				startPos.set(x, y);
				startPos.pX = x - 1;
				startPos.pY = y;
			}
			if (arr[y][x] == 'E') { endPos.set(x, y); }
		}
	}

	if (PRINTMANYIT) {
		std::ios_base::sync_with_stdio(false);
		std::cin.tie(NULL);
		std::cout << "\x1b[?25l" << std::flush; /// hide cursor
		std::cout << "\x1b[?7l" << std::flush; /// disable wrapping
		printGrid(arr);
	}
	auto setCells = [&arr](const std::vector<sCoord> &path, std::string style = "") -> void {
		std::ostringstream oss;
		for (size_t i = 0; i < path.size(); ++i) {
			oss << "\x1b[999999;1H";
			oss << "\x1b[" << (arr.size() - (path[i].y + 1)) << "A";
			oss << "\x1B[" << path[i].x * GRID_COLUMN_WIDTH << "C";
			if (arr[path[i].y][path[i].x] == '.') oss << COLOREMPTY;
			else if (arr[path[i].y][path[i].x] == 'S') oss << COLORSTART;
			else if (arr[path[i].y][path[i].x] == 'E') oss << COLOREND;
			oss << style;
			for (size_t i = 0; i < GRID_COLUMN_WIDTH; ++i) oss << arr[path[i].y][path[i].x];
			oss << "\x1b[0m";
		}
		std::cout << oss.str() << std::flush;
	};

	std::vector<sCoord> shortestPath0;
	std::vector<sCoord> shortestPath;
	std::vector<sCoord> queue({startPos});
	std::vector<sCoord> dirs{{-1, 0}, {0, 1}, {0, -1}, {1, 0}};
	sCoord curr;
	while (queue.size() > 0) {
		std::sort(queue.begin(), queue.end(), [](const auto &a, const auto &b) { return a.cost > b.cost; });
		curr = queue.back();
		queue.pop_back();
		if (curr.y != -1 && curr.x != -1 && gridChecked[curr.y][curr.x] == 'x') {
			sCoord dirC = curr.dir();
			sCoord dirN = grid[curr.y][curr.x].dir();
			if (grid[curr.y + dirN.y][curr.x + dirN.x].chr != '#') continue;
			if (grid[curr.y + dirC.y][curr.x + dirC.x].chr == '#') continue;
		}
		grid[curr.y][curr.x] = curr; 
		gridChecked[curr.y][curr.x] = 'x';
		if (curr == endPos) break; 
		if (curr.pX != -1 && curr.pY != -1) {
			dirs.clear();
			sCoord dir = curr.dir();
			dirs.emplace_back(dir);
			dirs.emplace_back(dir.rotate());
			dir.rotate();
			dirs.emplace_back(dir.rotate());
		}
		for (unsigned int i = 0; i < dirs.size(); i++) {
			const auto dir = curr + dirs[i];
			if (!isInGrid(dir, arr)) continue;
			if (gridChecked[dir.y][dir.x] == '#') continue;
			queue.emplace_back(sCoord(dir.x, dir.y, curr.x, curr.y, curr.cost + 1, curr.turns));
			queue.back().steps += curr.steps + 1;
			if (dirs[i] != curr.dir()) {
				queue.back().cost += 1000 * 1;
				queue.back().turns += 1;
			}
		}
		if (PRINTMANYIT) {
			shortestPath = getShortestPath(grid, startPos, curr, false);
			if (shortestPath0.size() > 0) setCells(shortestPath0, COLOREXPLORED);
			setCells(shortestPath, COLORPATH);
			shortestPath0 = shortestPath;
			std::this_thread::sleep_for(std::chrono::microseconds(DELAY));
		}
	}
	if (PRINTMANYIT) setCells(shortestPath, COLOREXPLORED);
	std::vector<sCoord> projectedPath = getShortestPath(grid, startPos, endPos);
	part1 = grid[endPos.y][endPos.x].cost;

	/// part2
	std::vector<sCoord> anotherQueue;
	std::set<sCoord> bestSpots;
	for (size_t i = 1; i < projectedPath.size(); ++i) {
		sCoord &prev = projectedPath[i - 1];
		sCoord &pp = projectedPath[i];
		bestSpots.insert(pp);
		sCoord dir = pp.dir();
		std::vector<sCoord> toCheck;
		toCheck.push_back(grid[pp.y + dir.y][pp.x + dir.x]);
		dir.rotate();
		toCheck.push_back(grid[pp.y + dir.y][pp.x + dir.x]);
		dir.rotate();
		toCheck.push_back(grid[pp.y + dir.y][pp.x + dir.x]);
		dir.rotate();
		toCheck.push_back(grid[pp.y + dir.y][pp.x + dir.x]);
		dir.rotate();
		for (size_t j = 0; j < toCheck.size(); ++j) {
			sCoord &p = toCheck[j];
			if (gridChecked[p.y][p.x] == '#') continue;
			if (grid[p.y][p.x].pp) continue;
			if (p.cost + 1000 + 1 == pp.cost) {
				bestSpots.insert(p);
				anotherQueue.push_back(p);
			} else if (p.cost + 1 == pp.cost) {
				bestSpots.insert(p);
				anotherQueue.push_back(p);
			}
		}
		while (anotherQueue.size() > 0) {
			sCoord p = anotherQueue.back();
			anotherQueue.pop_back();
			if (!isInGrid(p, arr)) continue;
			bestSpots.insert(p);
			grid[p.y][p.x].pp = true;
			if (gridChecked[p.pY][p.pX] == '#') continue;
			if (p.pY == -1) continue;
			anotherQueue.push_back(grid[p.pY][p.pX]);
			sCoord dir2 = p.dir();
			if (p != grid[p.y + dir2.y][p.x + dir2.x] && grid[p.pY][p.pX] != grid[p.y + dir2.y][p.x + dir2.x] && grid[p.y + dir2.y][p.x + dir2.x].chr != '#' && grid[p.y + dir2.y][p.x + dir2.x].cost == p.cost - 1001) anotherQueue.push_back(grid[p.y + dir2.y][p.x + dir2.x]);
			if (isInGrid(sCoord(p.x - dir2.x * 2, p.y - dir2.y * 2), arr))
				if (grid[p.y - dir2.y * 2][p.x - dir2.x * 2].dir() == p.dir())
					if (!grid[p.y - dir2.y * 2][p.x - dir2.x * 2].pp)
						if (grid[p.y - dir2.y * 2][p.x - dir2.x * 2].cost == p.cost - 2) anotherQueue.push_back(grid[p.y - dir2.y * 2][p.x - dir2.x * 2]);
			dir2.rotate();
			if (p != grid[p.y + dir2.y][p.x + dir2.x] && grid[p.pY][p.pX] != grid[p.y + dir2.y][p.x + dir2.x] && grid[p.y + dir2.y][p.x + dir2.x].chr != '#' && grid[p.y + dir2.y][p.x + dir2.x].cost == p.cost - 1001) anotherQueue.push_back(grid[p.y + dir2.y][p.x + dir2.x]);
			dir2.rotate();
			if (p != grid[p.y + dir2.y][p.x + dir2.x] && grid[p.pY][p.pX] != grid[p.y + dir2.y][p.x + dir2.x] && grid[p.y + dir2.y][p.x + dir2.x].chr != '#' && grid[p.y + dir2.y][p.x + dir2.x].cost == p.cost - 1001) anotherQueue.push_back(grid[p.y + dir2.y][p.x + dir2.x]);
			dir2.rotate();
			if (p != grid[p.y + dir2.y][p.x + dir2.x] && grid[p.pY][p.pX] != grid[p.y + dir2.y][p.x + dir2.x] && grid[p.y + dir2.y][p.x + dir2.x].chr != '#' && grid[p.y + dir2.y][p.x + dir2.x].cost == p.cost - 1001) anotherQueue.push_back(grid[p.y + dir2.y][p.x + dir2.x]);
		}
	}
	if (🍕== true) part2 = bestSpots.size();

	if (PRINTMANYIT) {
		setCells(std::vector<sCoord>(bestSpots.begin(), bestSpots.end()), COLORPATH);
		std::cout << "\033[9999999;1H" << std::endl; /// move cursor to end
	} else if (PRINTIT) {
		printGrid(arr, bestSpots, 1);
		std::cout << std::endl;
	}

	std::cout << DEBUG_INFO_S << duration_str(t0) << "  part1=\x1b[1m\x1b[38;2;155;255;15m" << part1 << "\x1b[0m part2=\x1b[1m\x1b[38;2;155;255;15m" << part2 << "\x1b[0m" << std::endl;
	return 0;
}
