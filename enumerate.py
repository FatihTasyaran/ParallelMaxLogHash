import sys

if __name__ == '__main__':
    dataset = sys.argv[1]

    filer = open(dataset, 'r')
    datastr = filer.readlines()

    print(datastr[0])
    print(type(datastr[0]))

    data = []

    for line in datastr:
        data.append(line.split(','))


    print(data[0])

    idx = 1
    seen_first = {}

    for items in data:
        for item in items:
            if item not in seen_first:
                seen_first[item] = idx
                idx += 1

    print('Mapping:', seen_first)

    data_enumerated = []

    for items in data:
        to_append = []
        for item in items:
            to_append.append(seen_first[item])
        data_enumerated.append(to_append)

    '''
    print('Remap check:')
    for i in range(20):
        print('Record %d'%i)
        print(data[i])
        print(data_enumerated[i])
    '''

    ##Write back##
    write_name =  dataset + '_enum'

    writer = open(write_name, 'w+')

    writer.write(str(len(data_enumerated)) + '\t' + str(len(data_enumerated[0])))
    writer.write('\n')
    
    for items in data_enumerated:
        for i in range (len(items)):
            if(i != len(items) -1):
                writer.write(str(items[i]))
                writer.write('\t')
            else:
                writer.write(str(items[i]))
                writer.write('\n')
