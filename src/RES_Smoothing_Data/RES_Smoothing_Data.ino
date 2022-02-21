/*##########################################################################
# Project           : RES_Class, Sonar Scan, Drawing Location/Path of the robot 
#
# Program name      : Smoothing.py
#
# Author            : Jong-Hoon Kim
#
# Date created      : 04/12/2020
#
# Purpose           : 1) Read sample dataset
#      2) apply noisy filtering 
#     3) send message to serial port to plot values 
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
# SimpleKalmanFilter class created by Denys Sene, January, 1, 2017. Released under MIT License.         
# SimpleKalmanFilter(e_mea, e_est, q);
# e_mea: Measurement Uncertainty - How much do we expect to our measurement vary
# e_est: Estimation Uncertainty - Can be initilized with the same value as e_mea since the kalman filter will adjust its value.
# q: Process Noise (Variance) - usually a small number between 0.001 and 1 - how fast your measurement moves. Recommended 0.01. Should be tunned to your needs.
##########################################################################*/
 
#include "RES_Smoothing_Data.h"
#include "SimpleKalmanFilter.h"
#include <TinyMPU6050.h>

const int numReadings = 10;



//SimpleKalmanFilter simpleKalmanFilter(2, 2, 0.001);
SimpleKalmanFilter simpleKalmanFilter(2, 2, 0.01);

// imu sensor reading
MPU6050 mpu (Wire);


int readings[numReadings];      // the readings from the input
int readIndex = 0;              // the index of the current reading
int total = 0;                  // the running total
int average = 0;                // the average

float prev = 0.0; float now = 0.0; float alpha = 0.99;  float beta = 0.01;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  Serial.println("Smoothing Data");

  for (int thisReading = 0; thisReading < numReadings; thisReading++) {
    readings[thisReading] = 0;
  }

  delay(1000);
  for(int i = 0; i < 1000; i++){
    Serial.print("orgData "); Serial.print(i); Serial.print(" ");
    Serial.println(noisy_data2[i]);
    delay(10);
  }

  for(int i = 0; i < 1000; i++){            // average onbly
    readings[readIndex] = noisy_data2[i];   // read from the sensor:    
    total = total + readings[readIndex];    // add the reading to the total:    
    readIndex = readIndex + 1;              // advance to the next position in the array:    
    if (readIndex >= numReadings) {         // if we're at the end of the array...      
      Serial.print("avrOnly  "); Serial.print(i); Serial.print(" ");   // avr-with-slidng-window
      average = total / numReadings;          // calculate the average:    
      Serial.println(average);                // send it to the computer as ASCII digits
      readIndex = 0;                        // ...wrap around to the beginning:
      total = 0;
    }    
    delay(10);
  }
      
  for (int thisReading = 0; thisReading < numReadings; thisReading++) {
    readings[thisReading] = 0;
  } 
  readIndex = 0;                        // ...wrap around to the beginning:
  total = 0;
  for(int i = 0; i < 1000; i++){
    Serial.print("avrWSW  "); Serial.print(i); Serial.print(" ");   // avr-with-slidng-window
    total = total - readings[readIndex];
    readings[readIndex] = noisy_data2[i];   // read from the sensor:    
    total = total + readings[readIndex];    // add the reading to the total:    
    readIndex = readIndex + 1;              // advance to the next position in the array:    
    if (readIndex >= numReadings) {         // if we're at the end of the array...      
      readIndex = 0;                        // ...wrap around to the beginning:
    }    
    average = total / numReadings;          // calculate the average:    
    Serial.println(average);                // send it to the computer as ASCII digits
    delay(10);
  }
  
  prev = noisy_data2[0];                    // initialization for quick start
  for(int i = 0; i < 1000; i++){
    Serial.print("formula "); Serial.print(i); Serial.print(" ");
    now = (noisy_data2[i] * 0.1) + (prev*0.9);
    Serial.println( (int)now);
    prev =  now ;
    delay(10);
  }

  // Kalman Filter
  for(int i = 0; i < 1000; i++){
    Serial.print("kalman "); Serial.print(i); Serial.print(" ");
    float estimated_value = simpleKalmanFilter.updateEstimate(noisy_data2[i]);
    Serial.println( (int)estimated_value);
    delay(10);
  }

  

}

void loop() {
  // put your main code here, to run repeatedly:

 
}
