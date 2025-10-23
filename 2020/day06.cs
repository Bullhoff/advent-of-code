using System.Diagnostics;

public class Day06 {
	static int part1 = 0;
	static int part2 = 0;
	static void Main(string[] args) {
		Stopwatch sw = new Stopwatch();
		sw.Start();
		solve();
		sw.Stop();
		Console.WriteLine("\x1b[38;2;255;55;200mday06\x1b[0m {0} \tpart1: \x1b[38;2;255;165;0m{1}\x1b[0m \tpart2: \x1b[38;2;255;165;0m{2}\x1b[0m", sw.Elapsed, part1, part2);
	}

	static void solve() {
		string str = File.ReadAllText("data/day06.txt").TrimEnd('\r', '\n');
		foreach (string group in str.Split(Environment.NewLine + Environment.NewLine)) {
			Dictionary<char, int> hm = new Dictionary<char, int>();
			string[] rows = group.Split("\n");
			foreach (string person in rows) {
				foreach (char question in person) {
					if (hm.ContainsKey(question)) hm[question] = hm[question] + 1;
					else hm.Add(question, 1);
				}
			}
			part1 += hm.Count;
			foreach (var pair in hm) {
				if (pair.Value == rows.Length) part2++;
			}
		}
		return;
	}

};