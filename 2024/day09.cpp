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

struct sItem {
	int64_t id = -1;
	uint value = 0;
	sItem(int value, int id = -1) : value(value), id(id) {}
};

int main() {
	auto t0 = std::chrono::high_resolution_clock::now();
	long part1 = 0;
	long part2 = 0;

	std::vector<sItem> arr;
	std::ifstream inputFile("data/day09.txt");
	if (inputFile.is_open()) {
		std::string line;
		int id = 0;
		while (std::getline(inputFile, line)) {
			for (int x = 0; x < line.size(); ++x) {
				std::string_view line_view(line);
				if ((x & 1) == 0) { 
					arr.emplace_back(line_view[x] - '0', id);
				} else {
					arr.emplace_back(line_view[x] - '0', -1);
					id++;
				}
			}
		}
		inputFile.close();
	}

	/// part1
	std::vector<int> p1Arr;
	for (size_t i = 0; i < arr.size(); ++i) {
		int value = arr[i].value;
		while (value--) { p1Arr.emplace_back(arr[i].id); }
	}
	for (size_t i = 0; i < p1Arr.size(); ++i) {
		int id = p1Arr[i];
		while (id == -1) {
			id = p1Arr.back();
			p1Arr.pop_back();
		}
		p1Arr[i] = id;
		if (part1 + id * 2 < 0) break;
		if (id != -1) part1 += id * i;
	}

	/// part2
	int pos = 0;
	for (auto it = arr.rbegin(); it != arr.rend(); it++) {
		auto item = *it;
		if (item.id != -1) {
			int j = arr.size() - 1 - std::distance(arr.rbegin(), it);
			for (size_t i = 0; i < j; ++i) {
				if (arr[i].id == -1) {
					int diff = arr[i].value - item.value;
					if (diff < 0) continue;
					arr[i].value = arr[j].value;
					arr[i].id = arr[j].id;
					arr[j].id = -1;
					if (diff != 0) { arr.insert(arr.begin() + i + 1, sItem(diff, -1)); }
					break;
				}
			}
		}
	}
	for (const auto &item : arr) {
		if (item.id == -1) {
			pos += item.value;
		} else {
			for (size_t k = 0; k < item.value; ++k) {
				part2 += item.id * pos;
				pos++;
			}
		}
	}

	std::cout << DEBUG_INFO_S << duration_str(t0) << "  part1=\x1b[1m\x1b[38;2;155;255;15m" << part1 << "\x1b[0m part2=\x1b[1m\x1b[38;2;155;255;15m" << part2 << "\x1b[0m" << std::endl;
	return 0;
}
