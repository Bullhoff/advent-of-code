using System.Diagnostics;

int part1 = 0;
Int64 part2 = 0;
List<int> numbers = new List<int>();
Dictionary<int, Int64> mem = new Dictionary<int, Int64>();
int count1 = 0;
int count3 = 0;

Stopwatch sw = new Stopwatch();
sw.Start();
solve();
sw.Stop();
Console.WriteLine("\x1b[38;2;255;55;200mday10\x1b[0m {0} \tpart1: \x1b[38;2;255;165;0m{1}\x1b[0m \tpart2: \x1b[38;2;255;165;0m{2}\x1b[0m", sw.Elapsed, part1, part2);

void solve() {
	IEnumerable<string> lines = File.ReadLines("data/day10.txt");
	foreach (string line in lines) {
		int num = Int32.Parse(line);
		numbers.Add(num);
	}
	numbers.Sort();
	int joltage = 0;
	for (int i = 0; i < numbers.Count; i++) {
		int num = numbers[i];
		if (num - joltage == 1) count1++;
		else if (num - joltage == 3) count3++;
		joltage = num;
	}
	part1 = count1 * (count3 + 1);
	part2 = getPossibilities();
}

Int64 getPossibilities(int i = -1, int joltage = 0) {
	if (i >= numbers.Count - 1) return 1;
	Int64 arrangements = 0;
	arrangements += getPossibilitiesFromIndex(i + 1, joltage);
	arrangements += getPossibilitiesFromIndex(i + 2, joltage);
	arrangements += getPossibilitiesFromIndex(i + 3, joltage);
	return arrangements;
}

Int64 getPossibilitiesFromIndex(int i = 0, int joltage = 0) {
	Int64 res = 0;
	if (i < numbers.Count && numbers[i] - joltage <= 3) {
		if (mem.ContainsKey(i)) {
			res = mem[i];
		} else {
			res = getPossibilities(i, numbers[i]);
			mem[i] = res;
		}
	}
	return res;
}
