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
#include <thread>

bool CONTROLIT = false;
#include <termios.h>
char getDir();

bool PRINTIT = false;
#define DELAY 500
#define COLOREMPTY "\x1b[38;2;0;0;0m"
#define COLORBOX "\x1b[38;2;50;250;50m"
#define COLORWALL "\x1b[38;2;255;255;255m"
#define COLORPOS "\x1b[38;2;50;255;5m\x1b[48;2;155;55;55m"
#define COLOREMPTY_START ""
#define COLORBOX_START ""
#define COLORWALL_START ""
#define COLORPOS_START COLORPOS

struct sCoord {
	int x, y;
	sCoord() {}
	sCoord(int x, int y) : x(x), y(y) {}
	void set(int x, int y) {
		this->x = x;
		this->y = y;
	};
	sCoord operator+=(const sCoord &other) {
		x += other.x;
		y += other.y;
		return *this;
	}
	sCoord operator-=(const sCoord &other) {
		x -= other.x;
		y -= other.y;
		return *this;
	}
};
inline sCoord operator+(const sCoord &a, const sCoord &b) {
	return sCoord{a.x + b.x, a.y + b.y};
}
inline sCoord operator*(const sCoord &a, int d) {
	return sCoord{a.x * d, a.y * d};
}

inline bool isInGrid(const sCoord &coord, const std::vector<std::string> &grid) {
	return coord.x >= 0 && coord.y >= 0 && coord.x < grid[0].size() && coord.y < grid.size();
}

void replaceAll(std::string &str, const std::string &fromStr, const std::string &toStr) {
	size_t pos = str.find(fromStr);
	while (pos != std::string::npos) {
		str.replace(pos, 1, toStr);
		pos = str.find(fromStr, pos + 2);
	}
}

void readFile(const std::string &file, std::vector<std::string> &arr, std::vector<std::string> &arr2, std::string &moves) {
	std::ifstream inputFile(file);
	if (inputFile.is_open()) {
		std::string line;
		while (std::getline(inputFile, line)) {
			if (line == "") { continue; }
			if (line[0] == '#') {
				arr.emplace_back(line);
				std::replace(line.begin(), line.end(), 'x', 'y');
				replaceAll(line, "#", "##");
				replaceAll(line, ".", "..");
				replaceAll(line, "@", "@.");
				replaceAll(line, "O", "[]");
				arr2.emplace_back(line);
			} else {
				moves.append(line);
			}
		}
		inputFile.close();
	}
}

void printGrid(const std::vector<std::string> &arr) {
	for (size_t y = 0; y < arr.size(); ++y) {
		for (size_t x = 0; x < arr[y].size(); ++x) {
			char chr = arr[y][x];
			if (chr == '[' || chr == ']' || chr == 'O') std::cout << COLORBOX_START;
			if (chr == '<' || chr == 'v' || chr == '^' || chr == '>' || chr == '@') std::cout << COLORPOS_START;
			if (chr == '#') std::cout << COLORWALL_START;
			if (chr == '.') std::cout << COLOREMPTY_START;
			if (chr == '[' || chr == ']' || chr == 'O') std::cout << COLORBOX_START;
			std::cout << chr << "\x1b[0m";
		}
		std::cout << std::endl;
	}
}

void updateGrid(std::vector<std::string> &arr, const int x, const int y, const char chr) {
	arr[y][x] = chr;
	if (PRINTIT) {
		std::cout << "\033[" << y + 1 << "H"; /// up
		std::cout << "\033[" << x << "C"; /// right
		if (chr == '<' || chr == 'v' || chr == '^' || chr == '>' || chr == '@') std::cout << COLORPOS;
		if (chr == '#') std::cout << COLORWALL;
		if (chr == '.') std::cout << COLOREMPTY;
		if (chr == '[' || chr == ']' || chr == 'O') std::cout << COLORBOX;
		std::cout << chr;
		std::cout << "\x1b[0m";
		std::cout << std::endl;
	}
}

