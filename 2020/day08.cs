using System.Diagnostics;

public class Day08 {
	static int part1 = 0;
	static int part2 = 0;
	static void Main(string[] args) {
		Stopwatch sw = new Stopwatch();
		sw.Start();
		solve();
		sw.Stop();
		Console.WriteLine("\x1b[38;2;255;55;200mday08\x1b[0m {0} \tpart1: \x1b[38;2;255;165;0m{1}\x1b[0m \tpart2: \x1b[38;2;255;165;0m{2}\x1b[0m", sw.Elapsed, part1, part2);
	}

	static List<string> listKeys = new List<string>();
	static List<int> listValues = new List<int>();
	static void solve() {
		string[] lines = File.ReadAllText("data/day08.txt").TrimEnd('\r', '\n').Split("\n");
		for (int i = 0; i < lines.Length; i++) {
			string line = lines[i];
			string[] splitted = line.Split(" ");
			string key = splitted[0];
			int value = Int32.Parse(splitted[1]);
			listKeys.Add(key);
			listValues.Add(value);
		}
		part1 = doit();
		part2 = doit(false);
		return;
	}

	static int doit(int i = 0, HashSet<int>? indexesChecked = null) {
		if (i > listKeys.Count - 1) return 0;
		indexesChecked ??= new HashSet<int>();
		if (indexesChecked.Contains(i)) return 0;
		indexesChecked.Add(i);
		string key = listKeys[i];
		int value = listValues[i];
		switch (key) {
			case "acc":
				return value + doit(i + 1, indexesChecked);
			case "jmp":
				return doit(i + value, indexesChecked);
			case "nop":
				return doit(i + 1, indexesChecked);
			default:
				return 0;
		}
	}

	static int doit(bool alreadySwapped, int i = 0, HashSet<int>? indexesChecked = null) {
		if (i > listKeys.Count - 1) return 0;
		indexesChecked ??= new HashSet<int>();
		if (indexesChecked.Contains(i)) return -1;
		indexesChecked.Add(i);
		string key = listKeys[i];
		int value = listValues[i];
		int res;
		switch (key) {
			case "acc":
				res = doit(alreadySwapped, i + 1, indexesChecked);
				if (res == -1) {
					indexesChecked.Remove(i);
					return -1;
				}
				return value + res;
			case "jmp":
				res = doit(alreadySwapped, i + value, indexesChecked);
				if (res == -1) {
					indexesChecked.Remove(i);
					if (alreadySwapped) return -1;
					return doit(true, i + 1, indexesChecked);
				}
				return res;
			case "nop":
				res = doit(alreadySwapped, i + 1, indexesChecked);
				if (res == -1) {
					indexesChecked.Remove(i);
					if (alreadySwapped) return -1;
					return doit(true, i + value, indexesChecked);
				}
				return res;
			default:
				return 0;
		}
	}

};