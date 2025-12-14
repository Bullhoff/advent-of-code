#!/usr/bin/env -S dotnet run --file
#:project include/include.csproj
#:property TargetFramework=net10.0
#pragma warning disable CS0164, CS0219, CS8321, CS0168, CS8981, CA1050, IDE0059

public class Program {
	
	static void Main(string[] args) {
		Program inst = new();
		System.Diagnostics.Stopwatch sw = new();
		sw.Start();
		if(args.Length==0) inst.Solve(); else inst.Solve(args[0]);
		sw.Stop();
		U.PrintResult(sw, "day08", inst.part1, inst.part2);
	}
	
	int part1 = 0;
	Int64 part2 = 0;
	
	void Solve(string file_ = "data/day08.txt"){
		int stopAfter = file_.Contains("_day")? 10 : 1000;
		List<(int x,int y,int z)> arr = [];
		List<(Int64 d, int i,int j)> distances = [];
		List<List<int>> circuits = [];
		int j = 0;
		
		foreach(var line in File.ReadLines(file_)){
			int[] xyz = [ .. line.Split(',').Select(int.Parse) ];
			for(int i=0; i<arr.Count; i++){
				int dx = xyz[0] - arr[i].x;
				int dy = xyz[1] - arr[i].y;
				int dz = xyz[2] - arr[i].z;
				distances.Add( ( d:(Int64)dx*dx+(Int64)dy*dy+(Int64)dz*dz, i, j ) );
			}
			circuits.Add([j]);
			arr.Add( (x:xyz[0], y:xyz[1], z:xyz[2]) );
			j++;
		}
		distances.Sort((a, b) => a.d.CompareTo(b.d));
		
		for(int ii=0; ii<distances.Count; ii++){
			int i1 = circuits.FindIndex(list=>list.Contains(distances[ii].i));
			int i2 = circuits.FindIndex(list=>list.Contains(distances[ii].j));
			if(i1 == i2) continue;
			
			foreach(var index in circuits[i2]) circuits[i1].Add(index);
			circuits.RemoveAt(i2);
			
			if(ii == stopAfter) {
				circuits.Sort((a, b) => b.Count.CompareTo(a.Count));
				part1 = circuits[0].Count * circuits[1].Count * circuits[2].Count;
			}
			if (circuits.Count==1){
				part2 = (Int64)arr[distances[ii].i].x * (Int64)arr[distances[ii].j].x;
				break;
			}
		}
	}

}
