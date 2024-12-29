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
char getDir();

bool PRINTIT = false;
int getMaxVisibleRows();
#define DELAYTEXT 500
#define DELAY 500
#define COLOREMPTY "\x1b[38;2;0;0;0m"
#define COLORBOX "\x1b[38;2;50;250;50m"
#define COLORWALL "\x1b[38;2;255;255;255m"
#define COLORPOS "\x1b[38;2;50;255;5m\x1b[48;2;155;55;55m"
#define COLOREMPTY_START ""
#define COLORBOX_START ""
#define COLORWALL_START COLORWALL
#define COLORPOS_START COLORPOS

struct sCoord : sCoordBase {
	using sCoordBase::sCoordBase;
};

template <typename T> inline bool isInGrid(const T &coord, const std::vector<std::string> &grid) {
	static_assert(std::is_base_of<sCoordBase, T>::value, "T must be derived from sCoordBase");
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
	std::ostringstream oss;
	for (size_t y = 0; y < arr.size(); ++y) {
		for (size_t x = 0; x < arr[y].size(); ++x) {
			const char &chr = arr[y][x];
			if (chr == '[' || chr == ']' || chr == 'O') oss << COLORBOX_START;
			else if (chr == '<' || chr == 'v' || chr == '^' || chr == '>' || chr == '@') oss << COLORPOS_START;
			else if (chr == '#') oss << COLORWALL_START;
			else if (chr == '.') oss << COLOREMPTY_START;
			oss << chr << "\x1b[0m";
		}
		oss << std::endl;
	}
	std::cout << oss.str();
}

