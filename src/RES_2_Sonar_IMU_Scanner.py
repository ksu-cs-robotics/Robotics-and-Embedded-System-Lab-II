#!/usr/bin/env python
#
# Redirect data from a TCP/IP connection to a serial port and vice versa.
#
# (C) 2002-2016 Chris Liechti <cliechti@gmx.net>
#
# SPDX-License-Identifier:    BSD-3-Clause

##########################################################################
# Project           : RES_Class, Sonar Scan with IMU for object hit-map generation 
#
# Program name      : RES_2_Sonar_IMU_Scanner.py
#
# Author            : Jong-Hoon Kim
#
# Date created      : 04/09/2022
#
# Purpose           : 1) Read a message ('/n' is the end of the message frame) from serial port 
#			2) split the message based on ' ' (space)
#			3) check message type and draw the postion 
#			4) IMU message will provide scanning direction
#           5) Sonar will generate hit points
#           -- Fixed radio r will be manualy maintained when the scanner get rotated for scanning						
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


from asyncio.constants import SENDFILE_FALLBACK_READBUFFER_SIZE
from time import sleep
from tracemalloc import StatisticDiff
import serial
import math
import numpy as np
import cv2


img = np.zeros((800,1000,3), np.uint8)
img = cv2.line(img,(500,0),(500,800),(255,255,0),1)
img = cv2.line(img,(0,400),(1000,400),(255,255,0),1)
img1 = img.copy()
img2 = img.copy()
img3 = img.copy()
img4 = img.copy()

# img_imu = np.zeros((500,500,3), np.uint8)
# img_imu = cv2.circle(img_imu,(255,255),(210),(255,255,0),5)
img_imu = np.zeros((800,800,3), np.uint8)
img_imu = cv2.circle(img_imu,(400,400),(350),(255,255,0),1)

img_sonar = np.zeros((800,800,3), np.uint8)
img_sonar = cv2.circle(img_sonar,(400,400),(350),(255,255,0),1)


#ser = serial.Serial('/dev/ttyACM0', 115200) # Establish the connection on a specific port
ser = serial.Serial('COM23', 115200) # Establish the connection on a specific port
counter = 32 # Below 32 everything in ASCII is gibberish

ser.reset_input_buffer()	# remove any previous data
ser.reset_output_buffer()	# remove any previous data

xPnt = xPntP = yPnt = yPntP = 0
xPnt4 = xPntP4 = yPnt4 = yPntP4 = 0
xPnt5 = xPntP5 = yPnt5 = yPntP5 = 0
xPnt6 = xPntP6 = yPnt6 = yPntP6 = 0

sonar_x = sonar_y= 0

centerX = centerY= 400
radiousR = 350
imuAngleZ = 0

def init():
    print("Start...")
    

def getSonarLocatoin():
    global centerX, centerY, radiousR, imuAngle
    print (str(centerX) +" : "+ str(centerY) +" : "+ str(radiousR)  +" : "+  str(imuAngle))

def rotate(origin, point, angle):
    """
    Rotate a point counterclockwise by a given angle around a given origin.    
    The angle should be given in radians.
    """
    ox, oy = origin
    px, py = point

    qx = ox + math.cos(angle) * (px - ox) - math.sin(angle) * (py - oy)
    qy = oy + math.sin(angle) * (px - ox) + math.cos(angle) * (py - oy)
    return qx, qy    

def transate(origin, point):
    ox, oy = origin
    px, py = point
    qx = ox + px    
    qy = oy + py 
    return qx, qy

def drawUpdate():
    
    global xPnt4, xPntP4, yPnt4, yPntP4
    global xPnt5, xPntP5, yPnt5, yPntP5 
    global xPnt6, xPntP6, yPnt6, yPntP6 
    global img_imu, img_sonar, imuAngleZ
    while True:
        msg = ser.readline() # Read the newest output from the Arduino
        
        try:
            #print msg
            str1 = msg.decode("utf-8").split( )
