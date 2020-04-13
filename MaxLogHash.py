import random
import math
import argparse
import time
import sys
import mmh3
import numpy as np
from heapq import heapify,heappop,heappush
import time

totalShingles = (1 << 32) - 1

def MaxLog(seed):
    randomNoA = hash_parameter(k)
    randomNoB = hash_parameter(k)

    

    
    for item in stream:

        #print(item)
        #print(maxShingleID)
        
        if item [0] in maxShingleID.keys():
            max_hash_val_list = maxShingleID[item[0]][0]
            max_hash_sig_list = maxShingleID[item[0]][1]

            #print(max_hash_val_list)
            #print(max_hash_sig_list)
            
                        
            # print max_hash_val_list
            # print max_hash_sig_list
            for x  in range(0, k):
                temp = ((randomNoA[x] * mmh3.hash(str(item[1]),seed) + randomNoB[x]) % totalShingles)
                temp = temp / float(totalShingles)

                #print('Temp: ', temp)
                
                log_temp = - math.log(temp,2)
                hash_val =  math.ceil(log_temp)

                #print('Hash val: ' , hash_val)
                
                if hash_val > max_hash_val_list[x]:
                   max_hash_val_list[x] = hash_val
                   max_hash_sig_list[x] = 1
                elif hash_val == max_hash_val_list[x]:
                     max_hash_sig_list[x] = 0
                     #maxShingleID [item[0]][0]= max_hash_val_list
            #maxShingleID [item[0]][1]= max_hash_sig_list
            ##
        else:
            max_hash_val_list = [-1] * k
            max_hash_sig_list = [0] * k
            max_hash_res_new = []
            max_hash_res_new.append(max_hash_val_list)
            max_hash_res_new.append(max_hash_sig_list)
            maxShingleID[item[0]] = max_hash_res_new
    return


def hash_parameter(k):
    randList = []
    randIndex = random.randint(0, totalShingles - 1)
    randList.append(randIndex)
    counter = 0
    while k > 0:
        #print('randlist: ', randList, 'counter: ', counter)
        counter += 1
        while randIndex in randList:
              randIndex = random.randint(0, totalShingles - 1)
              counter += 1
        randList.append(randIndex)
        k = k - 1

    #print('Length of randList: ', len(randList))
    return randList

def estimate():

    #for i in range(k):
        #print(maxShingleID['setA'][0][i], ':', maxShingleID['setA'][1][i])
    
    con = 0
    for x in range(0, k):
       if (maxShingleID['setA'][0][x] > maxShingleID['setB'][0][x] and  maxShingleID['setA'][1][x] ==1 ):
            con = con + 1
       elif (maxShingleID['setA'][0][x] < maxShingleID['setB'][0][x] and  maxShingleID['setB'][1][x] ==1 ):
            con = con + 1
    # print con
    num = float(k)
    # print num
    jaccard_sim = 1.0 - con*(1/num)*(1/0.7213)
    #print('con: ', con)
    return jaccard_sim


def generate_stream(stream, cardinality):

    setA = []
    setB = []
    
    total_num = cardinality * 2
    the_same_index = total_num / 2 * sim
    setA_uni_index = total_num / 2 * 1
    setB_uni_index = total_num / 2 * (2 - sim)
    #synthetic data
    for num in range(total_num):
        if num <= the_same_index:
            stream.append(['setA', num])
            stream.append(['setB', num])
            setA.append(num)
            setB.append(num)
        elif num <= setA_uni_index:
            stream.append(['setA', num])
            setA.append(num)
        elif num <= setB_uni_index:
            stream.append(['setB', num])
            setB.append(num)
        else:
            break

    return setA, setB

def read_file_into_stream(filename):

    data = open(filename, 'r')
    lines = data.readlines()
    records = []
    
    for item in lines:
        records.append(item.split(','))

    uid = -1
    for item in records:
        uid += 1
        uname = 'u' + str(uid)
        for element in item:
            #print('uid: ', uid, 'element: ', element)
            stream.append([element, uname])


def write_stream(setA, setB, cardinality):

    filer = open(str(cardinality)+'.txt', 'w+')

    filer.write(str(cardinality))
    filer.write('\n')

    for i in range(cardinality):
        filer.write(str(setA[i]))
        filer.write(' ')
        filer.write(str(setB[i]))
        filer.write('\n')

def read_generated_into_stream(filename, stream):

    filer = open(filename, 'r')
    lines = filer.readlines()

    i = 0

    for item in lines:
        if(i != 0):
            item = item[0:item.find('\n')]
            value = item.split(" ")
            stream.append(['setA', int(value[0])])
            stream.append(['setB', int(value[1])])
        i = i+1
    
    
    

if __name__ == '__main__':
    random_seed = 1
    card = 100000
    jaccard_true = 0.9
    k = int(128)
    total_num = card * 2
    sim = (2 * jaccard_true) / (1 + jaccard_true)
    mod = sys.argv[1]

    maxShingleID = {}
        
    stream = []

    if(mod == 'genstream'):
        cardinality = int(sys.argv[2])
        setA, setB = generate_stream(stream, cardinality)
        write_stream(setA, setB, cardinality)

    elif(mod == 'readstream'):
        filename = sys.argv[2]
        read_generated_into_stream(filename, stream)

    else:
        print('Wtf')


    mltimes = time.time()
    MaxLog(random_seed)
    mltimef = time.time()
    jtimes = time.time()
    jaccard_est = estimate()
    jtimef = time.time()
    #print('Len Stream: ', len(stream))
    #print('setA: ', setA)
    #print('setB', setB)
    print(jaccard_true, jaccard_est)
    mltime = mltimef - mltimes
    jtime = jtimef - jtimes
    print('Max Log Time: ', mltime)
    print('Jaccard Time: ', jtime)
    print('Total:', mltime + jtime)



