#include "headerfile.h"
#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <cmath>

std::vector<std::vector<std::vector<int>>> readFile(std::string file) {
	std::vector<std::vector<std::vector<int>>> arr{{}, {}};
	std::vector<std::string> separators{"|", ","};
	int partIndex = 0;
	std::ifstream inputFile(file);
	if (inputFile.is_open()) {
		std::string line;
		while (std::getline(inputFile, line)) {
			if (line == "") {
				partIndex = 1;
				continue;
			}
			int sep = 0;
			std::vector<int> arrRow;
			while (sep != -1) {
				sep = line.find(separators[partIndex]);
				if (sep == -1) {
					arrRow.emplace_back(std::stoi(line));
				} else {
					arrRow.emplace_back(std::stoi(line.substr(0, sep)));
					line = line.substr(sep + 1);
				}
			}
			arr[partIndex].push_back(arrRow);
		}
		inputFile.close();
	}
	return arr;
}

bool is_ok(std::vector<int> &cantOccur, int &nr) {
	for (size_t i = 0; i < cantOccur.size(); ++i) {
		if (nr == cantOccur[i]) { return false; }
	}
	return true;
}

bool check_row(std::vector<int> &arrRow, std::vector<std::vector<int>> &arrRules) {
	std::vector<int> cantOccur;
	for (size_t x = 0; x < arrRow.size(); ++x) {
		int nr = arrRow[x];
		if (!is_ok(cantOccur, nr)) { return false; }
		for (size_t i = 0; i < arrRules.size(); ++i) {
			if (arrRules[i][1] == nr) cantOccur.push_back(arrRules[i][0]);
		}
	}
	return true;
}

int main() {
	auto t0 = std::chrono::high_resolution_clock::now();
	int part1 = 0;
	int part2 = 0;
	std::vector<std::vector<std::vector<int>>> arr = readFile("data/day05.txt");
	std::vector<std::vector<int>> &arrRules = arr[0];
	std::vector<std::vector<int>> &arrUpdates = arr[1];
	for (size_t y = 0; y < arrUpdates.size(); ++y) {
		std::vector<int> &arrUpdatesRow = arrUpdates[y];
		bool ok = check_row(arrUpdatesRow, arrRules);
		if (ok) {
			part1 += arrUpdatesRow[floor((arrUpdatesRow.size()) / 2.0)];
		} else {
			std::sort(arrUpdatesRow.begin(), arrUpdatesRow.end(), [&arrRules](const auto &a, const auto &b) {
				for (size_t i = 0; i < arrRules.size(); ++i) {
					if (arrRules[i][0] == b && arrRules[i][1] == a) return true;
				}
				return false;
			});
			part2 += arrUpdatesRow[floor((arrUpdatesRow.size()) / 2.0)];
		}
	}
	std::cout << DEBUG_INFO_S << duration_str(t0) << "  part1=\x1b[1m\x1b[38;2;155;255;15m" << part1 << "\x1b[0m part2=\x1b[1m\x1b[38;2;155;255;15m" << part2 << "\x1b[0m" << std::endl;
	return 0;
}
