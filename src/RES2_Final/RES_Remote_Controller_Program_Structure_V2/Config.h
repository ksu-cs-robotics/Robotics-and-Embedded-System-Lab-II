/**********************************************************************;
* Project           : RES_Class, Localization with Dead Reckoning 
*
* Program name      : Config.h
*
* Author            : Jong-Hoon Kim
*
* Date created      : 04/25/2023
*
* Purpose           : Localization of a Simple Rescue Robot 
*
* Revision History  :
*
* Date        Author      Ref    Revision (Date in MMDDYYYY format) 
* MMDDYYYY    name      1  v-xx   revision note. 
*
*********************************************************************/

#ifndef Config_h
#define Config_h


#define PIN_JOY_X                    A8            
#define PIN_JOY_Y                    A9            
#define PIN_JOY_SW                    A0            

#define INTERVAL_JOYSTICK            100000     //0.01  
#define INTERVAL_LED_UPDATE            1000      //0.001
#define INTERVAL_MESSAGE_CHECK         1000       //0.001
#define INTERVAL_DISCOVERY_UPDATE    100000      //0.1


#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x

#define MESSAGE_BUFFER_SIZE   128

#define LINEAR    0
#define ANGULAR   1





int motor_speed[2] = {0,0};   // current left/right motor speed {range -255 ~ + 255}
float cmd_vel[2] = {0.0 , 0.0};
float scanned_point[2] = {0.0 , 0.0};
float pose[3] = {0.0 , 0.0, 0.0};


int msgBufferPointer_Serial0 = 0;
char msgBuffer_Serial0[MESSAGE_BUFFER_SIZE]; 
int msgBufferPointer_Serial1 = 0;
char msgBuffer_Serial1[MESSAGE_BUFFER_SIZE]; 
int msgBufferPointer_mySerial = 0;
char msgBuffer_mySerial[MESSAGE_BUFFER_SIZE]; 



#endif
