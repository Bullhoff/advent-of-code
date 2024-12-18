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

#define int long long

void split(std::vector<int> &arr, std::string str, const std::string &delimiter) {
	size_t pos = 0;
	while ((pos = str.find(delimiter)) != std::string::npos) {
		arr.emplace_back(std::stoi(str.substr(0, pos)));
		str.erase(0, pos + delimiter.size());
	}
	arr.emplace_back(std::stoi(str));
}

std::string join(const std::vector<int> &arr, const std::string &sep = ",") {
	std::stringstream ss;
	for (size_t i = 0; i < arr.size(); ++i) {
		if (i != 0) ss << sep;
		ss << arr[i];
	}
	return ss.str();
}

void readFile(const std::string &filename, std::vector<int> &Program, int &A, int &B, int &C) {
	std::ifstream file(filename);
	if (!file.is_open()) { throw std::runtime_error("Unable to open file"); }
	std::string line;
	while (std::getline(file, line)) {
		if (line.size() == 0) continue;
		if (line[0] == 'R') {
			switch (line[9]) {
			case 'A':
				A = std::stoi(line.substr(12));
				break;
			case 'B':
				B = std::stoi(line.substr(12));
				break;
			case 'C':
				C = std::stoi(line.substr(12));
				break;
			}
		}
		if (line[0] == 'P') { split(Program, line.substr(9), ","); }
	}
}

enum Instructions { adv, bxl, bst, jnz, bxc, out, bdv, cdv };

std::vector<int> solve(const std::vector<int> &program, int A, int B, int C) {
	std::vector<int> output;
	int combo_operand;
	for (size_t i = 0; i < program.size(); i += 2) {
		auto &opcode = program[i];
		auto &literal_operand = program[i + 1];
		combo_operand = literal_operand;

		switch (literal_operand) {
		case 4:
			combo_operand = A;
			break;
		case 5:
			combo_operand = B;
			break;
		case 6:
			combo_operand = C;
			break;
		case 7:
			break;
		}

		switch (opcode) {
		case adv:
			A = floor(A / (pow(2, combo_operand)));
			break;
		case bxl:
			B ^= literal_operand;
			break;
		case bst:
			B = combo_operand % 8;
			break;
		case jnz:
			if (A != 0) { i = literal_operand - 2; }
			break;
		case bxc:
			B ^= C;
			break;
		case out:
			output.emplace_back(combo_operand % 8);
			break;
		case bdv:
			B = floor(A / (pow(2, combo_operand)));
			break;
		case cdv:
			C = floor(A / (pow(2, combo_operand)));
			break;
		}
	}
	return output;
}

int32_t main(int32_t argc, char *argv[]) {
	auto t0 = std::chrono::high_resolution_clock::now();
	int A, B, C;
	std::vector<int> program;
	readFile("data/day17.txt", program, A, B, C);

	/// part1
	std::string part1 = join(solve(program, A, B, C));

	/// part2
	int part2 = 0;
	std::vector<int> output;
	output.reserve(program.size());
	int maxMatches = 0;
	int matches = 0;
	A = 0;
	while (true) {
		output = solve(program, A, B, C);
		matches = 0;
		if (program.size() == output.size()) {
			for (size_t i = 0; i < program.size(); ++i) {
				int &inst1 = program[program.size() - 1 - i];
				int &inst2 = output[program.size() - 1 - i];
				if (inst1 != inst2) break;
				matches++;
			}
		}
		if (matches > maxMatches || true) maxMatches = matches; /// "|| true" because some datasets get too many matches early
		if (program.size() < output.size()) break;
		int diff = program.size() - maxMatches;
		if (diff == 0) {
			part2 = A;
			break;
		} else if (program.size() - output.size()) {
			A += pow(8, (program.size() - output.size()));
		} else {
			if (diff > 2) A += 1 + pow(8, (diff - 2));
			else A += 1;
		}
	}

	std::cout << DEBUG_INFO_S << duration_str(t0) << "  part1=\x1b[1m\x1b[38;2;155;255;15m" << part1 << "\x1b[0m part2=\x1b[1m\x1b[38;2;155;255;15m" << part2 << "\x1b[0m" << std::endl;
	return 0;
}
