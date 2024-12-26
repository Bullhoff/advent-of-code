#pragma once
#include <cstdlib>
#include <cstring>
#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
// clang-format off
#define RGB(r,g,b)		"\x1b[38;2;" #r ";" #b ";" #g "m"
#define RGBFG(r,g,b)	"\x1b[38;2;" #r ";" #b ";" #g "m"
#define RGBBG(r,g,b)	"\x1b[48;2;" #r ";" #b ";" #g "m"
#define RESET		"\x1b[0m"
#define BLACK		"\x1b[30m"	// Black
#define RED			"\x1b[31m"	// Red
#define GREEN		"\x1b[32m"	// Green
#define YELLOW		"\x1b[33m"	// Yellow
#define BLUE		"\x1b[34m"	// Blue
#define MAGENTA		"\x1b[35m"	// Magenta
#define CYAN		"\x1b[36m"	// Cyan
#define WHITE		"\x1b[37m"	// White
#define BOLD		"\x1b[1m"	// Bold
#define BOLDBLACK	"\x1b[1m\x1b[30m"	// Bold Black
#define BOLDRED		"\x1b[1m\x1b[31m"	// Bold Red
#define BOLDGREEN	"\x1b[1m\x1b[32m"	// Bold Green
#define BOLDYELLOW	"\x1b[1m\x1b[33m"	// Bold Yellow
#define BOLDBLUE	"\x1b[1m\x1b[34m"	// Bold Blue
#define BOLDMAGENTA	"\x1b[1m\x1b[35m"	// Bold Magenta
#define BOLDCYAN	"\x1b[1m\x1b[36m"	// Bold Cyan
#define BOLDWHITE	"\x1b[1m\x1b[37m"	// Bold White
// clang-format on
#define COLORFILE RED
#define COLORLINE BOLDGREEN
#define COLORNUMBER BOLDGREEN
#define COLORFUNC BLUE
#define DEBUG_INFO_S COLORFILE << __FILENAME__ << "\x1b[0m:" << COLORLINE << __LINE__ << "\x1b[0m:" << COLORFUNC << __func__ << "\x1b[0m  "

#define colorbool(b) ((b) == 0 || (b) == false ? "\x1b[31m" : "\x1b[32m") << (b) << "\x1b[0m"

#include <algorithm>
#include <locale>

inline void ltrim(std::string &s) {
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) { return !std::isspace(ch); }));
}
inline void rtrim(std::string &s) {
	s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) { return !std::isspace(ch); }).base(), s.end());
}
inline void trim(std::string &s) {
	rtrim(s);
	ltrim(s);
}

inline bool is_number(const std::string &s) {
	std::string::const_iterator it = s.begin();
	while (it != s.end() && std::isdigit(*it)) ++it;
	return !s.empty() && it == s.end();
}

#include <string>
#include <chrono>
#include <sstream>
inline std::string duration_str(std::chrono::system_clock::time_point t0) {
	using namespace std::chrono;
	system_clock::time_point t1 = system_clock::now();
	std::ostringstream oss;
	oss << "Duration=\x1b[1m\x1b[38;2;155;255;15m";
	auto diff = duration_cast<microseconds>(t1 - t0);
	if (diff < microseconds(5000)) { /// Less than 5000µs
		oss << diff.count() << "\x1b[0mµs";
	} else if (diff < milliseconds(5000)) { /// Less than 5000ms
		oss << duration_cast<milliseconds>(diff).count() << "\x1b[0mms";
	} else {
		oss << duration_cast<duration<float>>(diff).count() << "\x1b[0ms";
	}
	return oss.str();
}

#include <random>
std::string generateRandomColor(bool bg = false) {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(0, 255);
	std::ostringstream oss;
	oss << ((bg) ? "\x1b[48;2;" : "\x1b[38;2;") << dis(gen) << ";" << dis(gen) << ";" << dis(gen) << "m";
	return oss.str();
}
int rand(int min = 0, int max = 1) {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(min, max);
	return dis(gen);
}

#include <vector>
#include <map>
#include <set>
#include <unordered_set>
#include <iostream>
#include <type_traits>

