#ifndef Config_h
#define Config_h


#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x

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

#define LINEAR  0
#define ANGULAR 1


#define INTERVAL_RIDAR            3
#define INTERVAL_OLED             100
#define INTERVAL_DEBUG            200
#define INTERVAL_MOTOR            20

#define MIN_RIDAR_ANGLE      30
#define MID_RIDAR_ANGLE      90
#define MAX_RIDAR_ANGLE      150
#define STRIDE_RIDAR_ANGLE     1


#define MESSAGE_BUFFER_SIZE              64

char msgBuffer[MESSAGE_BUFFER_SIZE]; 
int msgBufferPointer = 0;


int motorSpeed[2] = {0,0};   // current left/right motor speed {range -255 ~ + 255}
float cmd_vel[2] = {0.0, 0.0};    // LINEAR 0, ANGULAR 1
float odom_pose[3] = {0.0, 0.0, 0.0};
float odom_vel[2] = {0.0, 0.0};
int controlMode = 0;         // manual control = 0, line following = 1,

int motorTestCount = 0;



#endif
