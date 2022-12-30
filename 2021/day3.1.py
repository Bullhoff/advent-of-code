

with open(".\data\day3.txt", "r") as f:
    values_string = f.read().split('\n')
    #values = [int(item) for item in values_string]

output_str = ""

values_sums = [0] * len(values_string[0]) 
gamma = [0] * len(values_string[0]) 
epsilon = [0] * len(values_string[0]) 

print("----------------")
for i in range(len(values_string)): 
    for y in range(len(values_string[i])):
        values_sums[y] += (int(values_string[i][y]))

    
      
for i in range(len(values_sums)): 
    if float(values_sums[i]) > len(values_string)/2:
        gamma[i] = 1
    else: 
        epsilon[i] = 1


def list_to_str(list):
    s = [str(i) for i in list]
    res = "".join(s)
    return(res)

gamma_binary = list_to_str(gamma)
epsilon_binary = list_to_str(epsilon)

gamma_decimal = int(gamma_binary, 2)
epsilon_decimal = int(epsilon_binary, 2)

result = gamma_decimal * epsilon_decimal
output_str = f"result: {result} --- gamma: {gamma_decimal} -- epsilon: {epsilon_decimal} ---- len(values_string)/2: {len(values_string)/2} - values_sums[y]: {float(values_sums[y])}"
print(output_str)

#int_test = 0
#int_test[0] = 2
    #with open('output.txt', 'a') as f:
    #    f.write('\n')
    #    f.write(output_str)




"""
1 Ans: 4118544 --> That's the right answer! You are one gold star closer to finding the sleigh keys.
"""