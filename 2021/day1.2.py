

#values = [199, 200, 208, 210, 200, 207, 240, 269, 260, 263]
#values_added = []

#measurements_added = []
with open(".\data\day1.txt", "r") as f:
    values_string = f.read().split('\n')
    values = [int(item) for item in values_string]

values_added = []
increased, decreased, same = 0, 0, 0
output_str = ""

def add_values(i, val):
    #val = 0
    if i == 0:
        return val[i]
    elif i == 1:
        return val[i] + val[i-1]
    else: 
        return val[i] + val[i-1] + val[i-2]



for i in range(len(values)-0): 

    values_added.append(add_values(i, values))

   
    if i <= 2:
        print(str(i) + ": " + str(values_added[i]) + " (N/A - no previous measurement)")

    elif int(values_added[i]) > int(values_added[i-1]): 
        increased += 1
        output_str = (str(i) + ": " +str(values[i]) + " --> " + str(values_added[i]) + "   (increased)")

    elif values_added[i] < values_added[i-1]: 
        decreased += 1
        output_str = (str(i) + ": " +str(values[i]) + " --> " + str(values_added[i]) + " (decreased)")

    else: 
        same += 1
        output_str = (str(i) + ": " +str(values[i]) + " --> " + str(values_added[i]) + " (same)")
    
    print(output_str)
    with open('output.txt', 'a') as f:
        f.write('\n')
        f.write(output_str)

    #print(measurements[i], measurements_added[i])
    #print(str(i) + ": " + str(measurements[i]) + " (N/A - no previous measurement)")
            
print("-Increased:" + str(increased) + " -Decreased:" + str(decreased) + " -Same:" + str(same) + " -Total:" + str(increased + decreased + same + 1))
# -Increased:1387 -Decreased:612 -Same:0 -Total:2000




"""
1 Ans: 1363 --> That's not the right answer; your answer is too high. 
2 Ans: 1362 --> That's the right answer! You are one gold star closer to finding the sleigh keys.
"""