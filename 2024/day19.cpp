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

void split(std::vector<std::string> &arr, std::string str, const std::string &delimiter) {
	size_t pos = 0;
	while ((pos = str.find(delimiter)) != std::string::npos) {
		arr.emplace_back(str.substr(0, pos));
		str.erase(0, pos + delimiter.size());
	}
	arr.emplace_back(str);
}

void readFile(const std::string &filename, std::vector<std::string> &patterns, std::vector<std::string> &designs) {
	std::ifstream file(filename);
	if (!file.is_open()) { throw std::runtime_error("Unable to open file"); }
	std::string line;
	while (std::getline(file, line)) {
		if (line == "") continue;
		if (patterns.size() == 0) split(patterns, line, ", ");
		else designs.emplace_back(line);
	}
}

inline long long solve(const std::vector<std::string> &patterns, const std::string &design, std::vector<long long> &mem, int index = 0) {
	long long res = 0;
	if (mem[index] != 0) return mem[index];
	for (size_t j = 0; j < patterns.size(); ++j) {
		const std::string &pattern = patterns[j];
		if (design.size() < index + pattern.size()) { continue; }
		if (design[index] != pattern[0]) continue;
		for (size_t k = 0; k < pattern.size(); ++k) {
			if (design[index + k] != pattern[k]) goto continueloop;
		}
		if (design.size() - index == pattern.size()) {
			++res;
		} else {
			res += solve(patterns, design, mem, index + pattern.size());
			mem[index] = res;
		}
	continueloop : {}
	}
	return res;
}

int32_t main() {
	auto t0 = std::chrono::high_resolution_clock::now();
	int part1 = 0;
	long long part2 = 0;
	std::vector<std::string> patterns;
	std::vector<std::string> designs;
	readFile("data/day19.txt", patterns, designs);
	std::sort(patterns.begin(), patterns.end(), [](const auto &a, const auto &b) { return a.size() > b.size(); });
	for (size_t i = 0; i < designs.size(); ++i) {
		const std::string &design = designs[i];
		std::vector<long long> mem(design.size(), 0);
		long long res = solve(patterns, design, mem);
		if (res > 0) ++part1;
		part2 += res;
	}
	std::cout << DEBUG_INFO_S << duration_str(t0) << "  part1=\x1b[1m\x1b[38;2;155;255;15m" << part1 << "\x1b[0m part2=\x1b[1m\x1b[38;2;155;255;15m" << part2 << "\x1b[0m" << std::endl;
	return 0;
}
