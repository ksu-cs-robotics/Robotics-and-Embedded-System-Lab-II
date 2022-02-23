#ifndef SENSING_H_
#define SENSING_H_

#define SONAR_BUFFER_SIZE   32

extern int lineR; 
extern int lineL;
extern int sonar; 

extern volatile long odometerR; 
extern volatile long odometerL; 

extern char sonarBuffer [SONAR_BUFFER_SIZE];


int updateSonar();
void updateLineSensor();
void leftOdometer();
void rightOdometer();

#endif /* SENSING_H_ */