template <typename Container> void print_elements(std::ostream &os, const Container &container) {
	os << "[";
	auto it = container.begin();
	if (it != container.end()) {
		os << COLORNUMBER << *it << RESET;
		++it;
	}
	for (; it != container.end(); ++it) { os << "," << COLORNUMBER << *it << RESET; }
	os << "]";
}
// print vector
template <typename T> std::ostream &operator<<(std::ostream &os, const std::vector<T> &arr) {
	print_elements(os, arr);
	return os;
}
// print set
template <typename T> std::ostream &operator<<(std::ostream &os, const std::set<T> &arr) {
	print_elements(os, arr);
	return os;
}
// print unordered_set
template <typename T> std::ostream &operator<<(std::ostream &os, const std::unordered_set<T> &arr) {
	print_elements(os, arr);
	return os;
}
// print 2d vector
template <typename T> std::ostream &operator<<(std::ostream &os, const std::vector<std::vector<T>> &grid) {
	for (size_t y = 0; y < grid.size(); ++y) {
		for (size_t x = 0; x < grid[y].size(); ++x) { os << grid[y][x]; }
		os << "\n";
	}
	return os;
}

namespace u {
template <typename T> std::string join(const std::vector<T> &arr, const std::string sep = ",") {
	std::stringstream ss;
	for (size_t i = 0; i < arr.size(); ++i) {
		if (i != 0) ss << sep;
		ss << arr[i];
	}
	return ss.str();
}
template <typename T> bool includes(const std::vector<T> &arr, const T &item) {
	return std::find(arr.begin(), arr.end(), item) != arr.end();
}
template <typename T> bool includes(const std::set<T> &arr, const T &item) {
	return arr.find(item) != arr.end();
}
template <typename T1, typename T2> bool includes(const std::map<T1, T2> &arr, const T1 &item) {
	return arr.find(item) != arr.end();
}
} // namespace u

struct sCoordBase {
	int x, y;
	sCoordBase() {}
	sCoordBase(int x, int y) : x(x), y(y) {}
	friend std::ostream &operator<<(std::ostream &os, const sCoordBase &coord) { return os << "[" << COLORNUMBER << coord.x << RESET "," << COLORNUMBER << coord.y << RESET << "]"; }
	sCoordBase set(int x, int y) {
		this->x = x;
		this->y = y;
		return *this;
	};
	sCoordBase rotate(bool r = false) {
		if (!r && y != 0) y = -y; // clockwise
		else if (r && x != 0) x = -x; // counterclockwise
		x ^= y;
		y ^= x;
		x ^= y;
		return *this;
	}
	sCoordBase operator+=(const sCoordBase &other) {
		x += other.x;
		y += other.y;
		return *this;
	}
	sCoordBase operator-=(const sCoordBase &other) {
		x -= other.x;
		y -= other.y;
		return *this;
	}
	sCoordBase operator*=(const sCoordBase &other) {
		x *= other.x;
		y *= other.y;
		return *this;
	}
	bool operator<(const sCoordBase &other) const {
		if (x == other.x) { return y < other.y; }
		return x < other.x;
	}
	bool operator>(const sCoordBase &other) const {
		if (x == other.x) { return y > other.y; }
		return x > other.x;
	}
	bool operator==(const sCoordBase &other) const { return x == other.x && y == other.y; }
	bool operator!=(const sCoordBase &other) const { return x != other.x || y != other.y; }
};
inline sCoordBase operator+(const sCoordBase &a, const sCoordBase &b) {
	return sCoordBase{a.x + b.x, a.y + b.y};
}
inline sCoordBase operator+(const sCoordBase &a, const int &d) {
	return sCoordBase{a.x + d, a.y + d};
}
inline sCoordBase operator*(const sCoordBase &a, const sCoordBase &b) {
	return sCoordBase{a.x * b.x, a.y * b.y};
}
inline sCoordBase operator*(const sCoordBase &a, const int &d) {
	return sCoordBase{a.x * d, a.y * d};
}
inline sCoordBase operator-(const sCoordBase &a, const sCoordBase &b) {
	return sCoordBase{a.x - b.x, a.y - b.y};
}
inline sCoordBase operator-(const sCoordBase &a, const int &d) {
	return sCoordBase{a.x - d, a.y - d};
}
