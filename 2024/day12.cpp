#include "headerfile.h"
#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <unistd.h>
#include <vector>

struct sCoord {
	int x, y;
	sCoord(int x, int y) : x(x), y(y) {}
};

inline bool isInGrid(const sCoord &coord, const std::vector<std::string> &grid) {
	return coord.x >= 0 && coord.y >= 0 && coord.x < grid[0].size() && coord.y < grid.size();
}

void readFile(const std::string file, std::vector<std::string> &arr) {
	std::ifstream inputFile(file);
	if (inputFile.is_open()) {
		std::string line;
		while (std::getline(inputFile, line)) { arr.emplace_back(line); }
		inputFile.close();
	}
}

std::vector<std::vector<int>> flipGrid(const std::vector<std::vector<int>> &grid) {
	size_t rows = grid.size();
	size_t cols = grid[0].size();
	std::vector<std::vector<int>> flipped(cols, std::vector<int>(rows));
	for (size_t y = 0; y < rows; ++y) {
		for (size_t x = 0; x < cols; ++x) { flipped[x][y] = grid[y][x]; }
	}
	return flipped;
}

auto findLines(std::vector<int> &groupLines, const std::vector<std::vector<int>> &groupsMap) -> void {
	for (size_t y = 0; y < groupsMap.size(); ++y) {
		int grpSide1_ = -2;
		int grpSide2_ = -2;
		int prevGrp = -1;
		for (size_t x = 0; x < groupsMap[y].size(); ++x) {
			auto &grp = groupsMap[y][x];
			bool isDiff = (prevGrp == -1) ? true : (prevGrp != grp);
			int grpSide1 = (y == 0) ? -1 : (groupsMap[y - 1][x] != grp) ? -1 : groupsMap[y - 1][x];
			int grpSide2 = (y == groupsMap.size() - 1) ? -1 : (groupsMap[y + 1][x] != grp) ? -1 : groupsMap[y + 1][x];
			if (isDiff) {
				if (grpSide1 != grp) groupLines[grp]++;
				if (grpSide2 != grp) groupLines[grp]++;
			} else {
				if (grpSide1 != grp && (grpSide1_ == -2 || grpSide1_ != grpSide1)) groupLines[grp]++;
				if (grpSide2 != grp && (grpSide2_ == -2 || grpSide2_ != grpSide2)) groupLines[grp]++;
			}
			grpSide1_ = grpSide1;
			grpSide2_ = grpSide2;
			prevGrp = grp;
		}
	}
};

int32_t main() {
	auto t0 = std::chrono::high_resolution_clock::now();
	int part1 = 0;
	int part2 = 0;
	std::vector<std::string> arr;
	readFile("data/day12.txt", arr);
	std::vector<std::string> checkedMap(arr);
	std::vector<std::vector<sCoord>> groups;
	std::vector<std::vector<sCoord>> edges;
	std::vector<sCoord> currGroup;
	std::vector<sCoord> queue;
	for (size_t y = 0; y < arr.size(); ++y) {
		for (size_t x = 0; x < arr[y].size(); ++x) {
			if (checkedMap[y][x] == '.') continue;
			edges.emplace_back(std::vector<sCoord>());
			currGroup.clear();
			queue.clear();
			queue.emplace_back(sCoord(x, y));
			char &chr = arr[y][x];
			while (queue.size() > 0) {
				sCoord next = queue.back();
				queue.pop_back();
				if (arr[next.y][next.x] != chr) { continue; }
				if (checkedMap[next.y][next.x] == '.') continue;
				currGroup.emplace_back(next);
				checkedMap[next.y][next.x] = '.';
				if (!isInGrid(sCoord(next.x - 1, next.y), arr) || arr[next.y][next.x - 1] != chr) edges.back().emplace_back(sCoord(next.x, next.y));
				if (!isInGrid(sCoord(next.x + 1, next.y), arr) || arr[next.y][next.x + 1] != chr) edges.back().emplace_back(sCoord(next.x, next.y));
				if (!isInGrid(sCoord(next.x, next.y - 1), arr) || arr[next.y - 1][next.x] != chr) edges.back().emplace_back(sCoord(next.x, next.y));
				if (!isInGrid(sCoord(next.x, next.y + 1), arr) || arr[next.y + 1][next.x] != chr) edges.back().emplace_back(sCoord(next.x, next.y));
				if (isInGrid(sCoord(next.x - 1, next.y), arr)) queue.emplace_back(sCoord(next.x - 1, next.y));
				if (isInGrid(sCoord(next.x + 1, next.y), arr)) queue.emplace_back(sCoord(next.x + 1, next.y));
				if (isInGrid(sCoord(next.x, next.y - 1), arr)) queue.emplace_back(sCoord(next.x, next.y - 1));
				if (isInGrid(sCoord(next.x, next.y + 1), arr)) queue.emplace_back(sCoord(next.x, next.y + 1));
			}
			groups.emplace_back(currGroup);
		}
	}

	std::vector<std::vector<int>> groupsMap(arr.size(), std::vector<int>(arr[0].size(), -1));
	for (size_t i = 0; i < groups.size(); ++i) {
		for (size_t j = 0; j < groups[i].size(); ++j) {
			sCoord &item = groups[i][j];
			groupsMap[item.y][item.x] = i;
		}
	}

	std::vector<int> groupLines(groups.size(), 0);
	findLines(groupLines, groupsMap);
	groupsMap = flipGrid(groupsMap);
	findLines(groupLines, groupsMap);

	for (size_t i = 0; i < groups.size(); ++i) {
		part1 += groups[i].size() * edges[i].size();
		part2 += groups[i].size() * groupLines[i];
	}

	std::cout << DEBUG_INFO_S << duration_str(t0) << "  part1=\x1b[1m\x1b[38;2;155;255;15m" << part1 << "\x1b[0m part2=\x1b[1m\x1b[38;2;155;255;15m" << part2 << "\x1b[0m" << std::endl;
	return 0;
}
