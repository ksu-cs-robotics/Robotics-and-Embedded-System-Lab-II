/**********************************************************************;
* Project           : RES_Class, Communication, Custom Servo Control 
*
* Program name      : RES_Bluetooth_mini_RoboCup.ino
*
* Author            : Jong-Hoon Kim
*
* Date created      : 03/10/2020
*
* Purpose           : How to make one control message communication .
*
* Revision History  :
*
* Date        Author      Ref    Revision (Date in MMDDYYYY format) 
* MMDDYYYY    name      1  v-xx   revision note. 
*
*********************************************************************/
/*********************************************************************
*   Instructional Note: Slave Addr (controller) e.g. 18:E4:400006
*                       Master Addr (Robot)     e.g. 98D3:32:F59EA7
*   This code will handle only one control message
*                       
***********************************************************************/

#include <Servo.h>

// Slave Addr (controller) 18:E4:400006
// Master Addr (Robot)  98D3:32:F59EA7

#define PIN_SERVO   8


int servo_position = 0;

Servo myServo;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);     //USB serial to PC
  Serial3.begin(115200);    // Bluetooth
  Serial.println("Robot Started");
  myServo.attach(PIN_SERVO);
}


void loop() {
  // put your main code here, to run repeatedly:

  if (Serial3.available() > 0){
    digitalWrite(13, !digitalRead(13));
    int a = Serial3.read();
    myServo.write(a);
    Serial.println(a);
  }

  
  delay(10);

}
