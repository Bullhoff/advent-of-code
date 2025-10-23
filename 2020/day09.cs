using System.Diagnostics;

public class Day09 {
	static Int64 part1 = 0;
	static Int64 part2 = 0;
	static void Main(string[] args) {
		Stopwatch sw = new Stopwatch();
		sw.Start();
		solve();
		sw.Stop();
		Console.WriteLine("\x1b[38;2;255;55;200mday09\x1b[0m {0} \tpart1: \x1b[38;2;255;165;0m{1}\x1b[0m \tpart2: \x1b[38;2;255;165;0m{2}\x1b[0m", sw.Elapsed, part1, part2);
	}

	static List<Int64> numbers = new List<Int64>();
	static void solve() {
		IEnumerable<string> lines = File.ReadLines("data/day09.txt");
		foreach (string line in lines) {
			numbers.Add(Int64.Parse(line));
		}
		part1 = solvePart1();
		part2 = solvePart2();
		return;
	}

	static Int64 solvePart1(int preamble = 25) {
		for (int i = preamble; i < numbers.Count; i++) {
			bool hasMatch = false;
			for (int j = i - preamble; j < i; j++) {
				for (int k = i - preamble; k < i; k++) {
					if ((numbers[j] + numbers[k]) == numbers[i]) {
						hasMatch = true;
						break;
					}
				}
				if (hasMatch) break;
			}
			if (!hasMatch) return numbers[i];
		}
		return 0;
	}

	static Int64 solvePart2() {
		for (int i = 0; i < numbers.Count; i++) {
			Int64 sum = 0;
			Int64 lowest = -1;
			Int64 highest = -1;
			for (int j = i; j < numbers.Count; j++) {
				Int64 num = numbers[j];
				if (num < lowest || lowest == -1) lowest = num;
				if (num > highest || highest == -1) highest = num;
				sum += num;
				if (sum > part1) break;
				if (sum == part1) return lowest + highest;
			}
		}
		return 0;
	}

};