#!/usr/bin/env python
#
# Redirect data from a TCP/IP connection to a serial port and vice versa.
#
# (C) 2002-2016 Chris Liechti <cliechti@gmx.net>
#
# SPDX-License-Identifier:    BSD-3-Clause

##########################################################################
# Project           : RES_Class, Sonar Scan, Drawing Scanned Sonar Data 
#
# Program name      : RES_Rotary_Encoder_Basic.ino
#
# Author            : Jong-Hoon Kim
#
# Date created      : 03/29/2020
#
# Purpose           : How to make a rotray encoder.
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

ser = serial.Serial('/dev/ttyACM0', 115200) # Establish the connection on a specific port
counter = 32 # Below 32 everything in ASCII is gibberish

xPnt = 0
yPnt = 0
while True:
	msg = ser.readline() # Read the newest output from the Arduino
	print msg
	try:
   		yPnt = int(msg)
		img = cv2.line(img,(xPnt,256),(xPnt,yPnt),(255,0,0),5)
		cv2.imshow('image',img)
		cv2.waitKey(3)
		#cv2.destroyAllWindows()
#		sleep(0.1) # Delay for one tenth of a second
		xPnt +=1
		if xPnt > 512: 
			xPnt = 0
			img = np.zeros((512,512,3), np.uint8)

	except ValueError:
   		pass
	

#     counter +=1
#     ser.write(str(chr(counter))) # Convert the decimal number to ASCII then send it to the Arduino

