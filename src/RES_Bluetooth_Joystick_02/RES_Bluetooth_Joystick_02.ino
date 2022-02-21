/**********************************************************************;
* Project           : RES_Class, Communication, Custom Joystick with three control message 
*
* Program name      : RES_Bluetooth_Joystick_02.ino
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
*   This code will handle three control message with simple message frame
*     -- the message frame has three parts                      
*        1) header (1byte) + three byte (three messages) + tailer (1byte)
*                       
***********************************************************************/


// Slave Addr (controller) 18:E4:400006
// Master Addr (Robot)  98D3:32:F59EA7

#define PIN_JOY_X   A0
#define PIN_JOY_Y   A1
#define PIN_JOY_SW   3

#define COM_JOY_UPDATE_INTERVAL   30

uint8_t joy_sw =0;
uint8_t joy_x = 0;
uint8_t joy_y = 0;

uint8_t buf [5]= {0x00,0x00,0x00,0x00,0xFF};
long joy_sending_time = 0;;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);     //USB serial to PC
  Serial3.begin(115200);    // Bluetooth
  Serial.println("Controller Started");
  pinMode(PIN_JOY_SW, INPUT); digitalWrite(PIN_JOY_SW, HIGH);
  joy_sending_time = millis() + COM_JOY_UPDATE_INTERVAL;
  
}



void loop() {
  // put your main code here, to run repeatedly:

//  Serial.println(digitalRead(PIN_JOY_SW));
  sensing();
  thinking();
  acting();
}


void thinking(){
  buf[1] = joy_x;
  buf[2] = joy_y;
  buf[3] = joy_sw;
}


void acting(){
  if (millis() > joy_sending_time){
    Serial3.write(buf[0]);
    Serial3.write(buf[1]);
    Serial3.write(buf[2]);
    Serial3.write(buf[3]);
    Serial3.write(buf[4]);
    joy_sending_time = millis() + COM_JOY_UPDATE_INTERVAL;
    Serial.print(buf[1]);Serial.print(" ");
    Serial.print(buf[2]);Serial.print(" ");
    Serial.println(buf[3]);
  }
}

void sensing(){
  joy_x = map( analogRead(PIN_JOY_X), 0, 1024, 1, 180);
  joy_y = map( analogRead(PIN_JOY_Y), 0, 1024, 1, 180);
  joy_sw = map( digitalRead(PIN_JOY_SW), 0, 1, 1, 2);
 
}
