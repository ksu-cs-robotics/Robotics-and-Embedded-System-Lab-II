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
#define PIN_JOY_SW_1                 53            
#define PIN_JOY_SW_2                 A11            
#define PIN_JOY_SW_3                 A10
#define PIN_JOY_SW_UP                61
#define PIN_JOY_SW_LEFT              59
#define PIN_JOY_SW_RIGHT             58
#define PIN_JOY_SW_DOWN              60


#define PIN_MOTOR_IN0_RIGHT          7
#define PIN_MOTOR_IN1_RIGHT          8
#define PIN_MOTOR_PWM_RIGHT          9
#define PIN_MOTOR_PWM_LEFT          10
#define PIN_MOTOR_IN0_LEFT          11
#define PIN_MOTOR_IN1_LEFT           12
            

#define INTERVAL_JOYSTICK            100000     //0.1  
#define INTERVAL_POSE               10000     //0.01  
#define INTERVAL_LED_UPDATE            1000      //0.001
#define INTERVAL_MESSAGE_CHECK         1000       //0.001
#define INTERVAL_DISCOVERY_UPDATE    100000      //0.1
#define INTERVAL_MOTOR_SPEED    100000      //0.1
#define INTERVAL_REPORT         200000      //0.2


#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x

#define MESSAGE_BUFFER_SIZE   128

#define LINEAR    0
#define ANGULAR   1
#define LEFT      0
#define RIGHT      1



bool ogmap[SCREEN_WIDTH][SCREEN_HEIGHT]; 

int motor_speed[2] = {0,0};   // current left/right motor speed {range -255 ~ + 255}
float cmd_vel[2] = {0.0 , 0.0};
float scanned_point[2] = {0.0 , 0.0};
float pose[3] = {0.0 , 0.0, 0.0};
int joystick[2] = {0,0};    // joystick status


int msgBufferPointer_Serial0 = 0;
char msgBuffer_Serial0[MESSAGE_BUFFER_SIZE]; 
//int msgBufferPointer_Serial1 = 0;
//char msgBuffer_Serial1[MESSAGE_BUFFER_SIZE]; 
//int msgBufferPointer_Serial2 = 0;
//char msgBuffer_Serial2[MESSAGE_BUFFER_SIZE]; 
//int msgBufferPointer_Serial3 = 0;
//char msgBuffer_Serial3[MESSAGE_BUFFER_SIZE]; 
int msgBufferPointer_mySerial = 0;
char msgBuffer_mySerial[MESSAGE_BUFFER_SIZE]; 



#endif
