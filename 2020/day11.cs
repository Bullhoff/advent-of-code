
int part1 = 0;
int part2 = 0;

System.Diagnostics.Stopwatch sw = new System.Diagnostics.Stopwatch();
sw.Start();
solve();
sw.Stop();
Console.WriteLine("\x1b[38;2;255;55;200mday11\x1b[0m {0} \tpart1: \x1b[38;2;255;165;0m{1}\x1b[0m \tpart2: \x1b[38;2;255;165;0m{2}\x1b[0m", sw.Elapsed, part1, part2);

void solve() {
	char[][] grid = File.ReadAllLines("data/day11.txt").Where(line => !string.IsNullOrWhiteSpace(line)).Select(line => line.TrimEnd('\r')).Select(line => line.ToCharArray()).ToArray();
	char[][] grid2 = grid.Select(row => row.ToArray()).ToArray();
	while (seatsChanged(grid) != 0) part1 = 0;
	while (seatsChanged(grid2, true) != 0) part2 = 0;
}

int seatsChanged(char[][] grid0, bool isPart2 = false) {
	int seatsChanged = 0;
	char[][] grid = grid0.Select(row => row.ToArray()).ToArray();
	for (int y = 0; y < grid.Length; y++) {
		for (int x = 0; x < grid[y].Length; x++) {
			if (grid[y][x] == '.') continue;
			
			bool isNeighbor(int dy, int dx, int yy = -1, int xx = -1) {
				if (yy == -1) yy = y;
				if (xx == -1) xx = x;
				yy += dy;
				xx += dx;
				if (yy >= 0 && xx >= 0 && yy < grid.Length && xx < grid[yy].Length) {
					if (grid[yy][xx] == '#') return true;
					if (grid[yy][xx] == '.' && isPart2) return isNeighbor(dy, dx, yy, xx);
				}
				return false;
			}
			int neighbors = 0;
			if (isNeighbor(-1, 0)) neighbors++;
			if (isNeighbor(0, -1)) neighbors++;
			if (isNeighbor(+1, 0)) neighbors++;
			if (isNeighbor(0, +1)) neighbors++;
			if (isNeighbor(-1, -1)) neighbors++;
			if (isNeighbor(-1, +1)) neighbors++;
			if (isNeighbor(+1, -1)) neighbors++;
			if (isNeighbor(+1, +1)) neighbors++;
			
			if (neighbors == 0 && grid0[y][x] == 'L') grid0[y][x] = '#';
			else if (neighbors >= (isPart2 ? 5 : 4) && grid0[y][x] == '#') grid0[y][x] = 'L';
			
			if (grid0[y][x] != grid[y][x]) seatsChanged++;
			if (grid0[y][x] == '#') {
				if (isPart2) part2++;
				else part1++;
			}
		}
	}
	return seatsChanged;
}
