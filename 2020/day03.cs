#!/usr/local/share/dotnet/dotnet run
using System.Diagnostics;
public class Day03 {
	static int part1 = 0;
	static Int64 part2 = 0;
	static void Main(string[] args) {
		Stopwatch sw = new Stopwatch();
		sw.Start();
		solve();
		sw.Stop();
		Console.WriteLine("\x1b[38;2;255;55;200mday03\x1b[0m {0} \tpart1: \x1b[38;2;255;165;0m{1}\x1b[0m \tpart2: \x1b[38;2;255;165;0m{2}\x1b[0m", sw.Elapsed, part1, part2);
	}
	static void solve() {
		string[] lines = File.ReadAllText("data/day03.txt").Split('\n');
		part1 = checkSlope(lines, 3, 1);
		part2 = 1;
		part2 *= checkSlope(lines, 1, 1);
		part2 *= part1;
		part2 *= checkSlope(lines, 5, 1);
		part2 *= checkSlope(lines, 7, 1);
		part2 *= checkSlope(lines, 1, 2);
		return;
	}
	static int checkSlope(string[] lines, int stepX = 3, int stepY = 1) {
		int trees = 0;
		int x = 0;
		for (int y = stepY; y < lines.Length - 1; y += stepY) {
			x += stepX;
			if (x > lines[y].Length - 1) x = x % lines[y].Length;
			if (lines[y][x] == '#') trees++;
		}
		return trees;
	}
};