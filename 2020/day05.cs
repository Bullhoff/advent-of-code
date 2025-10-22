using System.Diagnostics;

public class Day05 {
	static int part1 = 0;
	static int part2 = 0;
	static void Main(string[] args) {
		Stopwatch sw = new Stopwatch();
		sw.Start();
		solve();
		sw.Stop();
		Console.WriteLine("\x1b[38;2;255;55;200mday05\x1b[0m {0} \tpart1: \x1b[38;2;255;165;0m{1}\x1b[0m \tpart2: \x1b[38;2;255;165;0m{2}\x1b[0m", sw.Elapsed, part1, part2);
	}

	static void solve() {
		IEnumerable<string> lines = File.ReadLines("data/day05.txt");
		List<int> seatsTaken = new List<int>();
		foreach (string line in lines) {
			int column_from = 0;
			int column_to = 7;
			int row_from = 0;
			int row_to = 127;
			foreach (char letter in line) {
				switch (letter) {
					case 'F':
						row_to -= (row_to - row_from + 1) / 2;
						break;
					case 'B':
						row_from += (row_to - row_from + 1) / 2;
						break;
					case 'R':
						column_from += (column_to - column_from + 1) / 2;
						break;
					case 'L':
						column_to -= (column_to - column_from + 1) / 2;
						break;
					default:
						break;
				}
			}
			int seat = row_to * 8 + column_to;
			if (seat > part1) part1 = seat;
			seatsTaken.Add(seat);
		}
		
		seatsTaken.Sort();
		for (int i = 1; i < seatsTaken.Count; i++) {
			if (seatsTaken[i] - seatsTaken[i - 1] != 1) {
				part2 = seatsTaken[i - 1] + 1;
				break;
			}
		}
		return;
	}

};