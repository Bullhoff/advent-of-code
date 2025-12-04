#!/usr/bin/env -S dotnet run --file
#pragma warning disable MSB3270, MSD3246, CS0164, CS0219, CS8321, CS0246, CS0168, CS8981
#:project include/include.csproj
#:property TargetFramework=net10.0

public class Program{
	
	static void Main(){
		Program inst = new Program();
		System.Diagnostics.Stopwatch sw = new System.Diagnostics.Stopwatch();
		sw.Start();
		inst.Solve();
		sw.Stop();
		U.PrintResult(sw, "day04", inst.part1, inst.part2);
	}
	
	int part1 = 0;
	Int64 part2 = 0;
	char[][]? grid;
	
	void Solve(){
		grid = File.ReadAllLines("data/day04.txt").Where(line => !string.IsNullOrWhiteSpace(line)).Select(line => line.TrimEnd('\r')).Select(line => line.ToCharArray()).ToArray();
		DoStuff((int x, int y)=> part1++ );
		while(true){
			int rollsRemoved = DoStuff((int x, int y)=> grid[y][x] = ' ' );
			part2 += rollsRemoved;
			if(rollsRemoved == 0) break;
		}
	}

	int DoStuff(Action<int,int> func){
		int removed = 0;
		if(grid == null) return removed;
		for(int y=0; y<grid.Length; y++){
			for(int x=0; x<grid[y].Length; x++){
				char chr = grid[y][x];
				if(chr != '@') continue;
				int count = 0;
				if(GetCellChar(x,y-1) == '@') count++;
				if(GetCellChar(x,y+1) == '@') count++;
				if(GetCellChar(x-1,y) == '@') count++;
				if(GetCellChar(x+1,y) == '@') count++;
				if(GetCellChar(x-1,y-1) == '@') count++;
				if(GetCellChar(x+1,y+1) == '@') count++;
				if(GetCellChar(x+1,y-1) == '@') count++;
				if(GetCellChar(x-1,y+1) == '@') count++;
				if(count < 4){
					func(x,y);
					removed++;
				}
			}
		}
		return removed;
	}

	char? GetCellChar(int x,int y){
		if(grid==null || y < 0 || x < 0 || y > grid.Length-1 || grid[y]==null || x > grid[y].Length-1) return null;
		return grid[y][x];
	}

}
