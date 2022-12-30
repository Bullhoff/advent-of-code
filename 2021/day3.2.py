

with open(".\data\day3.txt", "r") as f:
    values_string = f.read().split('\n')
    #values = [int(item) for item in values_string]

output_str = ""

#values_sums = [0] * len(values_string[0]) 
gamma =  []#[0] * len(values_string[0]) 
epsilon = []#[0] * len(values_string[0]) 

print("----------------")


def check_number(list, gamma_or_epsilon): # recursive function
    list = list
    most_common = [0] * len(list[0]) 
    values_sums = [0] * len(list[0]) 
    for y in range(len(list[0])): # iterate binary position (y)
        if len(list) == 1: 
            break

        for i in range(len(list)):  # iterate list (i)
            values_sums[y] += (int(list[i][y]))    # add selected bit to a new list
        
        if (float(values_sums[y]) >= len(list)/2): # and (gamma_or_epsilon=="gamma"):
            most_common[y] = 1
        #elif (float(values_sums[y]) >= len(list)/2) and (gamma_or_epsilon=="epsilon"):
            #most_common[y] = 1
        
        i = 0
        while i < len(list):
            output_str = f"{i} {y} - {values_sums[y]} - {most_common[y]} - {list[i]} {gamma_or_epsilon}"
            #print(f"{list[i]} bajs {i} {y}  {len(list)/2} -- {list[i][y]} <-- {most_common[y]}") #{values_sums[y]}
            if float(list[i][y]) != most_common[y] and (gamma_or_epsilon=="gamma"):
                #print("1:",list[i])
                del list[i]
                #print("2:",list[i])
                #i = i - 1
            elif (float(list[i][y]) == most_common[y]) and (gamma_or_epsilon=="epsilon"):
                del list[i]
            else:
                i = i + 1

    return list

values_string2 = values_string[:]
values_string3 = values_string[:]

gamma = check_number(values_string2, "gamma")
epsilon = check_number(values_string3, "epsilon")
print(f"Gamma binary: {gamma}")
print(f"epsilon binary: {epsilon}")
#print(f"values_string: {values_string}")


def list_to_str(list):
    s = [str(i) for i in list]
    res = "".join(s)
    return(res)

gamma_binary = list_to_str(gamma)
epsilon_binary = list_to_str(epsilon)

gamma_decimal = int(gamma_binary, 2)
epsilon_decimal = int(epsilon_binary, 2)

result = gamma_decimal * epsilon_decimal
output_str = f"result: {result} --- gamma: {gamma_decimal} -- epsilon: {epsilon_decimal} ---- len(values_string)/2: {len(values_string)/2} "
print(output_str)



"""
1 Ans: 3832770 --> That's the right answer! You are one gold star closer to finding the sleigh keys.
"""