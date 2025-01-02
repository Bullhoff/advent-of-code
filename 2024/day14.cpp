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
#include <sstream>

bool PRINTIT = false;
#define GRID_COLUMN_WIDTH 2

struct sCoord {
	int x, y;
	int vx, vy;
	sCoord() {}
	sCoord(int x, int y) : x(x), y(y) {}
	sCoord(int x, int y, int vx, int vy) : x(x), y(y), vx(vx), vy(vy) {}
};

void readFile(const std::string file, std::vector<sCoord> &arr) {
	std::ifstream inputFile(file);
	if (inputFile.is_open()) {
		std::string line;
		while (std::getline(inputFile, line)) {
			int sep = line.find(" ");
			std::string pos = line.substr(0, sep);
			std::string vel = line.substr(sep + 1);
			sep = pos.find(",");
			arr.emplace_back(std::stoi(pos.substr(2, pos.find(","))), std::stoi(pos.substr(pos.find(",") + 1)), std::stoi(vel.substr(2, vel.find(","))), std::stoi(vel.substr(vel.find(",") + 1)));
		}
		inputFile.close();
	}
}

void printGrid(const sCoord &gridSize, const std::vector<sCoord> &arr) {
	for (size_t y = 0; y < gridSize.y; ++y) {
		for (size_t x = 0; x < gridSize.x; ++x) {
			sCoord coord(x, y);
			int count = 0;
			for (size_t i = 0; i < arr.size(); ++i) {
				if (arr[i].x == x && arr[i].y == y) count++;
			}
			if (count == 0) std::cout << ".";
			else std::cout << "X\x1b[0m";
			std::cout << " ";
		}
		std::cout << std::endl;
	}
}

void printTree(const std::vector<std::vector<int8_t>> &grid, int x, int y, int inRow, const std::vector<sCoord> &arr, const sCoord &gridSize) {
	int x0 = x - 0;
	int x1 = -1;
	int y0 = y - inRow - 1;
	int y1 = y;
	for (size_t xx = x + 1; xx < grid[y].size(); ++xx) {
		bool ok = true;
		for (size_t yy = y0 + 1; yy < y1; ++yy) {
			if (grid[yy][xx] < 1) {
				ok = false;
				break;
			}
		}
		if (ok) {
			x1 = xx + 1;
			break;
		}
	}
	if (x1 != -1) {
		printGrid(gridSize, arr);
		std::ostringstream oss;
		bool topReached = false;
		bool botReached = false;
		for (size_t y = y0 + 1; y < y1; ++y) {
			for (size_t x = x0; x < x1; ++x) {
				std::string chr = "";
				if (y == y0 + 1 || y == y1 - 1 || x == x0 || x == x1 - 1) {
					chr = "\x1b[38;2;25;190;25m##\x1b[0m";
				} else if (!topReached && grid[y + 1][x] > 0 && x == (x0 + (x1 - x0) / 2)) {
					if (GRID_COLUMN_WIDTH == 2) chr += (rand(0, 10) > 8) ? "🍕" : "🌟";
					else chr += "\x1b[5m★\x1b[0m";
					topReached = true;
				} else if (grid[y][x] > 0) {
					int chrs = 0;
					if (botReached || (grid[y][x - 1] < 1 && grid[y + 1][x - 1] < 1 && grid[y - 1][x - 1] > 0)) {
						chrs = GRID_COLUMN_WIDTH;
						chr = "\x1b[38;2;190;95;0m" + std::string(GRID_COLUMN_WIDTH, '#');
						botReached = true;
					} else if (rand(0, 90) >= 69 && grid[y + 1][x] > 0 && grid[y][x - 1] > 0 && grid[y - 1][x] > 0) {
						++chrs;
						chr += generateRandomColor() + "\x1b[5m◉\x1b[0m";
					} 
					while (chrs < GRID_COLUMN_WIDTH) {
						chr += "\x1b[38;2;0;255;0m";
						if (x < (x0 + (x1 - x0) / 2)) chr += "/";
						else if (x > (x0 + (x1 - x0) / 2)) chr += "\\";
						else if (GRID_COLUMN_WIDTH == 2 && chrs == 0) chr += "/";
						else if (GRID_COLUMN_WIDTH == 2 && chrs == 1) chr += "\\";
						else chr += "|";
						++chrs;
					}
				} else {
					chr = std::string(GRID_COLUMN_WIDTH, ' ');
				}
				oss << "\x1b[" << (grid.size() - y + 0) << "A\x1b[" << (0 + (x)*GRID_COLUMN_WIDTH) << "C" << chr << "\x1b[0m\x1b[99999;1H";
			}
		}
		std::cout << oss.str() << std::flush;
	}
}

int32_t main(int argc, char *argv[]) {
	if (argc > 1) PRINTIT = true;
	auto t0 = std::chrono::high_resolution_clock::now();
	int part1 = 1;
	int part2 = 0;
	std::vector<sCoord> arr;
	readFile("data/day14.txt", arr);
	sCoord gridSize(101, 103);

	/// part1
	std::vector robots(arr);
	std::vector<int> q(4, 0);
	int qW = gridSize.x / 2;
	int qH = gridSize.y / 2;
	int seconds = 100;
	for (size_t i = 0; i < robots.size(); ++i) {
		sCoord &robot = robots[i];
		robot.x = (robot.x + robot.vx * seconds) % (gridSize.x);
		robot.y = (robot.y + robot.vy * seconds) % (gridSize.y);
		if (robot.x < 0) robot.x = gridSize.x + robot.x;
		if (robot.y < 0) robot.y = gridSize.y + robot.y;
		if (robot.x < qW && robot.y < qH) ++q[0];
		if (robot.x < qW && robot.y > qH + 0) ++q[1];
		if (robot.x > qW && robot.y < qH) ++q[2];
		if (robot.x > qW && robot.y > qH + 0) ++q[3];
	}
	for (size_t i = 0; i < q.size(); ++i) { part1 *= q[i]; }

	/// part2
	std::vector<std::vector<int8_t>> grid(gridSize.y, std::vector<int8_t>(gridSize.x, 0));
	for (size_t i = 0; i < arr.size(); ++i) { ++grid[arr[i].y][arr[i].x]; }
	seconds = 0;
	while (part2 == 0) {
		++seconds;
		for (size_t i = 0; i < arr.size(); ++i) {
			sCoord &robot = arr[i];
			--grid[robot.y][robot.x];
			robot.x = (robot.x + robot.vx * 1) % (gridSize.x);
			robot.y = (robot.y + robot.vy * 1) % (gridSize.y);
			if (robot.x < 0) robot.x = gridSize.x + robot.x;
			if (robot.y < 0) robot.y = gridSize.y + robot.y;
			++grid[robot.y][robot.x];
		}
		for (size_t x = 0; x < gridSize.x; ++x) {
			int inRow = 0;
			for (size_t y = 0; y < gridSize.y; ++y) {
				if (grid[y][x] > 0) ++inRow;
				else if (inRow > 24) {
					part2 = seconds;
					if (PRINTIT) printTree(grid, x, y, inRow, arr, gridSize);
					goto isDone;
				} else inRow = 0;
			}
		}
	}
isDone:
	std::cout << DEBUG_INFO_S << duration_str(t0) << "  part1=\x1b[1m\x1b[38;2;155;255;15m" << part1 << "\x1b[0m part2=\x1b[1m\x1b[38;2;155;255;15m" << part2 << "\x1b[0m" << std::endl;
	return 0;
}
