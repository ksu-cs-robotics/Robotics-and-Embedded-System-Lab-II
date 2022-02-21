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
# Program name      : Smoothing.py
#
# Author            : Jong-Hoon Kim
#
# Date created      : 04/12/2020
#
# Purpose           : 1) Read a message ('/n' is the end of the message frame) from serial port 
#			2) split the message based on ' ' (space)
#			3) check message type and draw the postion 
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

img = np.zeros((800,1000,3), np.uint8)
#img = cv2.line(img,(500,0),(500,800),(255,255,0),1)
#img = cv2.line(img,(0,400),(1000,400),(255,255,0),1)
img1 = img.copy()
img2 = img.copy()
img3 = img.copy()
img4 = img.copy()

ser = serial.Serial('/dev/ttyACM0', 115200) # Establish the connection on a specific port
counter = 32 # Below 32 everything in ASCII is gibberish

ser.reset_input_buffer()	# remove any previous data
ser.reset_output_buffer()	# remove any previous data

xPnt = xPntP = yPnt = yPntP = 0
xPnt1 = xPntP1 = yPnt1 = yPntP1 = 0
xPnt2 = xPntP2 = yPnt2 = yPntP2 = 0
xPnt3 = xPntP3 = yPnt3 = yPntP3 = 0
xPnt4 = xPntP4 = yPnt4 = yPntP4 = 0
xPnt5 = xPntP5 = 500
yPnt5 = yPntP5 = 400




while True:
	msg = ser.readline() # Read the newest output from the Arduino
	
	try:
		#print msg
		str = msg.split( )
		print str
		#print len(str)
		if str[0] == "orgData":
			print "orgData"
			xPntP = xPnt
			yPntP = yPnt
			xPnt = int(str[1])
			yPnt = int(str[2])
			img = cv2.line(img,(xPntP,yPntP),(xPnt,yPnt),(255,0,0),1)
			cv2.imshow('smoothing',img)
			cv2.imshow('orgData',img)
			cv2.waitKey(3)
		elif str[0] == "avrOnly" :
			print "avrOnly"
			xPntP1 = xPnt1
			yPntP1 = yPnt1
			xPnt1 = int(str[1])
			yPnt1 = int(str[2])
			img = cv2.line(img,(xPntP1,yPntP1),(xPnt1,yPnt1),(0,255,0),1)
			img1 = cv2.line(img1,(xPntP1,yPntP1),(xPnt1,yPnt1),(0,255,0),1)
			cv2.imshow('smoothing',img)
			cv2.imshow('avrOnly',img1)
			cv2.waitKey(3)

		elif str[0] == "avrWSW" :
			print "avrWSW"
			xPntP2 = xPnt2
			yPntP2 = yPnt2
			xPnt2 = int(str[1])
			yPnt2 = int(str[2])
			img = cv2.line(img,(xPntP2,yPntP2),(xPnt2,yPnt2),(0,0,255),1)
			img2 = cv2.line(img2,(xPntP2,yPntP2),(xPnt2,yPnt2),(0,0,255),1)
			cv2.imshow('smoothing',img)
			cv2.imshow('avrWSW',img2)
			cv2.waitKey(3)

		elif str[0] == "formula" :
			print "formula"
			xPntP3 = xPnt3
			yPntP3 = yPnt3
			xPnt3 = int(str[1])
			yPnt3 = int(str[2])
			img = cv2.line(img,(xPntP3,yPntP3),(xPnt3,yPnt3),(255,0,255),1)
			img3 = cv2.line(img3,(xPntP3,yPntP3),(xPnt3,yPnt3),(255,0,255),1)
			cv2.imshow('smoothing',img)
			cv2.imshow('formula',img3)
			cv2.waitKey(3)

		elif str[0] == "kalman" :
			print "kalman"
			xPntP4 = xPnt4
			yPntP4 = yPnt4
			xPnt4 = int(str[1])
			yPnt4 = int(str[2])
			img = cv2.line(img,(xPntP4,yPntP4),(xPnt4,yPnt4),(255,255,0),1)
			img4 = cv2.line(img4,(xPntP4,yPntP4),(xPnt4,yPnt4),(255,255,0),1)
			cv2.imshow('smoothing',img)
			cv2.imshow('kalman',img4)
			cv2.waitKey(3)

		elif str[0] == "path" :
			print "path"
			xPntP5 = xPnt5
			yPntP5 = yPnt5
			xPnt5 = int(float(str[1]))
			yPnt5 = int(float(str[2]))
			img = cv2.line(img,(xPntP5,yPntP5),(xPnt5,yPnt5),(255,255,0),2)
#			img4 = cv2.line(img4,(xPntP5,yPntP5),(xPnt5,yPnt5),(255,255,0),1)
			cv2.imshow('path',img)
#			cv2.imshow('kalman',img4)
			cv2.waitKey(3)


	except ValueError:
   		pass
	


