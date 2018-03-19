# -*- coding: utf-8 -*-
import os
from itertools import accumulate
import csvUtil as csv
from numba import cuda
##count the no of matches between tables 
@cuda.jit
def countMatches(table1, table2, idx1, idx2, pos, i):
    
    tx = cuda.threadIdx.x
    ty = cuda.blockIdx.x
    bw = cuda.blockDim.x
    pos = tx + ty * bw
    if(pos < len(table1)):
        if table1[tx][idx1] == table2[pos][idx2]:
            cuda.atomic.add(pos,i,1)
    

##prefix sum
def prefixSum(array):
    res = list(accumulate(array))
    res.insert(0, 0)
    return res

##join
def join(table1, table2, output, pos, idx1, idx2):
    i = 0
    
    for row1 in table1:
        
        for row2 in table2:
            if row1[idx1] == row2[idx2]:
                
                #print(op1, op2, "index = ",pos + i)
                output[pos + i] = row1 + row2
                #print(output[pos + i])
                i += 1
            

##table files path
path = os.path.join(os.path.dirname(os.getcwd()), "Tables")
pathMgr = os.path.join(path, "Manager.csv")
pathEmp = os.path.join(path, "Trainees.csv")

##read the tables
mgr = []
emp = []

for chunk in csv.readRows(pathMgr, chunkSize = 10240):
    mgr.append(chunk)

for chunk in csv.readRows(pathEmp, chunkSize = 10240):
    emp.append(chunk)

pos = [0 for _ in range(len(emp))]  

##finding index of common column
header = emp.pop(0)
idx1 = csv.findIdx(header, "managerID")

header = mgr.pop(0)
idx2 = csv.findIdx(header, "managerID") 
##counting sort to find no of matches
i = 0
for table1 in emp:
    for table2 in mgr:
        threadsperblock = 32 
        blockspergrid = (len(emp) + (threadsperblock - 1))
        countMatches[blockspergrid, threadsperblock](table1, table2, 2, 2, pos, i )
        i+=1
        
##calculate prefix sum of positions
pos = prefixSum(pos)
length = pos[len(pos) - 1]
##allocate memory for output
output = [[] for _ in range(length)]

##perform join
idx = 0

for table1 in emp:
    for table2 in mgr:
        join(table1, table2, output, pos[idx], idx1, idx2 )
        idx += 1

##print output
csv.writeRows("out.csv", rows = output)