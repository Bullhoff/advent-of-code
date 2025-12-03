public static class U {
	
	public static void PrintResult<T1, T2>(System.Diagnostics.Stopwatch sw, string day, T1 part1, T2 part2) {
		var now = DateTime.Now.ToString("yyyy-MM-ddTHH:mm:ss");
		Console.WriteLine("{0}  \x1b[38;2;255;55;200m{1}\x1b[0m {2} \tpart1: \x1b[38;2;255;165;0m{3}\x1b[0m \tpart2: \x1b[38;2;255;165;0m{4}\x1b[0m", now, day, sw.Elapsed, part1, part2);
	}

}
