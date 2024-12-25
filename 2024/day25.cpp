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

void readFile(const std::string &filename, std::vector<std::vector<std::string>> &arr) {
	std::ifstream file(filename);
	if (!file.is_open()) { throw std::runtime_error("Unable to open file"); }
	std::string line;
	arr.emplace_back(std::vector<std::string>());
	while (std::getline(file, line)) {
		if (line == "") {
			arr.emplace_back(std::vector<std::string>());
			continue;
		}
		arr.back().emplace_back(line);
	}
}

int32_t main() {
	auto t0 = std::chrono::high_resolution_clock::now();
	int part1 = 0;
	std::string part2 = "\u2605";
	std::vector<std::vector<std::string>> arr;
	readFile("data/day25.txt", arr);
	std::vector<std::vector<int>> keys;
	std::vector<std::vector<int>> locks;
	for (size_t i = 0; i < arr.size(); ++i) {
		auto theset = *std::next(arr.begin(), i);
		std::vector<std::vector<int>> &theArr = (theset[0] == "#####") ? locks : keys;
		theArr.emplace_back(std::vector<int>(5, 0));
		for (size_t y = 0; y < theset.size(); ++y) {
			for (size_t x = 0; x < theset[y].size(); ++x) {
				if (theset[y][x] == '#') ++theArr.back()[x];
			}
		}
	}
	for (size_t i = 0; i < keys.size(); ++i) {
		auto key = *std::next(keys.begin(), i);
		for (size_t j = 0; j < locks.size(); ++j) {
			auto lock = *std::next(locks.begin(), j);
			bool match = true;
			for (size_t y = 0; y < key.size(); ++y) {
				if ((key[y] + lock[y]) > 7) {
					match = false;
					break;
				};
			}
			if (match) ++part1;
		}
	}
	std::cout << DEBUG_INFO_S << duration_str(t0) << "  part1=\x1b[1m\x1b[38;2;155;255;15m" << part1 << "\x1b[0m part2=\x1b[1m\x1b[38;2;155;255;15m" << part2 << "\x1b[0m" << std::endl;
	return 0;
}
