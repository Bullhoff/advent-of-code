
int part1 = 0;
Int64 part2 = 0;

System.Diagnostics.Stopwatch sw = new System.Diagnostics.Stopwatch();
sw.Start();
solve();
sw.Stop();
Console.WriteLine("\x1b[38;2;255;55;200mday13\x1b[0m {0} \tpart1: \x1b[38;2;255;165;0m{1}\x1b[0m \tpart2: \x1b[38;2;255;165;0m{2}\x1b[0m", sw.Elapsed, part1, part2);

void solve() {
	string[] lines = File.ReadAllLines("data/day13.txt");
	int earliest = Int32.Parse(lines[0]);
	var busses = lines[1].Split(',').Select(x => x == "x" ? -1 : Int32.Parse(x)).ToList();
	
	int solvePart1() {
		int nextBusNr = 0;
		int nextBusTimeLeft = -1;
		for (int x = 0; x < busses.Count; x++) {
			int busNr = busses[x];
			if (busNr == -1) continue;
			
			int timeUntilNext = -earliest;
			while (timeUntilNext < 0) timeUntilNext += busNr;
			
			if (nextBusTimeLeft == -1 || timeUntilNext < nextBusTimeLeft) {
				nextBusNr = busNr;
				nextBusTimeLeft = timeUntilNext;
			}
		}
		return nextBusNr * nextBusTimeLeft;
	}
	
	Int64 solvePart2() {
		Int64 time = 0;
		Int64 y = busses[0];
		for (var x = 1; x < busses.Count; x++) {
			if (busses[x] == -1) continue;
			while ((time + x) % busses[x] != 0) time += y;
			y *= busses[x];
		}
		return time;
	}
	
	part1 = solvePart1();
	part2 = solvePart2();
	return;
}
