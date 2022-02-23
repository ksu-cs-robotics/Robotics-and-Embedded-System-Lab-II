#ifndef SDR_SUGV_03_H_
#define SDR_SUGV_03_H_



#define SONAR_INTERVAL  10
#define LINE_INTERVAL  10
#define PLAN_INTERVAL  10
#define ODOMETOR_INTERVAL  20

#define MOTOR_SPEED_MAX      255

#define MOTOR_LEFT_ENABLE   13
#define MOTOR_LEFT_A        12
#define MOTOR_LEFT_B        11
#define MOTOR_RIGHT_A        10
#define MOTOR_RIGHT_B        9
#define MOTOR_RIGHT_ENABLE   8
#define LINE_RIGHT           7
#define LINE_LEFT            6
#define SONAR_ECHO           5
#define SONAR_TRIG           4


#define SONAR_BUFFER_SIZE   32
#define SONAR_STOP_DISTANCE   30

#define ODOMETER_PER_ANGLE    9   // 9 degree per click ==> 40 clinks per one revolution 
#define ODOMETER_PER_DISTANCE    5.1   // 65 x 3.14 / 40 
#define WHEEL_SIZE            65  // 65 mm

#define SONAR_INTERVAL  10
#define LINE_INTERVAL  10
#define PLAN_INTERVAL  10
#define ODOMETOR_INTERVAL  20



extern int stopFlag;
extern int autoFlag ;   // Auto or Manual Control

extern unsigned long sonarClock;
extern unsigned long lineClock;
extern unsigned long odometerClock;
extern unsigned long currentMillis;
extern unsigned long previousMillis;



#endif /* SDR_SUGV_03_H_ */
