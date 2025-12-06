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
		U.PrintResult(sw, "day06", inst.part1, inst.part2);
	}
	
	Int64 part1 = 0;
	Int64 part2 = 0;
	string[]? lines;
	
	void Solve() {
		lines = File.ReadAllText("data/day06.txt").TrimEnd('\r','\n').Split("\n");
		Part1();
		Part2();
	}

	bool IsSeparator(string[] lines, int x){
		for(int y=0; y<lines.Length; y++) if(lines[y][x] != ' ') return false;
		return true;
	}

	void Part1(){
		int x0 = 0;
		for(int x=0; x<lines?[0].Length; x++){
			if(IsSeparator(lines, x) || x==lines[0].Length-1){
				Int64 res = 0;
				char op = lines[^1][x0 .. x].Trim()[0];
				if(op=='*') res=1;
				for(int y=0; y<lines.Length -1; y++){
					int num = Int32.Parse(lines[y][x0 .. (x+1)].Trim());
					switch(op){
						case '+': res += num; break;
						case '*': res *= num; break;
						default: break;
					}
				}
				part1 += res;
				x0 = x;
			}
		}
	}

	void Part2(){
		int x0 = -1;
		char op = default;
		Int64 res = 0;
		for(int x=0; x<lines?[0].Length; x++){
			if(IsSeparator(lines, x) || op == default){
				while(true){
					x0++;
					if(lines[^1][x0] != ' ') {
						op = lines[^1][x0];
						break;
					}
				}
				part2 += res;
				res = 0;
				if(op=='*') res=1;
			}
			
			System.Text.StringBuilder s = new();
			for(int y=0; y<lines.Length -1; y++){
				if(lines[y][x] != ' ') s.Append(lines[y][x]);
			}
			if(s.Length == 0) continue;
			
			int num = Int32.Parse(s.ToString());
			switch(op){
				case '+': res += num; break;
				case '*': res *= num; break;
				default: break;
			}
		}
		part2 += res;
	}

}
