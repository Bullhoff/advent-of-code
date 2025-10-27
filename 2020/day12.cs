
int part1 = 0;
int part2 = 0;

System.Diagnostics.Stopwatch sw = new System.Diagnostics.Stopwatch();
sw.Start();
solve();
sw.Stop();
Console.WriteLine("\x1b[38;2;255;55;200mday12\x1b[0m {0} \tpart1: \x1b[38;2;255;165;0m{1}\x1b[0m \tpart2: \x1b[38;2;255;165;0m{2}\x1b[0m", sw.Elapsed, part1, part2);

void solve() {
	IEnumerable<string> lines = File.ReadLines("data/day12.txt");
	List<Tuple<char, int>> instructions = new List<Tuple<char, int>>();
	foreach (string line in lines) {
		char chr = line[0];
		int num = Int32.Parse(line.Substring(1));
		instructions.Add(new Tuple<char, int>(chr, num));
	}
	part1 = solvePart1(instructions);
	part2 = solvePart2(instructions);
}

int solvePart1(List<Tuple<char, int>> instructions) {
	int x = 0;
	int y = 0;
	int dir = 90;
	foreach (Tuple<char, int> item in instructions) {
		char chr = item.Item1;
		int num = item.Item2;
		switch (chr) {
			case 'N':
				y -= num;
				break;
			case 'S':
				y += num;
				break;
			case 'E':
				x += num;
				break;
			case 'W':
				x -= num;
				break;
			case 'L':
				dir -= num;
				if (dir < 0) dir = 360 + dir;
				break;
			case 'R':
				dir += num;
				if (dir >= 360) dir = dir - 360;
				break;
			case 'F':
				switch (dir) {
					case 0:
						y -= num;
						break;
					case 90:
						x += num;
						break;
					case 180:
						y += num;
						break;
					case 270:
						x -= num;
						break;
				}
				break;
		}
	}
	return Math.Abs(x) + Math.Abs(y);
}

int solvePart2(List<Tuple<char, int>> instructions) {
	int x = 0;
	int y = 0;
	int xWp = x + 10;
	int yWp = y - 1;
	foreach (var item in instructions) {
		char chr = item.Item1;
		int num = item.Item2;
		switch (chr) {
			case 'N':
				yWp -= num;
				break;
			case 'S':
				yWp += num;
				break;
			case 'E':
				xWp += num;
				break;
			case 'W':
				xWp -= num;
				break;
			case 'L': {
				int diffX = xWp - x;
				int diffY = yWp - y;
				switch ((num / 90) % 4) {
					case 1:
						xWp = x + diffY;
						yWp = y - diffX;
						break;
					case 2:
						xWp = x - diffX;
						yWp = y - diffY;
						break;
					case 3:
						xWp = x - diffY;
						yWp = y + diffX;
						break;
				}
				break;
			}
			case 'R': {
				int diffX = xWp - x;
				int diffY = yWp - y;
				switch ((num / 90) % 4) {
					case 1:
						xWp = x - diffY;
						yWp = y + diffX;
						break;
					case 2:
						xWp = x - diffX;
						yWp = y - diffY;
						break;
					case 3:
						xWp = x + diffY;
						yWp = y - diffX;
						break;
				}
				break;
			}
			case 'F': {
				int diffX = xWp - x;
				int diffY = yWp - y;
				while (num > 0) {
					x += diffX;
					y += diffY;
					xWp += diffX;
					yWp += diffY;
					num--;
				}
				break;
			}
		}
	}
	return Math.Abs(x) + Math.Abs(y);
}
