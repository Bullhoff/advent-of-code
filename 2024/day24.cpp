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
#include <set>
#include <list>

#define int long long

using namespace u;

struct sOp {
	std::string w1;
	std::string w2;
	std::string w3;
	std::string op;
	sOp(std::string w1, std::string w2, std::string w3, std::string op) : w1(w1), w2(w2), w3(w3), op(op) {}
};

void readFile(const std::string &filename, std::map<std::string, int> &vals, std::vector<sOp> &ops) {
	std::ifstream file(filename);
	if (!file.is_open()) { throw std::runtime_error("Unable to open file"); }
	std::string line;
	bool atOps = false;
	while (std::getline(file, line)) {
		if (line == "") {
			atOps = true;
			continue;
		}
		if (!atOps) {
			vals[line.substr(0, 3)] = std::stoi(line.substr(4));
		} else {
			std::string w1, w2, w3, op;
			w1 = line.substr(0, 3);
			op = line.substr(4, 3);
			if (op == "AND" || op == "XOR") {
				w2 = line.substr(8, 3);
				w3 = line.substr(15);
			} else {
				w2 = line.substr(7, 3);
				w3 = line.substr(14);
				op = "OR";
			}
			ops.emplace_back(sOp(w1, w2, w3, op));
		}
	}
}

int32_t main() {
	auto t0 = std::chrono::high_resolution_clock::now();
	int part1 = 0;
	std::string part2 = "";
	std::map<std::string, int> vals;
	std::vector<sOp> ops;
	readFile("data/day24.txt", vals, ops);

	auto calc = [](sOp op, std::map<std::string, int> &vals) -> void {
		if (op.op == "AND") { /// w1 && w2
			if (vals[op.w1] == 1 && vals[op.w2] == 1) vals[op.w3] = 1;
			else vals[op.w3] = 0;
		} else if (op.op == "XOR") { /// w1 != w2
			if (vals[op.w1] != vals[op.w2]) vals[op.w3] = 1;
			else vals[op.w3] = 0;
		} else { /// w1==1 || w2==1
			if ((vals[op.w1] + vals[op.w2]) >= 1) vals[op.w3] = 1;
			else vals[op.w3] = 0;
		}
	};
	auto swap = [&ops](const std::vector<int> &sw) -> void {
		for (int i = 0; i < sw.size(); i += 2) {
			std::string w3 = ops[sw[i]].w3;
			ops[sw[i]].w3 = ops[sw[i + 1]].w3;
			ops[sw[i + 1]].w3 = w3;
		}
	};

	/// part1
	auto calcP1 = [&ops, &part1, &calc](std::map<std::string, int> vals) -> void {
		std::list<sOp> queue;
		for (int i = 0; i < ops.size(); ++i) {
			sOp &op = ops[i];
			if (!includes(vals, op.w1) || !includes(vals, op.w2)) {
				queue.emplace_back(op);
				continue;
			}
			calc(op, vals);
			auto it = queue.begin();
			while (it != queue.end()) {
				if (includes(vals, it->w1) && includes(vals, it->w2)) {
					calc(*it, vals);
					it = queue.erase(it);
					it = queue.begin();
				} else {
					it++;
				}
			}
		}
		std::string str = "";
		for (auto it = vals.rbegin(); it != vals.rend(); ++it) {
			if (it->first[0] == 'z') str.append(std::to_string(it->second));
		}
		part1 = std::stoll(str, 0, 2);
	};
	calcP1(vals);

	/// part2
	auto calcP2 = [&ops, &part2, &calc, &swap](std::map<std::string, int> vals, std::vector<int> sw = std::vector<int>()) -> bool {
		swap(sw);
		std::list<sOp> queue;
		for (int i = 0; i < ops.size(); ++i) {
			sOp &op = ops[i];
			if (!includes(vals, op.w1) || !includes(vals, op.w2)) {
				queue.emplace_back(op);
				continue;
			}
			calc(op, vals);
			auto it = queue.begin();
			while (it != queue.end()) {
				if (includes(vals, it->w1) && includes(vals, it->w2)) {
					calc(*it, vals);
					it = queue.erase(it);
					it = queue.begin();
				} else {
					it++;
				}
			}
		}
		std::map<char, std::string> bin{{'x', ""}, {'y', ""}, {'z', ""}};
		for (auto it = vals.rbegin(); it != vals.rend(); ++it) {
			if (it->first[0] == 'x' || it->first[0] == 'y' || it->first[0] == 'z') bin[it->first[0]].append(std::to_string(it->second));
		}
		if ((std::stoll(bin['x'], 0, 2) + std::stoll(bin['y'], 0, 2)) == std::stoll(bin['z'], 0, 2)) return true;
		swap(sw); /// swap back
		return false;
	};

	std::vector<int> sw = (part2 == "7847" || true) ? std::vector<int>{124, 44, 109, 87, 121, 92, 171, 60} : std::vector<int>{};
	for (int i = 0; i < sw.size(); ++i) {
		int t1 = sw[i];
		for (int j = 0; j < sw.size(); ++j) {
			int t2 = sw[j];
			for (int k = 0; k < sw.size(); ++k) {
				int t3 = sw[k];
				for (int l = 0; l < sw.size(); ++l) {
					int t4 = sw[l];
					for (int m = 0; m < sw.size(); ++m) {
						int t5 = sw[m];
						for (int n = 0; n < sw.size(); ++n) {
							int t6 = sw[n];
							for (int o = 0; o < sw.size(); ++o) {
								int t7 = sw[o];
								for (int p = 0; p < sw.size(); ++p) {
									int t8 = sw[p];
									if (std::set<int>({i, j, k, l, m, n, o, p}).size() != 8) continue;
									std::vector<int> sw = {t1, t2, t3, t4, t5, t6, t7, t8};
									if (calcP2(vals, sw)) {
										std::vector<std::string> res;
										for (size_t i = 0; i < sw.size(); ++i) { res.emplace_back(ops[sw[i]].w3); }
										std::sort(res.begin(), res.end(), [](const auto &a, const auto &b) { return a < b; });
										part2 = join(res, ",");
										goto isdone;
									}
								}
							}
						}
					}
				}
			}
		}
	}
isdone : {}

	std::cout << DEBUG_INFO_S << duration_str(t0) << "  part1=\x1b[1m\x1b[38;2;155;255;15m" << part1 << "\x1b[0m part2=\x1b[1m\x1b[38;2;155;255;15m" << part2 << "\x1b[0m" << std::endl;
	return 0;
}
