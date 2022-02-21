/**********************************************************************;
* Project           : RES_Class, Communication, Bluetooth Setup
*
* Program name      : RES_Bluetooth_Setup.ino
*
* Author            : Jong-Hoon Kim
*
* Date created      : 03/10/2020
*
* Purpose           : How to setup a bluetooth serial module (HC-05) as master and slave.
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
*
*   PIN mapping (wiring)
*     Bluetooh module       Arduion Mega
*       RX                    TX3
*       TX                    RX3       
*       GND                   GND
*       VCC                   +5V
*   
*   Bluetooth (HC-05) Setup 
*     1) Setup Mode initiation:Push and hold the button right above on EN (PIN) and then power up (connect to VCC and GND)
*       -- result : onboard LED will blink by one sec
*       
*     2) Upload this program and then open serial monitor 
*       -- setup [Both NL & CR] and [38400 baud]
*       
*     3) Type "AT" and enter
*       -- you should see "OK" message
*       
*     4) Type "AT+NAME=YOURNAME_MASTER or YOURNAME_SLAVE (depend on what role you want to assign on the bluetooth module
*       -- e.g. "AT+NAME=KIM_MASTER"  then you will see "OK"
*       -- for checking the updated result, type "AT+NAME", then you should see "+NAME:KIM_MASTER"  
*     5) Communication Speed Setup (115200 bps)
*       -- type "AT+UART=115200,0,0" 
*       
*     6) Setup Slave Bluetooth
*       1) Get Bluetooth Address for paring purpuse so you need to note the address
*       -- Type "AT+ADDR" and then you will see "+ADDR:XXXX:YY:ZZZZZZ", here "XXXX:YY:ZZZZZZ" is the address
*       2) setup slave role ( "AT+ROLE=0"), master role ("AT+ROLE=1")
*       -- e.g. slave node, Type "AT+ROLE=0"
*       -- after setup this role, it occurs automatic reboot the bluetooth module
*       -- so you need to unplug and do 1) setup mode initiation
*     7) Paring setup on Master bluetooth module only
*       -- Type "AT+CMODE=0", 0 means that it will bind only speicific address
*       -- Type "AT+BIND=XXXX,YY,ZZZZZZ", here XXXX,YY,ZZZZ is the address of your slave module
*       
*     Finally, unplug and power up without pushing the button (normal bluetooth bootup) 
*       -- after power up, it needs time to paring about 3 ~ 5 sec 
*       --    and then if it get paired, you will see the led blinking behavior
*       --    (blink twice with short time and then about 1 sec led off)
*              
*     Testing Instruction
*       -- If your two bluetooth modules (master and slave) get paired, you may open two "gtkterm"
*       -- and setup ports on each gtkterm for each bluetooth module
*       -- and then setup buad as 115200
*       -- and type any charector in each gtkterm, your typed charector will be appeared at the other gtkterm
*     
*********************************************************************/

/************************************************
 * If your Arduino has only one Serial port (such as Arduion Nano, micro, etc.)
 * then you can setup Software serial using below library
 * 
 *  #include <SoftwareSerial.h>
 *  SoftwareSerial mySerial(2, 3); // RX, TX
************************************************/

// My Master = 98D3:31:F5934E
// Kim_Slave = 98D3:31:F73A44



void setup() {
  // put your setup code here, to run once:
  Serial.begin(38400);    // after setup your bluetooth, you need to set 115200 bps "Serial.begin(115200);"
  Serial3.begin(38400);   // after setup your bluetooth, you need to set 115200 bps "Serial3.begin(115200);
//  mySerial.begin(38400);
  Serial.println("Started _KIM2");
}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial.available() > 0){
    char a = Serial.read();
    Serial3.print(a);
  }

  if (Serial3.available() > 0){
    char b = Serial3.read();
    Serial.print(b);
  }

/* for software serial
 *  
  if (Serial.available() > 0){
    char a = Serial.read();
    mySerial.print(a);
  }

  if (mySerial.available() > 0){
    char b = mySerial.read();
    Serial.print(b);
  }
*/


}
