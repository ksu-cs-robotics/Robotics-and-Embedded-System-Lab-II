#!/usr/bin/env python
#
# Redirect data from a serial port (bluetooth) connection to a serial port and vice versa.
#
##########################################################################
# Project           : RES_Class, Serial to Serial fowarder 
#
# Program name      : serial2bluetooth.py
#
# Author            : Jong-Hoon Kim
#
# Date created      : 05/04/2022
#
# Purpose           : Message fowarding from USB serial (Arduino) to Bluetooth
#						
#
# Revision History  :
#
# Date        Author      Ref    Revision (Date in MMDDYYYY format) 
# MMDDYYYY    name      1  v-xx   revision note. 
#
##########################################################################
##########################################################################
#   Instructional Note:  
#           
#           
##########################################################################


from time import sleep
import serial
import numpy as np
import cv2
import threading
import time
import sys

img = np.zeros((800,1000,3), np.uint8)
img_imu = np.zeros((400,500,3), np.uint8)
img_imu = cv2.circle(img_imu,(250,200),(140),(255,255,0),5)
img_sonar = np.zeros((400,500,3), np.uint8)
img_sonar = cv2.circle(img_sonar,(250,10),(310),(255,255,0),5)


######################################################################
# Bluetooth port setup. depend your pc, port number will be differet
######################################################################
sUSB = serial.Serial('COM29', 115200) # Establish the connection on a specific port
sBluetooth = serial.Serial('COM10', 115200) # Establish the connection on a specific port

xPnt = xPntP = yPnt = yPntP = 0
xPnt1 = xPntP1 = yPnt1 = yPntP1 = 0
xPnt2 = xPntP2 = yPnt2 = yPntP2 = 0

myLock = threading.Lock()

def thread_bluetooth(event):
 while not event.is_set():
   try:
      msg = sBluetooth.readline() # Read the newest output from the Arduino
#      print ((msg).decode())
#      tmpStr = msg.split( )
      while True:	
		######################################################################
		#        Message Forwarding from bluetooth to usbserial(Arduino)
		######################################################################
        if myLock.acquire():			# lock for sharing ser object
          myLock.locked()
          sUSB.write(msg)
          sUSB.flush()
          myLock.release()
          break
        else:
          print ("block1")
          
      # if str[0] == b"imu" :
      #   print ("imu")
		######################################################################
		#       Parsing a IMU Message for drawing direction
		#		You should implement your messase pasring code (use a simple protocal)
		#		(e.g) first string "imu" as the target message
		######################################################################        
        # xPntP1 = xPnt1
        # yPntP1 = yPnt1
        # h = 250
        # k = 200
        # r = 130
        # t = math.radians(int(str[1])) # z axis value
        # xPnt1 = (int)(r*math.cos(t) + h)
        # yPnt1 = (int)(r*math.sin(t) + k)
        # img_imu = cv2.line(img_imu,(250,200),(xPntP1,yPntP1),(0,0,0),5)			
        # img_imu = cv2.line(img_imu,(250,200),(xPnt1,yPnt1),(255,0,0),5)			
        # cv2.imshow('imu',img_imu)
        # cv2.moveWindow("imu", 865,0 )
        # cv2.waitKey(3)
                
   except ValueError:
      pass
   time.sleep(0.01)
    
    
def thread_usbserial(event):
  while not event.is_set():
    try:
      msg = sUSB.readline() # Read the newest output from the Arduino
      print (msg.decode())
      # tmpStr = msg.split( )
      while True:	
		######################################################################
		#        Message Forwarding from bluetooth to usbserial(Arduino)
		######################################################################
        if myLock.acquire():			# lock for sharing ser object
          myLock.locked()
          sBluetooth.write(msg)
          sBluetooth.flush()
          myLock.release()
          break
        else:
          print ("block1")
          
    #   if str[0] == b"joy" :
    #     print ("joymessage")
		# ######################################################################
		# #       Parsing a Joystic Message for extra control steps
		# #		You should implement your messase pasring code (use a simple protocal)
		# #		(e.g) first string "joy" as the target message
		# ######################################################################        
                
    except ValueError:
      pass
    time.sleep(0.01)

        
event = threading.Event()
y = threading.Thread(target=thread_usbserial,   args=(event,))
y.start()	
time.sleep(3)
x = threading.Thread(target=thread_bluetooth,   args=(event,))
x.start()	

        
while True:
	try:
		time.sleep(0.1)
	except KeyboardInterrupt:
		event.set()  # inform the child thread that it should exit
		x.join()
		y.join()
		break

cv2.destroyAllWindows()

sys.stderr.write('\n--- exit ---\n')