void updateGrid(std::vector<std::string> &arr, const int x, const int y, const char chr) {
	arr[y][x] = chr;
	if (PRINTIT) {
		std::ostringstream oss;
		oss << "\x1B[" << y + 1 << "H"; /// abs row
		oss << "\x1B[" << x << "C"; /// col
		if (chr == '<' || chr == 'v' || chr == '^' || chr == '>' || chr == '@') oss << COLORPOS;
		else if (chr == '#') oss << COLORWALL;
		else if (chr == '.') oss << COLOREMPTY;
		else if (chr == '[' || chr == ']' || chr == 'O') oss << COLORBOX;
		oss << chr;
		oss << "\x1b[0m";
		std::cout << oss.str() << std::endl;
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

template <typename T1, typename T2> bool moveP2(std::vector<std::string> &arr, const T1 &dir, T2 pos) {
	static_assert(std::is_base_of<sCoordBase, T1>::value, "T1 must be derived from sCoordBase");
	static_assert(std::is_base_of<sCoordBase, T2>::value, "T2 must be derived from sCoordBase");
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
				if (arr[pos.y][pos.x] == ']' && !moveP2(arr, dir, pos + dir * 1)) { return false; }
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
		int visibleRows = getMaxVisibleRows();
		if (visibleRows < arr.size()) {
			std::cout << "Requires \x1b[7m" << arr.size() << "\x1b[0m rows to print. Currently theres only \x1b[7m" << visibleRows << "\x1b[0m visible rows. Make the terminal bigger or zoom out." << std::endl;
			PRINTIT = false;
			CONTROLIT = false;
		}
	}
	if (PRINTIT) {
		std::ios_base::sync_with_stdio(false);
		std::cin.tie(NULL);
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

	// clang-format off
	std::map<char, std::vector<std::pair<int, int>>> chars = {
		{'0', {{1,0}, {2,0}, {3,1}, {3,2}, {3,3}, {3,4}, {3,5}, {3,6}, {2,7}, {1,7}, {0,6}, {0,5}, {0,4}, {0,3}, {0,2}, {0,1}}},
		{'1', {{1,0}, {2,0}, {2,1}, {2,2}, {2,3}, {2,4}, {2,5}, {2,6}, {2,7}, {1,7}, {3,7}}},
		{'2', {{0,1}, {1,0}, {2,0}, {3,1}, {3,2}, {3,3}, {2,4}, {1,5}, {0,6}, {0,7}, {1,7}, {2,7}, {3,7}}},
		{'3', {{0,1}, {1,0}, {2,0}, {3,1}, {3,2}, {3,3}, {2,3}, {3,4}, {3,5}, {3,6}, {2,7}, {1,7}, {0,6}}},
		{'4', {{3,0}, {3,1}, {3,2}, {3,3}, {3,4}, {3,5}, {3,6}, {3,7}, {0,1},  {0,2}, {0,3}, {0,4}, {1,4}, {2,4}}},
		{'5', {{0,0}, {1,0}, {2,0}, {3,0}, {0,1}, {0,2}, {0,3}, {1,3}, {2,3}, {3,4}, {3,5}, {3,6}, {2,7}, {1,7}, {0,6}}},
		{'6', {{2,0}, {1,0}, {0,1}, {0,2}, {0,3}, {0,4}, {0,5}, {0,6}, {1,7}, {2,7}, {3,6}, {3,5}, {3,4}, {2,3}, {1,3}}},
		{'7', {{0,0}, {1,0}, {2,0}, {3,0}, {3,1}, {3,2}, {2,3}, {2,4}, {1,5}, {1,6}, {1,7}}},
		{'8', {{1,0}, {2,0}, {3,1}, {3,2}, {2,3}, {1,3}, {0,4}, {0,5}, {0,6}, {1,7}, {2,7}, {3,6}, {3,5}, {3,4}, {0,1}, {0,2}}},
		{'9', {{1,0}, {2,0}, {3,1}, {3,2}, {3,3}, {2,4}, {1,4}, {0,3}, {0,2}, {0,1}, {3,4}, {3,5}, {3,6}, {2,7}, {1,7}, {0,6}}}
	};
	// clang-format on

	auto setText = [&arrP2, &chars](std::string str, std::string style = "") -> void {
		std::ostringstream oss;
		const int fs = 8;
		const int yPos = (arrP2.size() / 2) - (fs / 2);
		const int xPos = (arrP2[0].size() / 2) - (fs / 2 * str.size());
		for (size_t j = 0; j < str.size(); ++j) {
			const char chr = str[j];
			for (size_t i = 0; i < chars[chr].size(); ++i) {
				auto p = chars[chr][i];
				int x_ = fs * j + xPos + p.first + 0;
				int y_ = yPos + p.second;
				oss << "\x1B[" << y_ << ";" << x_ << "H";
				oss << style;
				oss << arrP2[y_ - 1][x_ - 1] << "\x1b[0m";
			}
		}
		std::cout << oss.str() << std::endl;
	};

	std::string prevStr = "";
	for (size_t y = 0; y < arrP2.size(); ++y) {
		for (size_t x = 0; x < arrP2[y].size(); ++x) {
			if (arrP2[y][x] == '[') {
				part2 += 100 * y + x;
				if (PRINTIT) {
					std::string str = std::to_string(part2);
					setText(prevStr, "");
					setText(str, "\x1b[7m");
					prevStr = str;
					std::this_thread::sleep_for(std::chrono::microseconds(DELAYTEXT));
				}
			}
		}
	}
	if (PRINTIT) std::cout << "\x1B[" << arrP2.size() << "H" << std::endl; /// move cursor to the bottom

	std::cout << DEBUG_INFO_S << duration_str(t0) << "  part1=\x1b[1m\x1b[38;2;155;255;15m" << part1 << "\x1b[0m part2=\x1b[1m\x1b[38;2;155;255;15m" << part2 << "\x1b[0m" << std::endl;
	return 0;
}

#ifdef _WIN32
#include <windows.h>
int getMaxVisibleRows() {
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi)) { return csbi.srWindow.Bottom - csbi.srWindow.Top + 1; }
	return -1; 
}
#else
#include <sys/ioctl.h>
#include <unistd.h>
int getMaxVisibleRows() {
	struct winsize w;
	if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) != -1) { return w.ws_row; }
	return -1; 
}
#endif

#ifdef _WIN32
#include <windows.h>
int getch(void) {
	HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
	INPUT_RECORD irInputRecord;
	DWORD dwEventsRead;
	char cChar;
	while (ReadConsoleInputA(hStdin, &irInputRecord, 1, &dwEventsRead)) {
		if (irInputRecord.EventType == KEY_EVENT && irInputRecord.Event.KeyEvent.bKeyDown) {
			cChar = irInputRecord.Event.KeyEvent.uChar.AsciiChar;
			FlushConsoleInputBuffer(hStdin);
			return cChar;
		}
	}
	return 0;
}
#else
#include <termios.h>
#include <unistd.h>
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
#endif

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
		case 66:
			return 'v';
		case 67:
			return '>';
		case 68:
			return '<';
		case 113:
			return 'q';
		}
	}
	return 'q';
}
