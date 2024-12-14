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
#include <unordered_set>

#define set unordered_set

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
			else std::cout << "\x1b[38;2;0;255;0mX\x1b[0m";
			std::cout << " ";
		}
		std::cout << std::endl;
	}
}

int32_t main(int argc, char *argv[]) {
	auto t0 = std::chrono::high_resolution_clock::now();
	int part1 = 1;
	int part2 = 0;
	std::vector<sCoord> arr;
	readFile("data/day14.txt", arr);
	sCoord gridSize(101, 103);

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
	seconds = 0;
	int prevX = 0;
	std::set<int> row;
	while (part2 == 0) {
		++seconds;
		for (size_t i = 0; i < arr.size(); ++i) {
			sCoord &robot = arr[i];
			robot.x = (robot.x + robot.vx * 1) % (gridSize.x);
			robot.y = (robot.y + robot.vy * 1) % (gridSize.y);
			if (robot.x < 0) robot.x = gridSize.x + robot.x;
			if (robot.y < 0) robot.y = gridSize.y + robot.y;
		}
		std::sort(arr.begin(), arr.end(), [](const auto &a, const auto &b) { return a.x < b.x; });
		for (size_t i = 0; i < arr.size(); ++i) {
			sCoord &robot = arr[i];
			if (robot.x == 0 || robot.x == gridSize.x - 1) continue;
			if (robot.x != prevX) { row.clear(); }
			row.insert(robot.y);
			if (row.size() > 35) {
				if (argc > 1) printGrid(gridSize, arr);
				part2 = seconds;
				break;
			}
			prevX = robot.x;
		}
	}

	std::cout << DEBUG_INFO_S << duration_str(t0) << "  part1=\x1b[1m\x1b[38;2;155;255;15m" << part1 << "\x1b[0m part2=\x1b[1m\x1b[38;2;155;255;15m" << part2 << "\x1b[0m" << std::endl;
	return 0;
}