bool move(std::vector<std::string> &arr, const sCoord &dir, sCoord pos) {
	pos += dir;
	if (arr[pos.y][pos.x] == '#') { return false; }
	if (arr[pos.y][pos.x] == 'O' && !move(arr, dir, pos)) { return false; }
	arr[pos.y - dir.y][pos.x - dir.x] = '.';
	arr[pos.y][pos.x] = 'O';
	return true;
}

bool moveP2(std::vector<std::string> &arr, const sCoord &dir, sCoord pos) {
	char chr = arr[pos.y][pos.x];
	if (dir.y != 0 && chr == ']') {
		pos.x -= 1;
		chr = arr[pos.y][pos.x];
	}
	if (dir.x != 0 && chr == ']') {
		pos.x -= 1;
		chr = arr[pos.y][pos.x];
	}
	pos += dir;
	if (!isInGrid(pos, arr)) return false;
	if (arr[pos.y][pos.x] == '#') { return false; }
	if (dir.x != 0) {
		if (chr == '[') {
			if (dir.x > 0) {
				if (arr[pos.y][pos.x + dir.x] == '#') return false;
				if (arr[pos.y][pos.x + dir.x] == '[' && !moveP2(arr, dir, pos + dir * 2)) { return false; }
			} else {
				if (arr[pos.y][pos.x] == '#') return false;
				if (arr[pos.y][pos.x + dir.x] == '[' && !moveP2(arr, dir, pos + dir * 0)) { return false; }
			}
		}
	}
	if (dir.y != 0) {
		if (chr == '[' && (arr[pos.y][pos.x] == '#' || arr[pos.y][pos.x + 1] == '#')) { return false; }
		if (chr == '[' && arr[pos.y][pos.x + 1] == '[') {
			if (!moveP2(arr, dir, sCoord(pos.x + 1, pos.y))) { return false; }
		}
		if (arr[pos.y][pos.x] == '[') {
			if (!moveP2(arr, dir, sCoord(pos.x, pos.y))) { return false; }
		}
		if (arr[pos.y][pos.x] == ']') {
			if (!moveP2(arr, dir, sCoord(pos.x - 1, pos.y))) { return false; }
		}
	}
	if (dir.x != 0) {
		if (chr == '[') {
			if (dir.x > 0) {
				updateGrid(arr, (pos.x), (pos.y), '[');
				updateGrid(arr, (pos.x + dir.x), (pos.y), ']');
			}
			if (dir.x < 0) {
				updateGrid(arr, (pos.x), (pos.y), '[');
				updateGrid(arr, (pos.x - dir.x), (pos.y), ']');
			}
		}
	}
	if (dir.y != 0) {
		if (chr == '[') {
			updateGrid(arr, (pos.x), (pos.y - dir.y), '.');
			updateGrid(arr, (pos.x + 1), (pos.y - dir.y), '.');
			updateGrid(arr, (pos.x), (pos.y), '[');
			updateGrid(arr, (pos.x + 1), (pos.y), ']');
		}
	}
	return true;
}

