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
#include <unordered_map>
#include <unordered_set>

struct sCoord {
	int x, y;
	sCoord(int x, int y) : x(x), y(y) {}
	bool operator<(const sCoord &other) const {
		if (x == other.x) { return y < other.y; }
		return x < other.x;
	}
	bool operator==(const sCoord &other) const { return x == other.x && y == other.y; }
};
inline sCoord operator+(const sCoord &a, const sCoord &b) {
	return sCoord{a.x + b.x, a.y + b.y};
}
inline sCoord operator*(const sCoord &a, int d) {
	return sCoord{a.x * d, a.y * d};
}
namespace std {
template <> struct hash<sCoord> {
	size_t operator()(const sCoord &coord) const { return hash<int>()(coord.x) ^ (hash<int>()(coord.y) << 1); }
};
} // namespace std

inline bool isInGrid(const sCoord &coord, const sCoord &gridSize) {
	return coord.x >= 0 && coord.y >= 0 && coord.x < gridSize.x && coord.y < gridSize.y;
}

inline void follow(const sCoord &gridSize, std::unordered_set<sCoord> &signals, const sCoord &fromCoord, const sCoord &diff) {
	sCoord current = fromCoord;
	while (isInGrid(current, gridSize)) {
		signals.insert(current);
		current = current + diff;
	}
}

int main() {
	auto t0 = std::chrono::high_resolution_clock::now();
	std::unordered_set<sCoord> signals;
	std::unordered_map<char, std::vector<sCoord>> frequencies;

	int x, y = 0;
	std::ifstream inputFile("data/day08.txt");
	if (inputFile.is_open()) {
		std::string line;
		while (std::getline(inputFile, line)) {
			for (x = 0; x < line.size(); ++x) {
				if (line[x] != '.') {
					frequencies[line[x]].emplace_back(x, y);
				}
			}
			y++;
		}
		inputFile.close();
	}
	sCoord gridSize(x, y);
	signals.reserve(x * y);

	/// part1
	for (auto const &[key, val] : frequencies) {
		for (size_t i = 0; i < val.size(); ++i) {
			for (size_t j = 0; j < val.size(); ++j) {
				if (i == j) continue;
				sCoord diff(val[j].x - val[i].x, val[j].y - val[i].y);
				sCoord signalCoord1(val[i].x + diff.x * -1, val[i].y - diff.y);
				if (isInGrid(signalCoord1, gridSize) && signals.find(signalCoord1) == signals.end()) { signals.insert(signalCoord1); }
				sCoord signalCoord2(val[j].x + diff.x * 1, val[j].y + diff.y);
				if (isInGrid(signalCoord2, gridSize) && signals.find(signalCoord2) == signals.end()) { signals.insert(signalCoord2); }
			}
		}
	}
	int part1 = signals.size();

	/// part2
	signals.clear();
	for (auto const &[key, val] : frequencies) {
		for (size_t i = 0; i < val.size(); ++i) {
			for (size_t j = 0; j < val.size(); ++j) {
				if (i == j) continue;
				sCoord diff(val[j].x - val[i].x, val[j].y - val[i].y);
				follow(gridSize, signals, val[i], diff * -1);
				follow(gridSize, signals, val[j], diff);
			}
		}
	}
	int part2 = signals.size();

	std::cout << DEBUG_INFO_S << duration_str(t0) << "  part1=\x1b[1m\x1b[38;2;155;255;15m" << part1 << "\x1b[0m part2=\x1b[1m\x1b[38;2;155;255;15m" << part2 << "\x1b[0m" << std::endl;
	return 0;
}
