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
# Date created      : 11/29/2022
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


sUSB = serial.Serial('COM16', 9600) # Establish the connection on a specific port
sBluetooth = serial.Serial('COM7', 9600) # Establish the connection on a specific port


myLock = threading.Lock()

def thread_bluetooth(event):
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
