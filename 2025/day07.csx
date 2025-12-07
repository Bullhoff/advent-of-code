#!/usr/bin/env -S dotnet run --file
#:project include/include.csproj
#:property TargetFramework=net10.0
#pragma warning disable CS0164, CS0219, CS8321, CS0168, CS8981, CA1050, IDE0059

public class Program {
	
	static void Main() {
		Program inst = new();
		System.Diagnostics.Stopwatch sw = new();
		sw.Start();
		inst.Solve();
		sw.Stop();
		U.PrintResult(sw, "day07", inst.part1, inst.part2);
	}
	
	int part1 = 0;
	Int64 part2 = 1;
	readonly Dictionary<int, Int64> mem = [];
	
	void Solve() {
		char[][] arrArr = [.. File.ReadAllLines("data/day07.txt").Where(line => !string.IsNullOrWhiteSpace(line)).Select(line => line.TrimEnd('\r')).Select(line => line.ToCharArray())];
		BeamIt(arrArr, 0, arrArr[0].Length / 2);
	}

	Int64 BeamIt(char[][] arrArr, int posY,int posX) {
		while(true){
			if(posY > arrArr.Length-1 || posX > arrArr[0].Length-1 || posX < 0) return 0;
			if(arrArr[posY][posX] == '^'){
				int i = posY + (arrArr.Length * posX);
				if(mem.TryGetValue(i, out Int64 val)){
					part2 += val;
				} else {
					mem[i] = BeamIt(arrArr, posY+1, posX+1);
					mem[i] += BeamIt(arrArr, posY+1, posX-1);
					part1 += 1;
				}
				part2 += 1;
				return mem[i] + 1;
			}
			posY++;
		}
	}

}
