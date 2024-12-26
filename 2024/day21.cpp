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
#include <map>

struct sCoord : sCoordBase {
	sCoord() : sCoordBase() {}
	sCoord(int x, int y) : sCoordBase(x, y) {}
};

void readFile(const std::string &filename, std::vector<std::string> &arr) {
	std::ifstream file(filename);
	if (!file.is_open()) { throw std::runtime_error("Unable to open file"); }
	std::string line;
	while (std::getline(file, line)) { arr.emplace_back(line); }
}

struct sMem {
	uint64_t cost;
	std::string pos = "";
	sMem() {}
	sMem(uint64_t cost) : cost(cost) {}
};

struct sStuff {
	std::map<char, sCoord> kpNumMap;
	std::map<sCoord, char> kpNumMap2;
	std::map<char, sCoord> kpMap;
	std::map<sCoord, char> kpMap2;
	sStuff() {
		std::vector<std::vector<char>> keypadNumeric = {
			{'7', '8', '9'}, 
			{'4', '5', '6'}, 
			{'1', '2', '3'}, 
			{'#', '0', 'A'}
		};
		std::vector<std::vector<char>> keypadDir = {
			{'#', '^', 'A'}, 
			{'<', 'v', '>'}
		};
		for (size_t y = 0; y < keypadDir.size(); ++y) {
			for (size_t x = 0; x < keypadDir[y].size(); ++x) {
				kpMap[keypadDir[y][x]] = sCoord(x, y);
				kpMap2[sCoord(x, y)] = keypadDir[y][x];
			}
		}
		for (size_t y = 0; y < keypadNumeric.size(); ++y) {
			for (size_t x = 0; x < keypadNumeric[y].size(); ++x) {
				kpNumMap[keypadNumeric[y][x]] = sCoord(x, y);
				kpNumMap2[sCoord(x, y)] = keypadNumeric[y][x];
			}
		}
	}
} stuff;

uint64_t solve(const std::string code, const int robots, std::vector<std::map<std::string, sMem>> &mem) {
	if (mem.size() == 0) mem = std::vector<std::map<std::string, sMem>>(1 + robots);
	std::vector<char> positions(1 + robots, 'A');
	auto get_cost = [&mem, &positions](auto &&self, const int index = 0, const char chr) -> uint64_t {
		uint64_t cost = 0;
		auto &kpm = (index == 0) ? stuff.kpNumMap : stuff.kpMap;
		auto &kpm2 = (index == 0) ? stuff.kpNumMap2 : stuff.kpMap2;
		sCoord fromPos = kpm[positions[index]];
		sCoord toPos = kpm[chr];
		auto diff = toPos - fromPos;
		char chr2;
		bool prioVertical = true;
		int dir = 0;
		while (true) {
			int dX = diff.x > 0 ? 1 : diff.x < 0 ? -1 : 0;
			int dY = diff.y > 0 ? 1 : diff.y < 0 ? -1 : 0;
			if (dir != 0) {
				if (dir == 1 && dY != 0) prioVertical = true;
				else if (dir == 2 && dX != 0) prioVertical = false;
			} else {
				if (diff.y > 0) { prioVertical = true; }
				if (diff.x < 0) { prioVertical = false; }
				if (positions[index] == 'v') prioVertical = true;
				else if (positions[index] == '>') prioVertical = false;
				else if (positions[index] == '^') prioVertical = true;
				else if (positions[index] == 'A' && chr == '<') prioVertical = true;
				else if (positions[index] == 'A' && chr == 'v') prioVertical = false;
				if (kpm['#'] == sCoord(fromPos.x + diff.x, fromPos.y)) prioVertical = true;
				if (kpm['#'] == sCoord(fromPos.x, fromPos.y + diff.y)) prioVertical = false;
			}
			if (chr == positions[index]) {
				if (index != positions.size() - 1) {
					std::string key2{positions[index + 1], 'A'};
					if (!u::includes(mem[index + 1], key2)) {
						mem[index + 1][key2] = sMem(self(self, index + 1, 'A'));
					} else {
						for (size_t i = index + 1; i < positions.size(); ++i) { positions[i] = 'A'; }
					}
					cost += mem[index + 1][key2].cost;
				}
				if (index == positions.size() - 1) ++cost;
				break;
			} else {
				if (diff.x == 0 || (diff.y != 0 && prioVertical)) {
					fromPos = sCoord(fromPos.x, fromPos.y + dY);
					chr2 = (dY > 0) ? 'v' : '^';
					dir = 1;
					diff.y -= dY;
				} else {
					fromPos = sCoord(fromPos.x + dX, fromPos.y);
					chr2 = (dX > 0) ? '>' : '<';
					dir = 2;
					diff.x -= dX;
				}
				positions[index] = kpm2[fromPos];
				if (index != positions.size() - 1) {
					std::string key2{positions[index + 1], chr2};
					if (!u::includes(mem[index + 1], key2)) {
						mem[index + 1][key2] = sMem(self(self, index + 1, chr2));
						mem[index + 1][key2].pos = std::string(positions.begin() + index + 1, positions.end());

					} else {
						for (size_t i = 0; i < mem[index + 1][key2].pos.size(); ++i) { positions[index + i + 1] = mem[index + 1][key2].pos[i]; }
					}
					cost += mem[index + 1][key2].cost;
				}
			}
		}
		return cost;
	};
	uint64_t totalCost = 0;
	for (size_t i = 0; i < code[i]; ++i) {
		const char &chr = code[i];
		totalCost += get_cost(get_cost, 0, chr);
	}
	return totalCost;
}

int32_t main() {
	auto t0 = std::chrono::high_resolution_clock::now();
	int part1 = 0;
	uint64_t part2 = 0;
	std::vector<std::string> arr;
	readFile("data/day21.txt", arr);
	std::vector<std::map<std::string, sMem>> mem1;
	std::vector<std::map<std::string, sMem>> mem2;
	for (size_t i = 0; i < arr.size(); ++i) {
		const std::string &code = arr[i];
		const int nr = std::stoi(code.substr(0, 3));
		part1 += nr * solve(code, 3, mem1);
		part2 += nr * solve(code, 26, mem2);
	}
	std::cout << DEBUG_INFO_S << duration_str(t0) << "  part1=\x1b[1m\x1b[38;2;155;255;15m" << part1 << "\x1b[0m part2=\x1b[1m\x1b[38;2;155;255;15m" << part2 << "\x1b[0m" << std::endl;
	return 0;
}