int32_t main(int argc, char *argv[]) {
	if (argc > 1) PRINTIT = true;
	if (argc > 1 && argv[1][0] == 'c') CONTROLIT = true;
	auto t0 = std::chrono::high_resolution_clock::now();
	int part1 = 0;
	int part2 = 0;
	std::vector<std::string> arr;
	std::vector<std::string> arrP2;
	std::string moves;
	readFile("data/day15.txt", arr, arrP2, moves);
	sCoord gridSize(arr[0].size(), arr.size());
	sCoord startPos;
	for (size_t y = 0; y < arr.size(); ++y) {
		for (size_t x = 0; x < arr[y].size(); ++x) {
			if (arr[y][x] == '@') { startPos.set(x, y); }
		}
	}
	sCoord pos(startPos);
	sCoord dir;

	/// part1
	for (size_t i = 0; i < moves.size(); ++i) {
		char chr = moves[i];
		switch (chr) {
		case '<':
			dir.set(-1, 0);
			break;
		case '>':
			dir.set(1, 0);
			break;
		case '^':
			dir.set(0, -1);
			break;
		case 'v':
			dir.set(0, 1);
			break;
		default:
			break;
		}
		pos += dir;
		if (arr[pos.y][pos.x] == '#') {
			pos -= dir;
			continue;
		}
		if (arr[pos.y][pos.x] == 'O' && !move(arr, dir, pos)) { pos -= dir; }
	}
	for (size_t y = 0; y < arr.size(); ++y) {
		for (size_t x = 0; x < arr[y].size(); ++x) {
			if (arr[y][x] == 'O') { part1 += 100 * y + x; }
		}
	}

	/// part2
	if (PRINTIT) {
		system("clear");
		printGrid(arrP2);
	}
	for (size_t y = 0; y < arrP2.size(); ++y) {
		for (size_t x = 0; x < arrP2[y].size(); ++x) {
			if (arrP2[y][x] == '@') { startPos.set(x, y); }
		}
	}
	pos = startPos;
	auto loopP2 = [&arrP2, &pos, &dir](char chr) -> bool {
		switch (chr) {
		case '<':
			dir.set(-1, 0);
			break;
		case '>':
			dir.set(1, 0);
			break;
		case '^':
			dir.set(0, -1);
			break;
		case 'v':
			dir.set(0, 1);
			break;
		default:
			return false;
			break;
		}
		pos += dir;
		if (arrP2[pos.y][pos.x] == '#') {
			pos -= dir;
			return true;
		}
		std::vector acopy(arrP2);
		if ((arrP2[pos.y][pos.x] == 'O' || arrP2[pos.y][pos.x] == '[' || arrP2[pos.y][pos.x] == ']') && !moveP2(arrP2, dir, pos)) {
			pos -= dir;
			arrP2 = acopy;
		} else {
			updateGrid(arrP2, (pos.x), (pos.y), chr);
			updateGrid(arrP2, (pos.x - dir.x), (pos.y - dir.y), '.');
		}
		if (PRINTIT) std::this_thread::sleep_for(std::chrono::microseconds(DELAY));
		return true;
	};
	if (CONTROLIT) {
		while (true) {
			if (!loopP2(getDir())) break;
		}
	} else {
		for (size_t i = 0; i < moves.size(); ++i) {
			loopP2(moves[i]); 
		}
	}
	for (size_t y = 0; y < arrP2.size(); ++y) {
		for (size_t x = 0; x < arrP2[y].size(); ++x) {
			if (arrP2[y][x] == '[') { part2 += 100 * y + x; }
		}
	}
	if (PRINTIT) std::cout << "\033[" << arrP2.size() << "H" << std::endl;

	std::cout << DEBUG_INFO_S << duration_str(t0) << "  part1=\x1b[1m\x1b[38;2;155;255;15m" << part1 << "\x1b[0m part2=\x1b[1m\x1b[38;2;155;255;15m" << part2 << "\x1b[0m" << std::endl;
	return 0;
}

int getch(void) {
	int ch;
	struct termios oldt;
	struct termios newt;
	tcgetattr(STDIN_FILENO, &oldt); 
	newt = oldt; 
	newt.c_lflag &= ~(ICANON | ECHO); 
	tcsetattr(STDIN_FILENO, TCSANOW, &newt); 
	ch = getchar(); 
	tcsetattr(STDIN_FILENO, TCSANOW, &oldt); 
	return ch; 
}
char getDir() {
	int x = ' ', y = ' ', z = ' ';
	x = getch();
	if (x == 27) {
		y = getch();
		z = getch();
	}
	if (x == 27 && y == 91) {
		switch (z) {
		case 65:
			return '^';
			break;
		case 66:
			return 'v';
			break;
		case 67:
			return '>';
			break;
		case 68:
			return '<';
			break;
		case 113:
			return 'q';
		}
	}
	return 'q';
}
