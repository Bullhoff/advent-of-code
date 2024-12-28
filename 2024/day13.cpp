#include "headerfile.h"
#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <unistd.h>
#include <cmath>

#define int long long

struct sCoord {
	int x, y;
	bool operator==(const sCoord &other) const { return x == other.x && y == other.y; }
};

auto startsWith(const std::string &haystack, const std::string &needle) -> bool {
	if (needle.size() > haystack.size()) return false;
	if (haystack.substr(0, needle.size()) == needle) return true;
	return false;
};

auto getXY(const std::string &str, sCoord &coord) -> void {
	coord.x = std::stoi(str.substr(str.find("X") + 2, str.find(",") - str.find("X") - 2));
	coord.y = std::stoi(str.substr(str.find("Y") + 2));
};

uint64_t do_math(sCoord A, sCoord B, sCoord Prize) {
	int64_t determinant = A.x * B.y - B.x * A.y;
	if (determinant == 0) return 0; 
	int64_t stepsA = (Prize.x * B.y - Prize.y * B.x) / determinant;
	int64_t stepsB = (Prize.y * A.x - Prize.x * A.y) / determinant;
	if (stepsA < 0 || stepsB < 0 || (Prize.x * B.y - Prize.y * B.x) % determinant != 0 || (Prize.y * A.x - Prize.x * A.y) % determinant != 0) return 0; 
	if (A.x * stepsA + B.x * stepsB != Prize.x || A.y * stepsA + B.y * stepsB != Prize.y) return 0; 
	return stepsA * 3 + stepsB * 1;
}

int32_t main() {
	auto t0 = std::chrono::high_resolution_clock::now();
	long long part1 = 0;
	long long part2 = 0;
	std::ifstream inputFile("data/day13.txt");
	if (inputFile.is_open()) {
		sCoord A, B, Prize;
		std::string line;
		while (std::getline(inputFile, line)) {
			if (startsWith(line, "Button A")) getXY(line, A);
			else if (startsWith(line, "Button B")) getXY(line, B);
			else if (startsWith(line, "Prize")) {
				getXY(line, Prize);
				part1 += do_math(A, B, Prize);
				Prize.x += 10000000000000;
				Prize.y += 10000000000000;
				part2 += do_math(A, B, Prize);
			}
		}
		inputFile.close();
	}
	std::cout << DEBUG_INFO_S << duration_str(t0) << "  part1=\x1b[1m\x1b[38;2;155;255;15m" << part1 << "\x1b[0m part2=\x1b[1m\x1b[38;2;155;255;15m" << part2 << "\x1b[0m" << std::endl;
	return 0;
}
