using System.Diagnostics;

public class Day07 {
	static int part1 = 0;
	static int part2 = 0;
	static void Main(string[] args) {
		Stopwatch sw = new Stopwatch();
		sw.Start();
		solve();
		sw.Stop();
		Console.WriteLine("\x1b[38;2;255;55;200mday07\x1b[0m {0} \tpart1: \x1b[38;2;255;165;0m{1}\x1b[0m \tpart2: \x1b[38;2;255;165;0m{2}\x1b[0m", sw.Elapsed, part1, part2);
	}

	static Dictionary<string, Dictionary<string, int>> hm = new Dictionary<string, Dictionary<string, int>>();
	static void solve() {
		IEnumerable<string> lines = File.ReadLines("data/day07.txt");
		foreach (string line in lines) {
			string[] kv = line.Split(" bags contain ");
			if (!hm.ContainsKey(kv[0])) hm[kv[0]] = new Dictionary<string, int>();
			if (kv[1] == "no other bags.") continue;
			string[] bags = kv[1].Split(", ");
			bags[bags.Length - 1] = bags[bags.Length - 1].TrimEnd('.');
			foreach (string countbag in bags) {
				int count = Int32.Parse(countbag.Substring(0, 1));
				string bag = countbag.Substring(2);
				int index = bag.IndexOf(" bag");
				if (index >= 0) bag = bag.Substring(0, index);
				hm[kv[0]][bag] = count;
			}
		}
		part1 = getPossibleBagsCount();
		part2 = getBagCount();
		return;
	}

	static int getPossibleBagsCount(string lookingFor = "shiny gold", HashSet<string>? hs = null) {
		hs ??= new HashSet<string>();
		foreach (var pair in hm) {
			if (pair.Value.ContainsKey(lookingFor)) {
				hs.Add(pair.Key);
				getPossibleBagsCount(pair.Key, hs);
			}
		}
		return hs.Count;
	}

	static int getBagCount(string lookingFor = "shiny gold") {
		int res = 0;
		foreach (var pair in hm[lookingFor]) {
			res += pair.Value;
			res += pair.Value * getBagCount(pair.Key);
		}
		return res;
	}

};