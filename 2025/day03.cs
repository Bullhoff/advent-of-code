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
		U.PrintResult(sw, "day03", inst.part1, inst.part2);
	}
	
	int part1 = 0;
	Int64 part2 = 0;
	
	void Solve(){
		foreach (string line in File.ReadLines(@"data/day03.txt")){
			int[] nums = { 0, 0 };
			int[] nums_part2 = new int[12];
			for (int i = 0; i < line.Length; i++){
				Int32.TryParse(line[i].ToString(), out int num);
				if (i != line.Length - 1 && nums[0] < num){
					nums[0] = num;
					nums[1] = 0;
				}else if (nums[1] < num){
					nums[1] = num;
				}
				
				/// part2
				bool changed = false;
				for (int j = 0; j < nums_part2.Length; j++){
					if (changed){
						nums_part2[j] = 0;
					} else if ((i-j <= line.Length - nums_part2.Length) && (nums_part2[j] < num)){
						nums_part2[j] = num;
						changed = true;
					}
				}
			}
			part1 += nums[0] * 10 + nums[1];
			
			/// part2
			for (int j = 0; j < nums_part2.Length; j++){
				part2 += nums_part2[j] * (Int64)Math.Pow(10, nums_part2.Length - j - 1);
			}
		}
	}

}
