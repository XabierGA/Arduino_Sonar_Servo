# -*- coding: utf-8 -*-
"""
Created on Sun Oct  7 13:51:19 2018

@author: xabia
"""

import serial 
import matplotlib.pyplot as plt 
import numpy as np
import seaborn as sns 
import drawnow as dnow

sns.set()

plt.ion()

ser = serial.Serial('/dev/ttyACM0' , 9600, timeout = 1)

distance = []
degrees = []
rmeans = []

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
    
    command = input()
    
    data = command.split(' ')
    if (command == 'scan'):
        ser.write(command.encode('utf-8'))
        while (ser.inWaiting()==0): 
            pass
        pos = 0
        while (pos!=180):
            valor = ser.readline()
            string = valor.decode('utf-8')
            linea = string.split(',')
            try:
                dis = float(linea[0])
                rms = float(linea[1]) 
                pos = float(linea[2])
            
                distance.append(dis)
                degrees.append(pos)
                rmeans.append(rms)
            except ValueError: 
                print('check sensor')
            except IndexError: 
                pass
            dnow.drawnow(plotear)
            print(pos)
           
        
    elif (command == 'measure'):
        ser.write(command.encode('utf-8'))
        while (ser.inWaiting()==0):
            pass
        ser.flush()
        valor = ser.readline()
        string = valor.decode('utf-8')
        linea = string.split(',')
        try: 
            dis = float(linea[0])
            rms = float(linea[1])
        
            distance.append(dis)
            rmeans.append(rms)
        
        except ValueError: 
            print('check sensor')
        except IndexError: 
            pass
        dnow.drawnow(plotear)
    
    elif (data[0]=='go' and data[1] == 'to'):
        ser.write(command.encode('utf-8'))
        print(data[2])
    elif (data[0]=='go' and data[1] == 'to' and data[2] == 'and' and data[3] == 'measure'):
        ser.write(command.encode('utf-8'))
        while (ser.inWaiting()==0):
            pass
        ser.flush()
        valor = ser.readline()
        string = valor.decode('utf-8')
        linea = string.split(',')
        try: 
            dis = float(linea[0])
            rmeans = float(linea[1])
        
            distance.append(dis)
            rmeans.append(rms)
        
        except ValueError: 
            print('check sensor')
        except IndexError: 
            pass
        dnow.drawnow(plotear)
        print(data[4])
    else: 
        print("Unknown command")
