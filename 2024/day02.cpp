#include "headerfile.h"
#include <chrono>
#include <cstdio> 
#include <cstdlib> 
#include <fstream>
#include <iostream> 
#include <string>
#include <vector>
#include <cmath>

std::vector<std::vector<int>> readFile(std::string file) {
	std::vector<std::vector<int>> arr;
	std::ifstream inputFile(file);
	if (inputFile.is_open()) {
		std::string line;
		while (std::getline(inputFile, line)) {
			std::vector<int> arrRow;
			while (true) {
				int sep = line.find(" ");
				arrRow.emplace_back(std::stoi(line.substr(0, sep)));
				line = line.substr(sep + 1);
				if (sep == -1) break;
			}
			arr.push_back(arrRow);
		}
		inputFile.close();
	}
	return arr;
}

int checkrow(std::vector<int> rowArr, int skipIndex = -1) {
	int dir = 0;
	bool faulty = false;
	if (skipIndex != -1) { rowArr.erase(rowArr.begin() + skipIndex); }
	for (size_t j = 1; j < rowArr.size(); ++j) {
		int previousNr = rowArr[j - 1];
		int currentNr = rowArr[j];
		if (abs(previousNr - currentNr) > 3 || previousNr == currentNr) faulty = true;
		if (dir != 0) {
			if (dir != 1 && previousNr < currentNr) { faulty = true; }
			if (dir != -1 && previousNr > currentNr) { faulty = true; }
		}
		if (dir == 0) dir = std::signbit(currentNr - previousNr) ? -1 : 1;
		if (faulty) return 1;
	}
	return 0;
}

int main() {
	auto t0 = std::chrono::high_resolution_clock::now();
	std::vector<std::vector<int>> arr = readFile("data/day02.txt");
	int part1 = 0;
	int part2 = 0;
	for (size_t i = 0; i < arr.size(); ++i) {
		if (checkrow(arr[i]) == 0) part1 += 1;

		int rmIndex = -1;
		while (true) {
			if (checkrow(arr[i], rmIndex) == 0) {
				part2++;
				break;
			}
			rmIndex++;
			if (rmIndex >= arr[i].size()) break;
		}
	}
	std::cout << DEBUG_INFO_S << "Duration=\x1b[1m\x1b[38;2;155;255;15m" << std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - t0).count() << "\x1b[0mμs  part1=\x1b[1m\x1b[38;2;155;255;15m" << part1 << "\x1b[0m part2=\x1b[1m\x1b[38;2;155;255;15m" << part2 << "\x1b[0m" << std::endl;
}
