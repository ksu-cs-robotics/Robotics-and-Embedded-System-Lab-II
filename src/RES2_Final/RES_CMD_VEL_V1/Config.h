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


#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x

#define MESSAGE_BUFFER_SIZE   128

#define LINEAR    0
#define ANGULAR   1


#define PIN_WHEEL_ENCODER_CLK_LEFT        2          // need to be changed = interrupt        
#define PIN_WHEEL_ENCODER_DT_LEFT         4          // need to be changed = interrupt       
#define PIN_WHEEL_ENCODER_CLK_RIGHT       3           // need to be changed = interrupt  
#define PIN_WHEEL_ENCODER_DT_RIGHT        5           // need to be changed = interrupt

#define PIN_SERVO                   6

#define PIN_MOTOR_PWM_LEFT    10
#define PIN_MOTOR_IN0_LEFT    11
#define PIN_MOTOR_IN1_LEFT    12

#define PIN_MOTOR_PWM_RIGHT    9
#define PIN_MOTOR_IN0_RIGHT    8
#define PIN_MOTOR_IN1_RIGHT    7

#define LEFT  0
#define RIGHT 1
#define PREV  0
#define NOW   1



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
