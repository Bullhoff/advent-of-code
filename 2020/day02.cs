#!/usr/local/share/dotnet/dotnet run
using System.Diagnostics;
using System.Text.RegularExpressions;
public class Day02 {
	static int part1 = 0, part2 = 0;
	static void Main(string[] args) {
		Stopwatch sw = new Stopwatch();
		sw.Start();
		solve();
		sw.Stop();
		Console.WriteLine("\x1b[38;2;255;55;200mday02\x1b[0m {0} \tpart1: \x1b[38;2;255;165;0m{1}\x1b[0m \tpart2: \x1b[38;2;255;165;0m{2}\x1b[0m", sw.Elapsed, part1, part2);
	}
	static void solve() {
		var lines = File.ReadLines("data/day02.txt");
		List<int> numbers = new List<int>();
		foreach (var line in lines) {
			string[] splitted = Regex.Split(line.Trim(), @":\s|\s+|-");
			int count = 0;
			foreach (char c in splitted[3]) {
				if (c == splitted[2][0]) count++;
			}
			int n1 = Int32.Parse(splitted[0]);
			int n2 = Int32.Parse(splitted[1]);
			if (n1 <= count && count <= n2) part1++;
			count = 0;
			if (splitted[3][n1 - 1] == splitted[2][0]) count++;
			if (splitted[3][n2 - 1] == splitted[2][0]) count++;
			if (count == 1) part2++;
		}
		return;
	}
};