import sys

filename = sys.argv[1]

base_sets = []
set_names = []
sets = [[] for i in range(129)]
name_base = 's'

col_states = ['a', 'b', 'c', 'd', 'e', 'f', 'g', 'h']
row_states = ['1', '2', '3', '4', '5', '6', '7', '8']
target_states = ['zero' ,'one', 'two', 'three', 'four', 'five', 'six', 'seven', 'eight', 'nine', 'ten', 'eleven', 'twelve', 'thirteen', 'fourteen', 'fifteen', 'sixteen']


for i in range(7):
    base_sets.append(name_base+str(i))

for i in range(6):
    s = base_sets[i]

    if(i%2 == 0):
        for item in col_states:
            set_names.append(s+item)
    else:
        for item in row_states:
            set_names.append(s+item)

for item in target_states:
    s = base_sets[6]
    set_names.append(s+item)
    
#print('set_names:', set_names)

for i in range(len(set_names)):
    print('i: ', i , ':', set_names[i])

exit(1)

reader = open(filename, 'r')
raw = reader.readlines()

data = []
for item in raw:
    data.append(item.strip("\n").split(","))

print(len(data))
transpose = []

for s in range(7):
    col = []
    for i in range(len(data)):
        col.append(data[i][s])
    transpose.append(col)

#print(set_names)



for column in range(7):
    for state in range(8):
        for i in range(len(data)):
            #print('i:', i, 'state:', transpose[column][i])
            if(column%2 == 0):
                if(transpose[column][i] == col_states[state]):
                    sets[(column*3)+state].append(i)
            else:
                if(transpose[column][i] == col_states[state]):
                    sets[(column*3)+state].append(i)

specials = [6]
for special in specials:
    for state in range(17):
        for i in range(len(data)):
            if(transpose[special][i] == target_states[state]):
                sets[(special*8)+state].append(i)

#print(sets[127])

write_name = filename + "_settled.txt"
writer = open(write_name, 'w+')

#writer. write(str(len(data)))
#writer.write("\t")
writer.write("65")
writer.write("\n")


for i in range(65):
    writer.write(str(len(sets[i])))

    for val in range(len(sets[i])):
        writer.write("\t")
        writer.write(str(sets[i][val]))

    writer.write("\n")

'''
for i in range(129):
    print('Length of set', i , ':', len(sets[i]))

for i in range(len(data)):
    for j in range(128):
        
        try:
            writer.write(str(sets[j][i]))
        except:
            print('j:', j, 'i:', i)
            exit(1)
        writer.write("\t")
    writer.write(str(sets[128][i]))
    writer.write("\n")
'''
