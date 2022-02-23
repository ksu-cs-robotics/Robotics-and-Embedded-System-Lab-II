#ifndef LOCALIZATION_H_
#define LOCALIZATION_H_

#define ODOMETER_PER_ANGLE    9   // 9 degree per click ==> 40 clinks per one revolution 
#define ODOMETER_PER_DISTANCE    5.1   // 65 x 3.14 / 40 
#define WHEEL_SIZE            65  // 65 mm

extern int speedRobot; 
extern int angle;
extern int positionX; 
extern int positionY;
extern int speedR; 
extern int speedL; 



void updateOdometer();
void updateSpeedmeter();

#endif /* LOCALIZATION_H_ */
