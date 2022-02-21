/**********************************************************************;
* Project           : RES_Class, Communication, Custom Joystick 
*
* Program name      : RES_Bluetooth_Joystick.ino
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

// Slave Addr (controller) 18:E4:400006
// Master Addr (Robot)  98D3:32:F59EA7

#define PIN_JOY_X   A0
#define PIN_JOY_Y   A1
#define PIN_JOY_SW   3


int joy_sw =0;
int joy_x = 0;
int joy_y = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);     //USB serial to PC
  Serial3.begin(115200);    // Bluetooth
  Serial.println("Started");
  pinMode(PIN_JOY_SW, INPUT); digitalWrite(PIN_JOY_SW, HIGH);
}


void loop() {
  // put your main code here, to run repeatedly:

//  Serial.println(digitalRead(PIN_JOY_SW));

  joy_x = map( analogRead(PIN_JOY_X), 0, 1024, 1, 180);
  Serial.println(joy_x);
  Serial3.write(joy_x);
  delay(30);
}
