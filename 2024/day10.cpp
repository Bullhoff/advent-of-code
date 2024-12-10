#include "headerfile.h"
#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <unistd.h>
#include <cmath>
#include <string_view>
#include <vector>

struct sCoord {
	int x, y;
	sCoord(int x, int y) : x(x), y(y) {}
};

#define xytype sCoord
#define xy(x, y) x, y
typedef std::vector<std::vector<int>> tGrid;

inline bool isInGrid(const sCoord &coord, const sCoord &gridSize) {
	return coord.x >= 0 && coord.y >= 0 && coord.x < gridSize.x && coord.y < gridSize.y;
}
inline bool isInGrid(const sCoord &coord, const tGrid &grid) {
	return coord.x >= 0 && coord.y >= 0 && coord.x < grid[0].size() && coord.y < grid.size();
}

int getScore(const tGrid &grid, tGrid &scoreArr, xytype pos, int fromHeight = -1) {
	if (grid[pos.y][pos.x] - fromHeight != 1) return 0;
	if (scoreArr[pos.y][pos.x] >= 0) return scoreArr[pos.y][pos.x];
	int score = 0;
	if (grid[pos.y][pos.x] == 9) score++;
	if (isInGrid(xytype(pos.x - 1, pos.y), grid)) score += getScore(grid, scoreArr, xytype(pos.x - 1, pos.y), fromHeight + 1);
	if (isInGrid(xytype(pos.x + 1, pos.y), grid)) score += getScore(grid, scoreArr, xytype(pos.x + 1, pos.y), fromHeight + 1);
	if (isInGrid(xytype(pos.x, pos.y - 1), grid)) score += getScore(grid, scoreArr, xytype(pos.x, pos.y - 1), fromHeight + 1);
	if (isInGrid(xytype(pos.x, pos.y + 1), grid)) score += getScore(grid, scoreArr, xytype(pos.x, pos.y + 1), fromHeight + 1);
	scoreArr[pos.y][pos.x] = score;
	return score;
}

int main() {
	auto t0 = std::chrono::high_resolution_clock::now();
	int part1 = 0;
	int part2 = 0;
	std::vector<xytype> startPoints;
	std::vector<xytype> endPoints;
	tGrid arr;

	std::ifstream inputFile("data/day10.txt");
	if (inputFile.is_open()) {
		std::string line;
		while (std::getline(inputFile, line)) {
			std::vector<int> arrRow;
			std::string_view line_view(line);
			for (int x = 0; x < line_view.size(); ++x) {
				arrRow.emplace_back(line_view[x] - '0');
				if (arrRow.back() == 9) { endPoints.emplace_back(xy(x, arr.size())); }
				if (arrRow.back() == 0) { startPoints.emplace_back(xy(x, arr.size())); }
			}
			arr.emplace_back(arrRow);
		}
		inputFile.close();
	}
	tGrid scoreArrEmpty(arr.size(), std::vector(arr[0].size(), -1));

	for (size_t i = 0; i < startPoints.size(); ++i) {
		tGrid scoreArr(scoreArrEmpty);
		getScore(arr, scoreArr, startPoints[i]);

		/// part1
		for (size_t j = 0; j < endPoints.size(); ++j) {
			if (scoreArr[endPoints[j].y][endPoints[j].x] > -1) { part1++; }
		}

		/// part2
		for (size_t j = 0; j < startPoints.size(); ++j) {
			if (scoreArr[startPoints[j].y][startPoints[j].x] > -1) { part2 += scoreArr[startPoints[j].y][startPoints[j].x]; }
		}
	}

	std::cout << DEBUG_INFO_S << duration_str(t0) << "  part1=\x1b[1m\x1b[38;2;155;255;15m" << part1 << "\x1b[0m part2=\x1b[1m\x1b[38;2;155;255;15m" << part2 << "\x1b[0m" << std::endl;
	return 0;
}
