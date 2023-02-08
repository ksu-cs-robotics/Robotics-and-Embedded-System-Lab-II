#ifndef Config_h
#define Config_h


#define PIN_IR_LEFT                 A0          // 
#define PIN_IR_RIGHT                A1          // 
#define PIN_ENCODER_LEFT_CLK        5          // need to be changed = interrupt        
#define PIN_ENCODER_LEFT_DT         3          // need to be changed = interrupt       
#define PIN_ENCODER_RIGHT_CLK       2           // need to be changed = interrupt  
#define PIN_ENCODER_RIGHT_DT        4           // need to be changed = interrupt
#define PIN_SONAR_PING              A3
#define PIN_SONAR_ECHO              A2
#define PIN_SERVO                   12
#define PIN_MOTOR_RIGHT_ENA         6
#define PIN_MOTOR_RIGHT_IN1         7
#define PIN_MOTOR_RIGHT_IN2         8
#define PIN_MOTOR_LEFT_ENB          11
#define PIN_MOTOR_LEFT_IN3          9
#define PIN_MOTOR_LEFT_IN4          10

#define LEFT  0
#define RIGHT 1
#define PREV  0
#define NOW   1
#define LINEAR  0
#define ANGULAR 1

#define INTERVAL_MOTOR_TEST       5000
#define INTERVAL_SONAR_TEST       500
#define INTERVAL_IMU_TEST         2000
#define INTERVAL_DEBUG_PRINT      2000

#define MIN_SONAR_ANGLE      30
#define MID_SONAR_ANGLE      90
#define MAX_SONAR_ANGLE      150
#define STRID_SONAR_ANGLE     5

#define ODOM_MSG                         "o"  // pose x, y, z, speed liner, angular
#define CMD_MSG                          "c"
#define GENERAL_MSG                      "g"
#define TORQUE_MSG                       "t"

#define MESSAGE_BUFFER_SIZE              64

char msgBuffer[MESSAGE_BUFFER_SIZE]; 
int msgBufferPointer = 0;






#endif
