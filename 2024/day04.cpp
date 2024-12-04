#include "headerfile.h"
#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <cmath>

std::vector<std::string> readFile(std::string file) {
	std::vector<std::string> arr;
	std::ifstream inputFile(file);
	if (inputFile.is_open()) {
		std::string line;
		while (std::getline(inputFile, line)) {
			arr.push_back(line);
		}
		inputFile.close();
	}
	return arr;
}

std::string get_word(std::vector<std::string> &arr, std::vector<std::pair<int, int>> cords) {
	std::string str = "";
	for (size_t i = 0; i < cords.size(); ++i) { str.append(std::string{arr[cords[i].second][cords[i].first]}); }
	return str;
}

int get_xmas_matches_part1(std::vector<std::string> &arr, size_t &x, size_t &y) {
	int matches = 0;
	if (std::string{arr[y][x]} != "X") return matches;
	if (y >= 3 && get_word(arr, std::vector<std::pair<int, int>>{{x, y}, {x, y - 1}, {x, y - 2}, {x, y - 3}}) == "XMAS") { matches += 1; }
	if (y <= arr.size() - 1 - 3 && get_word(arr, std::vector<std::pair<int, int>>{{x, y}, {x, y + 1}, {x, y + 2}, {x, y + 3}}) == "XMAS") { matches += 1; }
	if (x >= 3 && get_word(arr, std::vector<std::pair<int, int>>{{x, y}, {x - 1, y}, {x - 2, y}, {x - 3, y}}) == "XMAS") { matches += 1; }
	if (x <= arr[0].size() - 1 - 3 && get_word(arr, std::vector<std::pair<int, int>>{{x, y}, {x + 1, y}, {x + 2, y}, {x + 3, y}}) == "XMAS") { matches += 1; }
	if (y >= 3 && x >= 3 && get_word(arr, std::vector<std::pair<int, int>>{{x, y}, {x - 1, y - 1}, {x - 2, y - 2}, {x - 3, y - 3}}) == "XMAS") { matches += 1; }
	if (y <= arr.size() - 1 - 3 && x >= 3 && get_word(arr, std::vector<std::pair<int, int>>{{x, y}, {x - 1, y + 1}, {x - 2, y + 2}, {x - 3, y + 3}}) == "XMAS") { matches += 1; }
	if (y <= arr.size() - 1 - 3 && x <= arr[0].size() - 1 - 3 && get_word(arr, std::vector<std::pair<int, int>>{{x, y}, {x + 1, y + 1}, {x + 2, y + 2}, {x + 3, y + 3}}) == "XMAS") { matches += 1; }
	if (y >= 3 && x <= arr[0].size() - 1 - 3 && get_word(arr, std::vector<std::pair<int, int>>{{x, y}, {x + 1, y - 1}, {x + 2, y - 2}, {x + 3, y - 3}}) == "XMAS") { matches += 1; }
	return matches;
}

int get_xmas_matches_part2(std::vector<std::string> &arr, size_t &x, size_t &y) {
	if (std::string{arr[y][x]} != "A") return 0;
	if (y < 1 || x < 1 || y > arr.size() - 1 - 1 || x > arr[0].size() - 1 - 1) return 0;
	int matches = 0;
	if (get_word(arr, std::vector<std::pair<int, int>>{{x - 1, y - 1}, {x, y}, {x + 1, y + 1}}) == "MAS") matches += 1;
	if (get_word(arr, std::vector<std::pair<int, int>>{{x - 1, y + 1}, {x, y}, {x + 1, y - 1}}) == "MAS") matches += 1;
	if (matches < 2 && get_word(arr, std::vector<std::pair<int, int>>{{x + 1, y - 1}, {x, y}, {x - 1, y + 1}}) == "MAS") matches += 1;
	if (matches > 0 && matches < 2 && get_word(arr, std::vector<std::pair<int, int>>{{x + 1, y + 1}, {x, y}, {x - 1, y - 1}}) == "MAS") matches += 1;
	if (matches < 2) return 0;
	return 1;
}

int main() {
	auto t0 = std::chrono::high_resolution_clock::now();
	int part1 = 0;
	int part2 = 0;
	std::vector<std::string> arr = readFile("data/day04.txt");
	for (size_t y = 0; y < arr.size(); ++y) {
		for (size_t x = 0; x < arr[y].size(); ++x) {
			part1 += get_xmas_matches_part1(arr, x, y);
			part2 += get_xmas_matches_part2(arr, x, y);
		}
	}
	std::cout << DEBUG_INFO_S << "Duration=\x1b[1m\x1b[38;2;155;255;15m" << std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - t0).count() << "\x1b[0mμs  part1=\x1b[1m\x1b[38;2;155;255;15m" << part1 << "\x1b[0m part2=\x1b[1m\x1b[38;2;155;255;15m" << part2 << "\x1b[0m" << std::endl;
	return 0;
}
