#include "headerfile.h"
#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <cmath>

void is_enabled(bool &enabled, std::string str) {
	while (true) {
		if (!enabled) {
			int sep = str.find("do()");
			if (sep == -1) { break; }
			str = str.substr(sep + 1);
			enabled = true;
		} else {
			int sep = str.find("don't()");
			if (sep == -1) { break; }
			str = str.substr(sep + 1);
			enabled = false;
		}
	}
}

int main() {
	auto t0 = std::chrono::high_resolution_clock::now();
	int part1 = 0;
	int part2 = 0;
	bool enabled = true;

	std::ifstream inputFile("data/day03.txt");
	if (inputFile.is_open()) {
		std::string line;
		while (std::getline(inputFile, line)) {
			while (true) {
				int sepStart = line.find("mul(");
				if (sepStart == -1) break;
				std::string partstr = line.substr(sepStart + 4);

				int sepEnd = partstr.find(")");
				if (sepEnd == -1) break;
				partstr = partstr.substr(0, sepEnd);

				int sepComma = partstr.find(",");
				std::string str1 = partstr.substr(0, sepComma);
				std::string str2 = partstr.substr(sepComma + 1);

				is_enabled(enabled, line.substr(0, sepStart));

				if (str1.size() < 4 && str2.size() < 4 && is_number(str1) && is_number(str2)) {
					int product = std::stoi(str1) * std::stoi(str2);
					part1 += product;
					if (enabled) part2 += product;
					line = line.substr(sepStart + 4 + sepEnd + 1);
				} else {
					line = line.substr(sepStart + 4);
				}
				if (sepEnd == -1) break;
			}
		}
		inputFile.close();
	}
	std::cout << DEBUG_INFO_S << "Duration=\x1b[1m\x1b[38;2;155;255;15m" << std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - t0).count() << "\x1b[0mμs  part1=\x1b[1m\x1b[38;2;155;255;15m" << part1 << "\x1b[0m part2=\x1b[1m\x1b[38;2;155;255;15m" << part2 << "\x1b[0m" << std::endl;
	return 0;
}
