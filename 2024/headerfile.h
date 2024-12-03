#pragma once

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
