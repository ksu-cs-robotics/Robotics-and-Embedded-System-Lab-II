/**********************************************************************;
* Project           : RES_Class, Communication, Custom one Servo and Two Motors Control 
*
* Program name      : RES_Bluetooth_mini_RoboCup_02.ino
*
* Author            : Jong-Hoon Kim
*
* Date created      : 03/10/2020
*
* Purpose           : How to make three control message communication .
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
*   This code will handle three control message
*                       
***********************************************************************/

#include <Servo.h>

// Slave Addr (controller) 18:E4:400006
// Master Addr (Robot)  98D3:32:F59EA7

#define PIN_SERVO   8


int servo_position = 0;
int motor_speed = 0;
int sw = 0;


uint8_t buf [5]= {0x00,0x00,0x00,0x00,0xFF};
int pointer = 0;


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

//  Serial.println(digitalRead(PIN_JOY_SW));
  sensing();
  thinking();
  acting();
}


void thinking(){
  updateMessage();
}


void acting(){
  
   myServo.write(servo_position);
}

void sensing(){

}


void updateMessage(){
    if (Serial3.available() > 0){
      
      byte msg = Serial3.read();
      if (msg == 0x00){
        pointer = 1;      
      }else if (msg == 0xFF){
        if (pointer == 4){
          servo_position = buf[1];
          motor_speed = buf[2];
          sw = buf[3];
          Serial.print(buf[1]);Serial.print(" ");
          Serial.print(buf[2]);Serial.print(" ");
          Serial.println(buf[3]);
        }
        pointer = 0;
      }else{
        if (pointer >= 4){
          pointer = 0;
        }else{
          buf[pointer] = msg;
          pointer++;
        }
      
      }
      
 

    }

}
