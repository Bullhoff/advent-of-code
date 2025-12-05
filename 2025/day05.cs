#!/usr/bin/env -S dotnet run --file
#pragma warning disable MSB3270, MSD3246, CS0164, CS0219, CS8321, CS0246, CS0168, CS8981, CS0169
#:project include/include.csproj
#:property TargetFramework=net10.0

public class Program{
	
	static void Main(){
		Program inst = new Program();
		System.Diagnostics.Stopwatch sw = new System.Diagnostics.Stopwatch();
		sw.Start();
		inst.Solve();
		sw.Stop();
		U.PrintResult(sw, "day05", inst.part1, inst.part2);
	}
	
	int part1 = 0;
	Int64 part2 = 0;
	
	void Solve(){
		List<Int64> ranges = new List<Int64>();
		bool atRanges = true;
		foreach(var line in File.ReadLines("data/day05.txt")){
			if(line=="") {atRanges = false; continue;}
			if(atRanges){
				Int64[] nums = line.Split('-').Select(str => Int64.Parse(str)).ToArray();
				ranges.Add(nums[0]);
				ranges.Add(nums[1]);
			} else {
				Int64 num = Int64.Parse(line);
				for(int i=0; i<ranges.Count; i+=2){
					if(num >= ranges[i] && num <= ranges[i+1]) {
						part1++;
						break;
					}
				}
			}
		}
		
		while(MergeRanges(ranges) == true){}
		for(int i=0; i<ranges.Count; i+=2){
			part2 += ranges[i+1] - ranges[i] + 1;
		}
	}

	bool MergeRanges(List<Int64> ranges){
		for(int i=0; i<ranges.Count; i+=2){
			var from1 = ranges[i];
			var to1 = ranges[i+1];
			for(int j=i+2; j<ranges.Count; j+=2){
				var from2 = ranges[j];
				var to2 = ranges[j+1];
				if(from1 <= to2 && to1 >= from2) {
					if(from2 < from1) ranges[i] = from2;
					if(to2 > to1) ranges[i+1] = to2;
					ranges.RemoveRange(j,2);
					return true;
				}
			}
		}
		return false;
	}

}
