#!/usr/bin/env python
#
# Redirect data from a TCP/IP connection to a serial port and vice versa.
#
# (C) 2002-2016 Chris Liechti <cliechti@gmx.net>
#
# SPDX-License-Identifier:    BSD-3-Clause

##########################################################################
# Project           : RES_Class, Sonar Scan, Drawing Location/Path of the robot 
#
# Program name      : Split.py
#
# Author            : Jong-Hoon Kim
#
# Date created      : 04/07/2020
#
# Purpose           : 1) Read a message ('/n' is the end of the message frame) from serial port 
#			2) split the message based on ' ' (space)
#			3) check message type and draw the postion 
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

img = np.zeros((512,512,3), np.uint8)
img2 = np.zeros((512,512,3), np.uint8)

ser = serial.Serial('/dev/ttyUSB0', 115200) # Establish the connection on a specific port
counter = 32 # Below 32 everything in ASCII is gibberish

xPnt = 0
xPntP = 0

yPnt = 0
yPntP = 0

while True:
	msg = ser.readline() # Read the newest output from the Arduino
	
	try:
		#print msg
		str = msg.split( )
		print str
		print len(str)
		if str[0] == 'S':
			print "OK"
			xPntP = xPnt
			yPntP = yPnt
			xPnt = int(str[1])
			yPnt = int(str[2])
			img = cv2.circle(img, (xPntP,yPntP), 20, (0, 0, 0), 2) 
			img = cv2.circle(img, (xPnt,yPnt), 20, (255, 0, 0), 2) 
			cv2.imshow('image',img)
			cv2.waitKey(3)
		else:
			print "NO"
			img2 = cv2.line(img2,(256,256),(xPnt,yPnt),(255,0,0),5)
			cv2.imshow('image2',img2)
			cv2.waitKey(3)

#   		yPnt = int(msg)
#		img = cv2.line(img,(xPnt,256),(xPnt,yPnt),(255,0,0),5)
#		cv2.imshow('image',img)
#		cv2.waitKey(3)

		#cv2.destroyAllWindows()
#		sleep(0.1) # Delay for one tenth of a second
#		xPnt +=1
#		if xPnt > 512: 
#			xPnt = 0
#			img = np.zeros((512,512,3), np.uint8)

	except ValueError:
   		pass
	

#     counter +=1
#     ser.write(str(chr(counter))) # Convert the decimal number to ASCII then send it to the Arduino

