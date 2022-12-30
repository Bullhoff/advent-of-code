

#measurements = [199, 200, 208, 210, 200, 207, 240, 269, 260, 263]
with open(".\data\day1.txt", "r") as f:
    measurements_string = f.read().split('\n')
    measurements = [int(item) for item in measurements_string]


increased, decreased, same = 0, 0, 0
for i in range(len(measurements)-0): 
    if i == 0:
        print(str(i) + ": " + str(measurements[i]) + " (N/A - no previous measurement)")
    else: 
        if int(measurements[i]) > int(measurements[i-1]): 
            increased += 1
            print(str(i) + ": " + str(measurements[i]) + "   (increased)")
        elif measurements[i] < measurements[i-1]: 
            decreased += 1
            print(str(i) + ": " + str(measurements[i]) + " (decreased)")
        else: 
            same += 1
            print(str(measurements[i]) + " (same)")
            
print("-Increased:" + str(increased) + " -Decreased:" + str(decreased) + " -Same:" + str(same) + " -Total:" + str(increased + decreased + same + 1))
# -Increased:1387 -Decreased:612 -Same:0 -Total:2000
