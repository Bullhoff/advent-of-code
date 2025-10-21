#!/usr/local/share/dotnet/dotnet run
using System.Diagnostics;
public class Day01 {
	static int part1 = 0, part2 = 0;
	static void Main(string[] args) {
		Stopwatch sw = new Stopwatch();
		sw.Start();
		solve();
		sw.Stop();
		Console.WriteLine("\x1b[38;2;255;55;200mday01\x1b[0m {0} \tpart1: \x1b[38;2;255;165;0m{1}\x1b[0m \tpart2: \x1b[38;2;255;165;0m{2}\x1b[0m", sw.Elapsed, part1, part2);
	}
	static void solve() {
		var lines = File.ReadLines("data/day01.txt");
		List<int> numbers = new List<int>();
		foreach (var line in lines) {
			Int32.TryParse(line, out int num);
			numbers.Add(num);
		}
		for (int i = 0; i < numbers.Count; i++) {
			for (int j = 0; j < numbers.Count; j++) {
				if (i == j) continue;
				if (numbers[i] + numbers[j] == 2020) {
					part1 = numbers[i] * numbers[j];
					goto donepart1;
				}
			}
		}
	donepart1:
		for (int i = 0; i < numbers.Count; i++) {
			for (int j = 0; j < numbers.Count; j++) {
				for (int k = 0; k < numbers.Count; k++) {
					if (i == j || i == k || j == k) continue;
					if (numbers[i] + numbers[j] + numbers[k] == 2020) {
						part2 = numbers[i] * numbers[j] * numbers[k];
						goto donepart2;
					}
				}
			}
		}
	donepart2:
		return;
	}
};