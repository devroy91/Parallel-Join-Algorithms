# -*- coding: utf-8 -*-
import os
import pandas as pd
import numpy as np
from itertools import accumulate
from numba import cuda
import csv

@cuda.reduce
def sum_reduce(a, b):
    return a + b

##count the no of matches between tables 
def countMatches(table1, table2, idx):
    count = 0
    for value1 in table1[idx]:
        for value2 in table2[idx]:
            if value1 == value2:
                count += 1
    return count

##prefix sum
def prefixSum(array):
    sum_reduce(array)
    res = list(accumulate(array))
    res.insert(0, 0)
    print(array)
    print(res)
    return res

##join
def join(table1, table2, output, pos, idx):
    i = 0
    id1 = 0
    
    for value1 in table1[idx]:
        id2 = 0
        for value2 in table2[idx]:
            if value1 == value2:
                op1 = list(table1.iloc[id1])
                op2 = list(table2.iloc[id2])
                #print(op1, op2, "index = ",pos + i)
                output[pos + i] = op1 + op2
                #print(output[pos + i])
                i += 1
            id2 += 1
        id1 += 1

##table files path
path = os.path.join(os.path.dirname(os.getcwd()), "Tables")
pathMgr = os.path.join(path, "Manager.csv")
pathEmp = os.path.join(path, "Trainees.csv")

##read the tables
mgr = []
emp = []

for chunk in pd.read_csv(pathMgr, chunksize = 1024):
    mgr.append(chunk)

for chunk in pd.read_csv(pathEmp, chunksize = 1024):
    emp.append(chunk)

pos = [0 for _ in range(len(emp))]    
##counting sort to find no of matches
i = 0
for table1 in emp:
    for table2 in mgr:
        pos[i] = countMatches(table1, table2, "managerID" )
        i+=1
        
##calculate prefix sum of positions
pos = prefixSum(pos)
length = pos[len(pos) - 1]
##allocate memory for output
output = [[] for _ in range(length)]

##perform join
idx = 0
col = "managerID"
for table1 in emp:
    for table2 in mgr:
        join(table1, table2, output, pos[idx], col )
        idx += 1

##print output
with open("NIJ_SJoin.csv", "w", newline = "") as outp:
    writer = csv.writer(outp, quoting = csv.QUOTE_NONE)
    writer.writerows(output)