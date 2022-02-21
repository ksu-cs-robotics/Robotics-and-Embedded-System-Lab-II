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

img = np.zeros((1500,1024,3), np.uint8)
img2 = np.zeros((1500,1024,3), np.uint8)

ser = serial.Serial('/dev/ttyACM0', 115200) # Establish the connection on a specific port
counter = 32 # Below 32 everything in ASCII is gibberish

ser.reset_input_buffer()	# remove any previous data
ser.reset_output_buffer()	# remove any previous data

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
		#print len(str)
		if str[0] == "data":
			#print "OK"
			xPntP = xPnt
			yPntP = yPnt
			xPnt = int(str[1])
			#print xPnt
			#print "	"
			yPnt = int(float(str[2]))
			#print yPnt
	
			img = cv2.line(img,(xPntP,yPntP),(xPnt,yPnt),(255,0,0),1)
			cv2.imshow('image',img)
			cv2.waitKey(3)
		else:
			print "NO"
			cv2.waitKey(3)


	except ValueError:
   		pass
	


