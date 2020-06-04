import sys
import operator
import collections
from collections import defaultdict
filename = sys.argv[1]

base_sets = []
set_names = []
sets = [[] for i in range(129)]
name_base = 's'

ordinary_states = ['b', 'o', 'x']
target_states = ['win', 'loss', 'draw']


for i in range(43):
    base_sets.append(name_base+str(i))

for i in range(42):
    s = base_sets[i]

    for item in ordinary_states:
        set_names.append(s+item)

for item in target_states:
    s = base_sets[42]
    set_names.append(s+item)
    
print('sets:', sets)

reader = open(filename, 'r')
raw = reader.readlines()

data = []
for item in raw:
    data.append(item.strip("\n").split(","))

print(len(data))
transpose = []

for s in range(43):
    col = []
    for i in range(len(data)):
        col.append(data[i][s])
    transpose.append(col)

print(set_names)

for column in range(42):
    for state in range(3):
        for i in range(len(data)):
            #print('i:', i, 'state:', transpose[column][i])
            if(transpose[column][i] == ordinary_states[state]):
                sets[(column*3)+state].append(i)

specials = [42]
for special in specials:
    for state in range(3):
        for i in range(len(data)):
            if(transpose[special][i] == target_states[state]):
                sets[(special*3)+state].append(i)

#print(sets[127])

write_name = filename + "_settled.txt"
writer = open(write_name, 'w+')

#writer. write(str(len(data)))
#writer.write("\t")
writer.write("129")
writer.write("\n")


for i in range(129):
    writer.write(str(len(sets[i])))

    for val in range(len(sets[i])):
        writer.write("\t")
        writer.write(str(sets[i][val]))

    writer.write("\n")

sims = {}

for i in range(0, 127):
    intersection = len(list(set(sets[i]).intersection(sets[126])))
    union = (len(sets[i]) + len(sets[126])) - intersection
    jaccard = float(intersection)/union
    
    sims[str(i)] = jaccard

for w in sorted(sims, key=sims.get, reverse=True):
    print(w, sims[w])
