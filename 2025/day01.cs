#!/usr/bin/env -S dotnet run --file
#pragma warning disable MSB3270, MSD3246, CS0164, CS0219, CS8321, CS0246, CS0168, CS8981
#:project include/include.csproj
#:property TargetFramework=net10.0

public class Program {
	
	static void Main() {
		Program inst = new Program();
		System.Diagnostics.Stopwatch sw = new System.Diagnostics.Stopwatch();
		sw.Start();
		inst.Solve();
		sw.Stop();
		U.PrintResult(sw, "day01", inst.part1, inst.part2);
	}
	
	int part1 = 0;
	int part2 = 0;
	
	void Solve() {
		int pos = 50;
		IEnumerable<string> lines = File.ReadLines("data/day01.txt");
		foreach(string line in lines){
			char dir = line[0];
			int val = Int32.Parse(line.Substring(1));
			if(pos==0 && dir=='L') part2 -= 1;
			pos += (dir=='R') ? val : -val;
			while(pos > 99 || pos < 0){
				pos += (dir=='L') ? 100 : -100;
				part2++;
			}
			if(pos==0) {
				part1++;
				if(dir != 'R') part2++;
			}
		}
	}

}
