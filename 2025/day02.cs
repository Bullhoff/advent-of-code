#!/usr/bin/env -S dotnet run --file
#pragma warning disable MSB3270, MSD3246, CS0164, CS0219, CS8321, CS0246, CS0168, CS8981
#:project include/include.csproj
#:property TargetFramework=net10.0
#nullable enable

using System.Text.RegularExpressions;
using System.Collections;
using System.Reflection;

public class Program {
	static void Main() {
		Program inst = new Program();
		System.Diagnostics.Stopwatch sw = new System.Diagnostics.Stopwatch();
		sw.Start();
		inst.Solve();
		sw.Stop();
		U.PrintResult(sw, "day02", inst.part1, inst.part2);
	}
	
	Int64 part1 = 0;
	Int64 part2 = 0;
	
	void Solve() {
		string[] lines = File.ReadAllText("data/day02.txt").TrimEnd('\r','\n').Split(",");
		for(int i=0; i<lines.Length; i++){
			string[] splitted = lines[i].Split('-');
			var n1 = Int64.Parse(splitted[0]);
			var n2 = Int64.Parse(splitted[1]);
			for(Int64 j=n1; j<=n2; j++){
				string str = j.ToString();
				if(!IsValid(str)) part1 += j;
				if(!IsValider(str)) part2 += j;
			}
		}
	}

	bool IsValid(string str, int splitAt = 2){
		if(str.Length % splitAt != 0) return true;
		for (int i=0; i<splitAt; i++){
			for(int j=1; j<str.Length/splitAt; j++){
				int jj = i + j*splitAt;
				if(str[i] != str[jj]) return true;
			}
		}
		return false;
	}

	bool IsValider(string str){
		for(int i=1; i<str.Length; i++) if(!IsValid(str, i)) return false;
		return true;
	}

}
