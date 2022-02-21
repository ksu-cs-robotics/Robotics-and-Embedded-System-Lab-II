#!/usr/bin/env python
#
# Redirect data from a TCP/IP connection to a serial port and vice versa.
#
# (C) 2002-2016 Chris Liechti <cliechti@gmx.net>
#
# SPDX-License-Identifier:    BSD-3-Clause

##########################################################################
# Project           : RES_Class, Serial to Serial fowarder 
#
# Program name      : Serial2Serial.py
#
# Author            : Jong-Hoon Kim
#
# Date created      : 04/22/2020
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

sUSB = serial.Serial('/dev/ttyUSB0', 115200) # Establish the connection on a specific port
sBluetooth = serial.Serial('/dev/ttyACM0', 115200) # Establish the connection on a specific port

xPnt = xPntP = yPnt = yPntP = 0
xPnt1 = xPntP1 = yPnt1 = yPntP1 = 0
xPnt2 = xPntP2 = yPnt2 = yPntP2 = 0

myLock = threading.Lock()

def thread_bluetooth(event):
	global myLock, img_imu, img_sonar
	global xPnt1, xPntP1, yPnt1,  yPntP1
	global xPnt2, xPntP2, yPnt2,  yPntP2
	while not event.is_set():
		try:
			msg = sBluetooth.readline() # Read the newest output from the Arduino
			print msg
			str = msg.split( )
			if len(str) > 0:
				print str			
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
						print "block1"
				######################################################################
				#        Drawnig IMU values from bluetooth (Robot)
				######################################################################
				if str[0] == "imu" :
					print "imu"
					xPntP1 = xPnt1
					yPntP1 = yPnt1
					h = 250
					k = 200
					r = 130
					t = math.radians(int(str[1])) # z axis value
					xPnt1 = (int)(r*math.cos(t) + h)
					yPnt1 = (int)(r*math.sin(t) + k)
					img_imu = cv2.line(img_imu,(250,200),(xPntP1,yPntP1),(0,0,0),5)			
					img_imu = cv2.line(img_imu,(250,200),(xPnt1,yPnt1),(255,0,0),5)			
					cv2.imshow('imu',img_imu)
					cv2.moveWindow("imu", 865,0 )
					cv2.waitKey(3)
				######################################################################
				#        Drawnig Sonar values from bluetooth (Robot)
				######################################################################
				elif str[0] == "sonar" :
					print "sonar"
					h = 250
					k = 10
					t = math.radians(int(str[1])) 	#  angle 
					r0 = int(str[2])			# previous distance	
					r1 = int(str[3])			# now distance	
					xPntP2 = (int)(r0*math.cos(t) + h)
					yPntP2 = (int)(r0*math.sin(t) + k)
					xPnt2 = (int)(r1*math.cos(t) + h)
					yPnt2 = (int)(r1*math.sin(t) + k)
					img_sonar = cv2.line(img_sonar,(250,10),(xPntP2,yPntP2),(0,0,0),5)			
					img_sonar = cv2.line(img_sonar,(250,10),(xPnt2,yPnt2),(255,0,0),5)			
					cv2.imshow('sonar',img_sonar)
					cv2.moveWindow("sonar", 865,430)
					cv2.waitKey(3)	

		except ValueError:
   			pass
		time.sleep(0.01)

def thread_usbserial(event):
	global myLock
	global xPnt, xPntP, yPnt,  yPntP
	global img
	while not event.is_set():
		try:
			msg = sUSB.readline() # Read the newest output from the Arduino
			str = msg.split( )
			if len(str) > 0:
				print str
				while True:	
					######################################################################
					#        Message Forwarding from usbserial(Arduino remote controller) to bluetooth
					######################################################################
					if myLock.acquire():			# lock for sharing ser object
						myLock.locked()
						sBluetooth.write(msg)
						sBluetooth.flush()
						myLock.release()
						break
					else:
						print "block2"
				######################################################################
				#        Drawnig Joystic values from USB serial (Arduino remote controller)
				######################################################################
				if (str[0] == "Joy"):
					xPntP = xPnt
					yPntP = yPnt
					xPnt = int(str[1])
					yPnt = int(str[2])
					#img = cv2.circle(img, (xPntP,yPntP), 20, (0, 0, 0), 2) 
					img = cv2.circle(img, (xPnt,yPnt), 20, (255, 0, 0), 2) 
					cv2.imshow('image',img)
					cv2.waitKey(3)
		except ValueError:
			print "####"
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



