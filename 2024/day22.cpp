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
#include <unordered_map>

#define int long long

void readFile(const std::string &filename, std::vector<int> &arr) {
	std::ifstream file(filename);
	if (!file.is_open()) { throw std::runtime_error("Unable to open file"); }
	std::string line;
	while (std::getline(file, line)) { arr.emplace_back(std::stoi(line)); }
}

inline void solve(int &num) {
	num = ((num * 64) ^ num) % 16777216;
	num = ((int)floor(num / 32) ^ num) % 16777216;
	num = ((num * 2048) ^ num) % 16777216;
}

struct IdentityHash {
	size_t operator()(uint16_t x) const { return x; }
};

int32_t main() {
	auto t0 = std::chrono::high_resolution_clock::now();
	int part1 = 0;
	int part2 = 0;
	std::vector<int> arr;
	readFile("data/day22.txt", arr);
	std::vector<std::unordered_map<uint16_t, uint8_t, IdentityHash>> seqMap(arr.size(), std::unordered_map<uint16_t, uint8_t, IdentityHash>());
	for (size_t i = 0; i < arr.size(); ++i) {
		int &num = arr[i];
		std::vector<uint8_t> digitsBuyer;
		for (size_t j = 0; j < 2000; ++j) {
			solve(num);
			digitsBuyer.emplace_back(num % 10);
			if (j >= 4) {
				const uint8_t &d0 = (digitsBuyer.size() < 5) ? 0 : digitsBuyer[digitsBuyer.size() - 5];
				const uint8_t &d1 = digitsBuyer[digitsBuyer.size() - 4];
				const uint8_t &d2 = digitsBuyer[digitsBuyer.size() - 3];
				const uint8_t &d3 = digitsBuyer[digitsBuyer.size() - 2];
				const uint8_t &d4 = digitsBuyer.back();
				const uint8_t diff1 = (digitsBuyer.size() < 5) ? 0 : (d0 - d1 + 9);
				const uint16_t compressed = ((diff1 + 9) << 12) | ((d1 - d2 + 9) << 8) | ((d2 - d3 + 9) << 4) | (d3 - d4 + 9);
				seqMap[i].emplace(compressed, digitsBuyer.back());
			}
		}
		part1 += num;
	}
	for (auto const &[key, val] : seqMap[0]) {
		uint32_t res = val;
		for (size_t i = 1; i < seqMap.size(); ++i) { res += seqMap[i][key]; }
		if (res > part2) part2 = res;
	}

	std::cout << DEBUG_INFO_S << duration_str(t0) << "  part1=\x1b[1m\x1b[38;2;155;255;15m" << part1 << "\x1b[0m part2=\x1b[1m\x1b[38;2;155;255;15m" << part2 << "\x1b[0m" << std::endl;
	return 0;
}
