#include "headerfile.h"
#include <chrono>
#include <algorithm>
#include <cstdio> 
#include <cstdlib> 
#include <fstream>
#include <iostream> 
#include <map>
#include <string>
#include <vector>

std::vector<std::vector<int>> readFile(std::string file) {
	std::vector<int> arr1;
	std::vector<int> arr2;
	std::ifstream inputFile(file);
	if (inputFile.is_open()) {
		std::string line;
		while (std::getline(inputFile, line)) {
			int sep = line.find(" ");
			std::string left = line.substr(0, sep);
			std::string right = line.substr(sep + 1);
			trim(right);
			arr1.emplace_back(std::stoi(left));
			arr2.emplace_back(std::stoi(right));
		}
		inputFile.close();
	}
	return {arr1, arr2};
}

int main() {
	auto t0 = std::chrono::high_resolution_clock::now();
	std::vector<std::vector<int>> arr = readFile("data/day01.txt");
	std::sort(arr[0].begin(), arr[0].end(), [](const auto &a, const auto &b) { return a < b; });
	std::sort(arr[1].begin(), arr[1].end(), [](const auto &a, const auto &b) { return a < b; });
	int part1 = 0;
	int part2 = 0;
	std::map<int, int> occurrences;
	for (size_t i = 0; i < arr[0].size(); ++i) {
		part1 += abs(arr[0][i] - arr[1][i]);

		if (!occurrences.count(arr[1][i])) { occurrences[arr[1][i]] = 0; }
		occurrences[arr[1][i]] += 1;
	}
	for (size_t i = 0; i < arr[0].size(); ++i) { part2 += abs(arr[0][i] * occurrences[arr[0][i]]); }

	std::cout << DEBUG_INFO_S << "Duration=\x1b[1m\x1b[38;2;155;255;15m" << std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - t0).count() << "\x1b[0mμs  part1=\x1b[1m\x1b[38;2;155;255;15m" << part1 << "\x1b[0m part2=\x1b[1m\x1b[38;2;155;255;15m" << part2 << "\x1b[0m" << std::endl;
}
