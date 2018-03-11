# -*- coding: utf-8 -*-

import pandas as pd
import numpy as np
import os

folder = "Tables"
if not os.path.isdir(folder):
    os.mkdir(folder)
    
path = os.path.join(folder, "names.csv")
names = pd.read_csv(path)["name"]

##number of trainees and managers
emp = 20000
mgr = 500

##mapping managers to trainees
idEmp = np.random.randint(low = 1, high = 10000000, size = 20000)
idEmp = idEmp % 500
idEmp += 1

##creating trainee table
empFile = []
for i in range(emp):
    empFile.append([i + 1, names[i],idEmp[i]])
    
##creating manager table
mgrFile = []
for i in range(mgr):
    mgrFile.append([i + 1, names[30100 + i]])

##converting to dataframes
empTable = pd.DataFrame(empFile, columns = ["empID", "Name", "managerID"])
mgrTable = pd.DataFrame(mgrFile, columns = ["managerID", "Name"])

##writing to csv
path = os.path.join(folder, "Trainees.csv" )
empTable.to_csv(path, index = False)

path = os.path.join(folder, "Manager.csv" )
mgrTable.to_csv(path, index = False)