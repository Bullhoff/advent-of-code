#include "headerfile.h"
#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iterator>
#include <sstream>
#include <iostream>
#include <string>
#include <unistd.h>
#include <cmath>
#include <vector>
#include <unordered_map>

#define map unordered_map
#define int long long int
#define stoi stoll

#define hasEvenNumberCount(num) ((static_cast<int>(log10(num)) + 1) % 2 == 0)

auto follow(std::map<int, std::vector<int>> &paths, const int &num, const int &blink = 0, int count = 0) -> int {
	if (blink < 0) return 0;
	std::vector<int> nums({num});
	nums.reserve(2);
	if (nums[0] == 0) {
		nums[0] = 1;
	} else if (hasEvenNumberCount(nums[0])) {
		std::string str = std::to_string(nums[0]);
		nums[0] = std::stoi(str.substr(0, str.size() / 2));
		nums.emplace_back(std::stoi(str.substr(str.size() / 2)));
	} else {
		nums[0] *= 2024;
	}
	for (size_t i = 0; i < nums.size(); ++i) {
		if (!paths.count(nums[i])) { paths[nums[i]] = std::vector<int>(75, 0); }
		if (paths[nums[i]][blink] == 0) { paths[nums[i]][blink] += follow(paths, nums[i], blink - 1); }
		count += paths[nums[i]][blink];
	}
	return count + nums.size() - 1;
};

int32_t main() {
	auto t0 = std::chrono::high_resolution_clock::now();
	int part1 = 0;
	int part2 = 0;
	std::vector<int> arr;
	std::ifstream inputFile("data/day11.txt");
	if (inputFile) {
		std::string line;
		while (std::getline(inputFile, line)) {
			std::istringstream iss(line);
			std::transform(std::istream_iterator<std::string>(iss), std::istream_iterator<std::string>(), std::back_inserter(arr), [](const std::string &str) { return std::stoi(str); });
		}
	}
	std::map<int, std::vector<int>> paths;
	for (size_t i = 0; i < arr.size(); ++i) { part1 += follow(paths, arr[i], 24, 1); }
	for (size_t i = 0; i < arr.size(); ++i) { part2 += follow(paths, arr[i], 74, 1); }
	std::cout << DEBUG_INFO_S << duration_str(t0) << "  part1=\x1b[1m\x1b[38;2;155;255;15m" << part1 << "\x1b[0m part2=\x1b[1m\x1b[38;2;155;255;15m" << part2 << "\x1b[0m" << std::endl;
	return 0;
}
