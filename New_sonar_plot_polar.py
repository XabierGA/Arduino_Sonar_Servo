# -*- coding: utf-8 -*-
"""
Created on Thu Sep 27 22:16:05 2018

@author: xabia
"""

import serial 
import matplotlib.pyplot as plt 
import numpy as np
import seaborn as sns 
import drawnow as dnow

sns.set()

plt.ion()

ser = serial.Serial('COM3' , 9600, timeout = 1)

distance = []
degrees = []
rms = []

def plotear():
    ax = plt.subplot(111, projection = 'polar')
    ax.plot(degrees , distance )
    plt.title('180 degrees position ')
    plt.grid(True)
    #plt.xlim(0,180)
    ax.set_thetamin(0)
    ax.set_thetamax(180)
    
    #plt.xlabel('Degrees º')
    #plt.ylabel('Distance (cm)')
    #plt.plot(degrees , distance , 'bo')
    
    



while True: 
    while (ser.inWaiting()==0): 
        pass
    while (pos!=180):
        valor = ser.readline()
        string = valor.decode('utf-8')
        linea = string.split(',')
        try:
            dis = float(linea[0])
            rmeans = float(linea[1]) 
            pos = float(linea[2])
            
            distance.append(dis)
            degrees.append(pos)
            rmeans.append(rmeans)
        except ValueError: 
            print('check sensor')
        
        dnow.drawnow(plotear)