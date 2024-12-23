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
#include <set>
#include <unordered_map>

#define map unordered_map

void readFile(const std::string &filename, std::map<std::string, std::vector<std::string>> &arr) {
	std::ifstream file(filename);
	if (!file.is_open()) { throw std::runtime_error("Unable to open file"); }
	std::string line;
	while (std::getline(file, line)) {
		std::string pc1 = line.substr(0, 2);
		std::string pc2 = line.substr(3);
		arr[pc1].emplace_back(pc2);
		arr[pc2].emplace_back(pc1);
	}
}

template <typename T> std::string join(const std::vector<T> &arr, const std::string &sep = ",") {
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

int32_t main() {
	auto t0 = std::chrono::high_resolution_clock::now();
	std::map<std::string, std::vector<std::string>> arr;
	readFile("data/day23.txt", arr);

	/// part1
	std::set<std::set<std::string>> sets;
	for (auto const &[pc1, val] : arr) {
		for (size_t i = 0; i < val.size(); ++i) {
			std::string &pc2 = arr[pc1][i];
			for (size_t j = 0; j < arr[pc2].size(); ++j) {
				std::string &pc3 = arr[pc2][j];
				if ((pc1[0] == 't' || pc2[0] == 't' || pc3[0] == 't') && includes(arr[pc1], pc3)) { sets.insert({pc1, pc2, pc3}); }
			}
		}
	}
	int part1 = sets.size();

	/// part2
	auto get_connections = [&arr](auto &&self, std::string k1, std::string k2) -> std::vector<std::string> {
		std::vector<std::string> resArr({k1, k2});
		for (size_t i = 0; i < arr[k2].size(); ++i) {
			std::string &k3 = arr[k2][i];
			if (includes(arr[k3], k1)) {
				bool ok = true;
				for (size_t j = 0; j < resArr.size(); ++j) {
					if (!includes(arr[resArr[j]], k3)) {
						ok = false;
						break;
					}
				}
				if (ok) resArr.emplace_back(k3);
			}
		}
		return resArr;
	};
	std::vector<std::string> largestSet;
	for (auto const &[k1, val] : arr) {
		std::vector<std::string> theset;
		for (size_t i = 0; i < val.size(); ++i) {
			std::string &k2 = arr[k1][i];
			std::vector<std::string> resArr = get_connections(get_connections, k1, k2);
			if (resArr.size() > theset.size()) theset = resArr;
		}
		if (largestSet.size() < theset.size()) largestSet = theset;
	}
	std::sort(largestSet.begin(), largestSet.end(), [](const std::string &a, const std::string &b) { return a < b; });
	std::string part2 = join(largestSet);

	std::cout << DEBUG_INFO_S << duration_str(t0) << "  part1=\x1b[1m\x1b[38;2;155;255;15m" << part1 << "\x1b[0m part2=\x1b[1m\x1b[38;2;155;255;15m" << part2 << "\x1b[0m" << std::endl;
	return 0;
}
