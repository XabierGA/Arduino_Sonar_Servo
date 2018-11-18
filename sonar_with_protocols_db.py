# -*- coding: utf-8 -*-
"""
Created on Sun Oct  7 13:51:19 2018

@author: xabia

"""
import MySQLdb
import serial 
import matplotlib.pyplot as plt 
import numpy as np
import seaborn as sns 
import drawnow as dnow
import atexit
import time

#set the seaborn style for plots
sns.set()

#allow interactive graphs so that they can be automatically updated
plt.ion()

#establish the serial port
ser = serial.Serial('/dev/ttyACM0' , 9600, timeout = 1)

def input_sql(dis,rms,pos):
    db = MySQLdb.connect("localhost", "dell" , "$Pass123" , "sonar") #initialize our database
    cur = db.cursor() #cursor to write commands to the mysql-server prompt
    sql = "insert into dis_rms_pos (dis,rms,pos) values (%f, %f , %d);" % (dis , rms , pos)
    cur.execute(sql)
    db.commit() #make sure that all changes are commited simultaneously
    db.close()

distance = []
degrees = []
rmeans = []

def cleanup():
    ser.close()

atexit.register(cleanup)

ser.flushInput()
ser.flush()
ser.flushOutput()
time.sleep(1)
def plotear():
    ax = plt.subplot(111, projection = 'polar')
    aux = [2*np.pi*x/360.0 for x in degrees]
    ax.plot(aux , distance , 'bo' )
    plt.title('180 degrees position ')
    plt.grid(True)
    #plt.xlim(0,180)
    ax.set_thetamin(0)
    ax.set_thetamax(60)

       
    
while True: 

    command = input()
    
    data = command.split(' ')
    if (command == 'SCAN?'):
        ser.write(command.encode('utf-8'))
        while (ser.inWaiting()==0): 
            pass
        pos = 0
        while (pos!=180):
            valor = ser.readline()
            string = valor.decode('utf-8')
            linea = string.split(',')
            print(linea)
            try:
                dis = float(linea[0])
                rms = float(linea[1]) 
                pos = float(linea[2])
            
                distance.append(dis)
                degrees.append(pos)
                rmeans.append(rms)
                input_sql(dis,rms,pos)
            except ValueError: 
                print('check sensor')
            except IndexError: 
                pass
            dnow.drawnow(plotear)
            print(pos)
            
           
        
    elif (command == 'MEASURE?'):
        ser.write(command.encode('utf-8'))
        while (ser.inWaiting()==0):
            pass
        valor = ser.readline()
        string = valor.decode('utf-8')
        linea = string.split(',')
        try: 
            dis = float(linea[0])
            rms = float(linea[1])
            pos = int(linea[2])
        
            distance.append(dis)
            rmeans.append(rms)
            input_sql(dis,rms,pos)
        
        except ValueError: 
            print('check sensor')
        except IndexError: 
            pass
        dnow.drawnow(plotear)
        
    
    elif (data[0]=='GOTO' and len(data)<=3):
        ser.write(command.encode('utf-8'))
        print(data[1])
    elif (data[0]=='GOTO'  and data[2] == 'and' and data[3] == 'measure'):
        ser.write(command.encode('utf-8'))
        while (ser.inWaiting()==0):
            pass
        ser.flush()
        valor = ser.readline()
        string = valor.decode('utf-8')
        linea = string.split(',')
        try: 
            dis = float(linea[0])
            rms  = float(linea[1])
            pos = int(linea[2])
        
            distance.append(dis)
            rmeans.append(rms)
            degrees.append(pos)
        
        except ValueError: 
            print('check sensor')
        except IndexError: 
            pass
        dnow.drawnow(plotear)
        print(data[1])
        input_sql(dis,rms,pos)
    else: 
        print("Unknown command")
