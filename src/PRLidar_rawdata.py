#!/usr/bin/env python
#
# PRLidar Raw data reading.
#
# (C) 2002-2016 Chris Liechti <cliechti@gmx.net>
#
# SPDX-License-Identifier:    BSD-3-Clause

##########################################################################
# Project           : RES_Class, Sonar Scan, Drawing Location/Path of the robot 
#
# Program name      : PRLidar_rawdata.py
#
# Author            : Jong-Hoon Kim
#
# Date created      : 04/29/2022
#
# Purpose           : 1) Read a message from serial port (PRLadir is connected)
#			2) split the message based on prlidar protocal
#
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
import math
import numpy as np
import cv2


ser = serial.Serial('COM41', 115200) # Establish the connection on a specific port
counter = 32 # Below 32 everything in ASCII is gibberish

ser.reset_input_buffer()	# remove any previous data
ser.reset_output_buffer()	# remove any previous data

xPnt = xPntP = yPnt = yPntP = 0
xPnt1 = xPntP1 = yPnt1 = yPntP1 = 0
xPnt2 = xPntP2 = yPnt2 = yPntP2 = 0
xPnt3 = xPntP3 = yPnt3 = yPntP3 = 0
xPnt4 = xPntP4 = yPnt4 = yPntP4 = 0
xPnt5 = xPntP5 = yPnt5 = yPntP5 = 0
xPnt6 = xPntP6 = yPnt6 = yPntP6 = 0



while True:
	msg = ser.readline() # Read the newest output from the Arduino
	
	try:
		#print msg
		str = msg.split( )
		print (str)
		#print len(str)
		if str[0] == b"orgData":
			print (str[0])
			xPntP = xPnt
			yPntP = yPnt
			xPnt = int(str[1])
			yPnt = int(str[2])
			img = cv2.line(img,(xPntP,yPntP),(xPnt,yPnt),(255,0,0),1)
			cv2.imshow('smoothing',img)
			cv2.imshow('orgData',img)
			cv2.waitKey(3)
		elif str[0] == b"avrOnly" :
			print (str[0])
			xPntP1 = xPnt1
			yPntP1 = yPnt1
			xPnt1 = int(str[1])
			yPnt1 = int(str[2])
			img = cv2.line(img,(xPntP1,yPntP1),(xPnt1,yPnt1),(0,255,0),1)
			img1 = cv2.line(img1,(xPntP1,yPntP1),(xPnt1,yPnt1),(0,255,0),1)
			cv2.imshow('smoothing',img)
			cv2.imshow('avrOnly',img1)
			cv2.waitKey(3)

		elif str[0] == b"avrWSW" :
			print (str[0])
			xPntP2 = xPnt2
			yPntP2 = yPnt2
			xPnt2 = int(str[1])
			yPnt2 = int(str[2])
			img = cv2.line(img,(xPntP2,yPntP2),(xPnt2,yPnt2),(0,0,255),1)
			img2 = cv2.line(img2,(xPntP2,yPntP2),(xPnt2,yPnt2),(0,0,255),1)
			cv2.imshow('smoothing',img)
			cv2.imshow('avrWSW',img2)
			cv2.waitKey(3)

		elif str[0] == b"formula" :
			print (str[0])
			xPntP3 = xPnt3
			yPntP3 = yPnt3
			xPnt3 = int(str[1])
			yPnt3 = int(str[2])
			img = cv2.line(img,(xPntP3,yPntP3),(xPnt3,yPnt3),(255,0,255),1)
			img3 = cv2.line(img3,(xPntP3,yPntP3),(xPnt3,yPnt3),(255,0,255),1)
			cv2.imshow('smoothing',img)
			cv2.imshow('formula',img3)
			cv2.waitKey(3)

		elif str[0] == b"kalman" :
			print (str[0])
			xPntP4 = xPnt4
			yPntP4 = yPnt4
			xPnt4 = int(str[1])
			yPnt4 = int(str[2])
			img = cv2.line(img,(xPntP4,yPntP4),(xPnt4,yPnt4),(255,255,0),1)
			img4 = cv2.line(img4,(xPntP4,yPntP4),(xPnt4,yPnt4),(255,255,0),1)
			cv2.imshow('smoothing',img)
			cv2.imshow('kalman',img4)
			cv2.waitKey(3)


		elif str[0] == b"imu" :
			print (str[0])
			xPntP5 = xPnt5
			yPntP5 = yPnt5
			h = 250
			k = 250
			r = 200
			t = math.radians(int(str[1])) 
			xPnt5 = (int)(r*math.cos(t) + h);
			yPnt5 = (int)(r*math.sin(t) + k);
			img_imu = cv2.line(img_imu,(250,250),(xPntP5,yPntP5),(0,0,0),5)			
			img_imu = cv2.line(img_imu,(250,250),(xPnt5,yPnt5),(255,0,0),5)			
			cv2.imshow('imu',img_imu)
			cv2.waitKey(3)

		elif str[0] == b"sonar" :
			print (str[0])
			h = 400
			k = 400
			r0 = (int(str[2]))
			r1 = (int(str[3]))
			t = math.radians(int(str[1])) 
			xPntP6 = (int)(r0*math.cos(t) + h);
			yPntP6 = (int)(r0*math.sin(t) + k);
			xPnt6 = (int)(r1*math.cos(t) + h);
			yPnt6 = (int)(r1*math.sin(t) + k);
			img_sonar = cv2.line(img_sonar,(400,400),(xPntP6,yPntP6),(0,0,0),5)			
			img_sonar = cv2.line(img_sonar,(400,400),(xPnt6,yPnt6),(255,0,0),5)			
			cv2.imshow('sonar',img_sonar)
			cv2.waitKey(3)


	except ValueError:
   		pass
	


