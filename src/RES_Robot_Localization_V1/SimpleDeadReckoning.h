/**********************************************************************;
* Project           : RES_Class, Localization with Dead Reckoning 
*
* Program name      : SimpleDeadReckoning.h
*
* Author            : Jong-Hoon Kim
*
* Date created      : 04/15/2020
*
* Purpose           : Localization of a Simple Rescue Robot 
*
* Revision History  :
*
* Date        Author      Ref    Revision (Date in MMDDYYYY format) 
* MMDDYYYY    name      1  v-xx   revision note. 
*
*********************************************************************/

#ifndef SimpleDeadReckoning_h
#define SimpleDeadReckoning_h

class SimpleDeadReckoning
{

public:


  SimpleDeadReckoning();
  SimpleDeadReckoning(float argEncoderValuePer360Rotation, float argWheelRadius, float argDistanceBetweenTwoWheels);
  SimpleDeadReckoning(float argEncoderValuePer360Rotation, float argWheelRadius, float argDistanceBetweenTwoWheels, int argDistanceUnit); // 0: Metric = cm (defalut) 1: Imperial = inch
  
  void updateOdometry(void);  
  void updateLocation(float argLeftEncoder, float argRightEncoder, float argTheta);  
  float getTheta(void);
  float getLeftOdom(void);
  float getRightOdom(void);
  float getXLocation(void);
  float getYLocation(void);
  void setXLocation(float argX);
  void setYLocation(float argY);
  long  getCheckTime(void);
  long  getLastCheckTime(void);

private:
  float _eValue = 122.0;
  float _wDistance = 15.5;
  float _wRadius = 3.39;
  int   _unit = 0;
  float _theta = 0.0;
  long _leftOdom = 0;
  long _rightOdom = 0;
  float _xLocation = 0.0;
  float _yLocation = 0.0;
  long  _checkTime = 0;

  float _pTheta = 0.0;
  long _pLeftOdom = 0;
  long _pRightOdom = 0;
  float _pXLocation = 0.0;
  float _pYLocation = 0.0;
  long  _pCheckTime = 0;


};

#endif
