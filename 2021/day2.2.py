

with open(".\data\day2.txt", "r") as f:
    values_string = f.read().split('\n')
    #values = [int(item) for item in values_string]

output_str = ""

class Values():
    def __init__(self, direction, value, horizontal, depth, aim):
        self.direction = direction
        self.value = value
        self.horizontal = horizontal
        self.depth = depth
        self.aim = aim
values = [] 

print("----------------")
for i in range(len(values_string)-0): 

    values.append(Values(values_string[i].split(" ")[0], int(values_string[i].split(" ")[1]), 0, 0, 0))
    
    if i != 0:
        values[i].horizontal = values[i-1].horizontal
        values[i].depth = values[i-1].depth
        values[i].aim = values[i-1].aim


    if values[i].direction == "down":
        values[i].aim = values[i].value
        if i != 0:
            values[i].aim += values[i-1].aim
    
    if values[i].direction == "up":
        values[i].aim = -values[i].value
        if i != 0:
            values[i].aim += values[i-1].aim
    
    if values[i].direction == "forward":
        values[i].horizontal = values[i].value
        if i != 0:
            values[i].horizontal += values[i-1].horizontal
            values[i].depth += values[i].value * values[i].aim


    result = values[i].horizontal * values[i].depth
    output_str = f"{values[i].horizontal} * {values[i].depth} * {values[i].aim} = {result}   ({values[i].value}   {values[i].direction})"

    print(output_str)
   
    #with open('output.txt', 'a') as f:
    #    f.write('\n')
    #    f.write(output_str)




"""
1 Ans: 1960569556 --> That's the right answer! You are one gold star closer to finding the sleigh keys.
"""