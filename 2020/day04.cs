#!/usr/local/share/dotnet/dotnet run
using System.Diagnostics;
using System.Text.RegularExpressions;
public class Day04 {
	static int part1 = 0;
	static Int64 part2 = 0;
	static void Main(string[] args) {
		Stopwatch sw = new Stopwatch();
		sw.Start();
		solve();
		sw.Stop();
		Console.WriteLine("\x1b[38;2;255;55;200mday04\x1b[0m {0} \tpart1: \x1b[38;2;255;165;0m{1}\x1b[0m \tpart2: \x1b[38;2;255;165;0m{2}\x1b[0m", sw.Elapsed, part1, part2);
	}
	static void solve() {
		string str = File.ReadAllText("data/day04.txt");
		string[] hairColors = { "amb", "blu", "brn", "gry", "grn", "hzl", "oth" };
		string[] keys = { "byr", "iyr", "eyr", "hgt", "hcl", "ecl", "pid", "cid" };
		foreach (string passportStr in str.Split("\n\n")) {
			int count1 = 0;
			int count2 = 0;
			foreach (string keyvalueStr in passportStr.Replace("\n", " ").Split(" ")) {
				string[] keyvalue = keyvalueStr.Split(":");
				if (keyvalue.Length != 2) continue;
				if (keyvalue[0] != "cid") if (keys.Contains(keyvalue[0])) count1++;
				int? num;
				switch (keyvalue[0]) {
					case "byr":
						num = Int32.Parse(keyvalue[1]);
						if (1920 <= num && num <= 2002) count2++;
						break;
					case "iyr":
						num = Int32.Parse(keyvalue[1]);
						if (2010 <= num && num <= 2020) count2++;
						break;
					case "eyr":
						num = Int32.Parse(keyvalue[1]);
						if (2020 <= num && num <= 2030) count2++;
						break;
					case "hgt":
						if (keyvalue[1].EndsWith("in")) {
							num = Int32.Parse(keyvalue[1].Replace("in", ""));
							if (59 <= num && num <= 76) count2++;
						} else if (keyvalue[1].EndsWith("cm")) {
							num = Int32.Parse(keyvalue[1].Replace("cm", ""));
							if (150 <= num && num <= 193) count2++;
						}
						break;
					case "hcl":
						if (Regex.IsMatch(keyvalue[1], @"^#[0-9a-z]{6}$")) count2++;
						break;
					case "ecl":
						if (hairColors.Contains(keyvalue[1])) count2++;
						break;
					case "pid":
						if (Regex.IsMatch(keyvalue[1], @"^\d{9}$")) count2++;
						break;
					case "cid":
						break;
					default:
						break;
				}
			}
			if (count1 == keys.Length - 1) part1++;
			if (count2 == keys.Length - 1) part2++;
		}
	}
};