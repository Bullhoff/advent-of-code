#!/usr/bin/env -S dotnet run --file
#:project include/include.csproj
#:property TargetFramework=net10.0
#pragma warning disable CS0164, CS0219, CS8321, CS0168, CS8981, CA1050, IDE0059, CA1806

public class Program {
	
	static void Main(string[] args) {
		Program inst = new();
		System.Diagnostics.Stopwatch sw = new();
		sw.Start();
		if(args.Length==0) inst.Solve();
		else inst.Solve(args[0]);
		sw.Stop();
		U.PrintResult(sw, "day09", inst.part1, inst.part2);
	}
	
	Int64 part1 = 0;
	Int64 part2 = 0;
	
	void Solve(string file_ = "data/day09.txt") {
		int[][] arrArr = [ .. File.ReadAllLines(file_).Where(line => !string.IsNullOrWhiteSpace(line)).Select(line => line.TrimEnd('\r')).Select(line => line.Split(',')).Select(chr => chr.Select(int.Parse).ToArray()) ];
		List<(Int64,int,int)> sizes = [];
		
		for(int i=0; i<arrArr.Length; i++){
			var c1 = arrArr[i];
			for(int j=0; j<arrArr.Length; j++){
				if(i==j) continue;
				var c2 = arrArr[j];
				Int64 size = (Int64)(Math.Abs(c2[0]-c1[0])+1) * (Int64)(Math.Abs(c2[1]-c1[1])+1);
				if(size > part1) part1 = size;
				sizes.Add( (size,i,j) );
			}
		}
		
		sizes.Sort((a, b) => b.Item1.CompareTo(a.Item1));
		for(int ii=0; ii<sizes.Count; ii++){
			var (size,i,j) = sizes[ii];
			if(IsWithin(arrArr, i,j)){
				part2 = size;
				break;
			}
		}
	}

	bool IsWithin(int[][] arrArr, int i1, int i2){
		int x1 = arrArr[i1][0];
		int y1 = arrArr[i1][1];
		int x2 = arrArr[i2][0];
		int y2 = arrArr[i2][1];
		if(x1 > x2) (x1,x2) = (x2,x1);
		if(y1 > y2) (y1,y2) = (y2,y1);
		for(int i=0; i<arrArr.Length; i++){
			int j = (i==arrArr.Length-1)? 0 : i+1;
			if(i == i1 || i == i2 || j == i1 || j == i2) continue;
			var x3 = arrArr[i][0];
			var y3 = arrArr[i][1];
			var x4 = arrArr[j][0];
			var y4 = arrArr[j][1];
			if(x3 > x4) (x3,x4) = (x4,x3);
			if(y3 > y4) (y3,y4) = (y4,y3);
			if(y3 < y2 && y4 > y1 && x3 < x2 && x4 > x1) return false;
		}
		return true;
	}

}