#            print (str)
#            print  (len(str1))
#            print  (str1)
            if len(str1) == 4 and str1[0] == "imu" :
#                print (str1)
                h = 400
                k = 400
                r = 350
                imuAngleZ = math.radians(int(str1[3])) 
                xPntP5 = xPnt5
                yPntP5 = yPnt5
                # xPnt5 = (int)(r*math.cos(t) + h)
                # yPnt5 = (int)(r*math.sin(t) + k)
                xPnt4 = (int)(r*math.cos(imuAngleZ) )
                yPnt4 = (int)(r*math.sin(imuAngleZ) )
                screenCenter = transate( (0,0), (h,k ))
                xPnt5, yPnt5 = transate( screenCenter, (xPnt4,yPnt4 ))

                # img_imu = cv2.line(img_imu,(250,250),(xPntP5,yPntP5),(0,0,0),5)			
                # img_imu = cv2.line(img_imu,(250,250),(xPnt5,yPnt5),(255,0,0),5)	
                img_sonar = cv2.circle(img_sonar,(xPntP5,yPntP5),(3),(0,0,0),3)			
                img_sonar = cv2.circle(img_sonar,(xPnt5,yPnt5),(3),(0,0,255),3)	
                img_sonar = cv2.circle(img_sonar,(h,k),(r),(255,255,0),1)  
#                print (str(str1[3])+" = "+ str(xPntP5) + " = "+ str(yPntP5) + " = "+ str(xPnt5) + " = "+ str(yPnt5))
                #getSonarLocatoin()		
                cv2.imshow('sonar',img_sonar)
#                cv2.imshow('imu',img_imu)
                cv2.waitKey(3)

            elif len(str1) == 4 and str1[0] == "sonar" :
                print (str1)
#                h = xPnt5  # 400
#                k = yPnt5  # 400
                h = 400
                k = 400
                r0 = (int(str1[2]))
                r1 = (int(str1[3]))
                t = math.radians(int(str1[1])) 
                xPntP6 = xPnt6
                yPntP6 = yPnt6
                xPntP6 = (int)(r0*math.cos(t) )
                yPntP6 = (int)(r0*math.sin(t) )
                xPnt6 = (int)(r1*math.cos(t) )
                yPnt6 = (int)(r1*math.sin(t) )
                xPnt6, yPnt6= rotate((0,0), (xPnt6,yPnt6), r1)
                xPnt6, yPnt6= rotate((0,0), (xPnt6,yPnt6), imuAngleZ)
                screenCenter = transate( (0,0), (h,k ))
                screenCenter = transate( screenCenter, (xPnt4,yPnt4))
                xPnt6, yPnt6 = transate( screenCenter, (xPnt6, yPnt6) )

#                img_sonar = cv2.line(img_sonar,(400,400),(xPntP6,yPntP6),(0,0,0),5)			
#                img_sonar = cv2.line(img_sonar,(400,400),(xPnt6,yPnt6),(255,0,0),5)
                img_sonar = cv2.line(img_sonar,(xPnt5,yPnt5),( int(xPnt6), int(yPnt6)),(0,255,255),2)
#                img_sonar = cv2.line(img_sonar,(xPnt4,yPnt4),(xPntP6,yPntP6),(0,0,255),5)
                print (str(xPnt5)+" = "+str(yPnt5)+" = "+str(int(xPnt6))+" = "+str(int(yPnt6)))
#                img_sonar = cv2.line(img_sonar,(int(xPnt5),int(yPnt5)),(int(xPnt6),int(yPnt6)),(255,0,0),5)
#                cv2.circle(image, centerOfCircle, radius, color, thickness)	

#                img_sonar = cv2.circle(img_sonar,(xPntP6,yPntP6),(5),(255,0,255),5)			
#                img_sonar = cv2.circle(img_sonar,(400,400),(10),(0,0,255),10)			
                cv2.imshow('sonar',img_sonar)
                cv2.waitKey(3)


        except ValueError:
            pass


if __name__ == '__main__':
    init()
    drawUpdate()
