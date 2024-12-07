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

std::vector<std::vector<std::string>> readFile(std::string file) {
	std::vector<std::vector<std::string>> arr;
	std::ifstream inputFile(file);
	if (inputFile.is_open()) {
		std::string line;
		while (std::getline(inputFile, line)) {
			std::vector<std::string> arrRow;
			int sep = line.find(":");
			arrRow.emplace_back((line.substr(0, sep)));
			line = line.substr(sep + 2);
			while (sep != -1) {
				sep = line.find(" ");
				if (sep == -1) {
					arrRow.emplace_back((line));
				} else {
					arrRow.emplace_back((line.substr(0, sep)));
					line = line.substr(sep + 1);
				}
			}
			arr.push_back(arrRow);
		}
		inputFile.close();
	}
	return arr;
}

void find_unique_combinations(std::vector<std::vector<char>> &resArr, const std::vector<char> &items, int r, std::vector<char> row = std::vector<char>()) {
	if (row.size() >= r) {
		resArr.push_back(row);
		return;
	}
	for (size_t i = 0; i < items.size(); ++i) {
		row.emplace_back(items[i]);
		find_unique_combinations(resArr, items, r, row);
		row.pop_back();
	}
}

int main() {
	auto t0 = std::chrono::high_resolution_clock::now();
	unsigned long long part1 = 0;
	unsigned long long part2 = 0;

	std::map<int, std::vector<std::vector<char>>> uniqueComboMap;

	std::vector<std::vector<std::string>> arr = readFile("data/day07.txt");
	for (size_t y = 0; y < arr.size(); ++y) {
		if (!uniqueComboMap.count(arr[y].size() - 2)) { find_unique_combinations(uniqueComboMap[arr[y].size() - 2], {'+', '*', '|'}, arr[y].size() - 2); }
		std::vector<std::vector<char>> &combos = uniqueComboMap[arr[y].size() - 2];

		std::vector<unsigned long long> longArr;
		for (size_t x = 2; x < arr[y].size(); ++x) { longArr.emplace_back(std::stoll(arr[y][x])); }

		bool foundp1 = false;
		bool foundp2 = false;
		unsigned long long sum = std::stoll(arr[y][0]);
		unsigned long long firstnr = std::stoll(arr[y][1]);
		for (size_t i = 0; i < combos.size(); ++i) {
			bool onlyp2 = false;
			unsigned long long res = firstnr; 
			for (size_t x = 2; x < arr[y].size(); ++x) {
				unsigned long long &nr = longArr[x - 2];
				char &op = combos[i][x - 2];
				if (op == '+') {
					res = res + nr;
				} else if (op == '*') {
					res = res * nr;
				} else if (!foundp2 && op == '|') {
					std::string str = std::to_string(res);
					str.append(arr[y][x]);
					res = std::stoll(str);
					onlyp2 = true;
				}
			}
			if (sum == res) {
				if (!foundp1 && !onlyp2) {
					part1 += res;
					foundp1 = true;
				}
				if (!foundp2) {
					part2 += res;
					foundp2 = true;
				}
			}
			if (foundp1 && foundp2) break;
		}
	}
	std::cout << DEBUG_INFO_S << duration_str(t0) << "  part1=\x1b[1m\x1b[38;2;155;255;15m" << part1 << "\x1b[0m part2=\x1b[1m\x1b[38;2;155;255;15m" << part2 << "\x1b[0m" << std::endl;
	return 0;
}
