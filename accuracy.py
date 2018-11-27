# -*- coding: utf-8 -*-
"""
Created on Tue Nov 27 17:30:19 2018

@author: xabia
"""

import pandas as pd
import numpy as np 
import csv 

df1 = pd.read_excel('measuresbattery.xlsx')
df2 = pd.read_excel('measurescomputer.xlsx')

data1 = df1.values
data2 = df2.values

dis_1 = [row[0] for row in data1]
dis_2 = [row[0] for row in data2]

diff = []

for i in range(len(dis_1)): 
    diff.append(np.abs(dis_1[i] - dis_2[i]))
    
mean = np.mean(diff)
std = np.std(diff)

