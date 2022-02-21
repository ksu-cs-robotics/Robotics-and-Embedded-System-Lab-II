/**********************************************************************;
* Project           : RES_Class, Localization with Dead Reckoning 
*
* Program name      : SimpleDeadReckoning.cpp
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

#include "SimpleDeadReckoning.h"
#include <Arduino.h>
#include <math.h>

SimpleDeadReckoning::SimpleDeadReckoning(float argEncoderValuePer360Rotation, float argWheelRadius, float argDistanceBetweenTwoWheels){
  _eValue = (360.0/argEncoderValuePer360Rotation) * (3.141592 / 180.0);
  _wRadius = argWheelRadius;
  _wDistance = argDistanceBetweenTwoWheels;
  _unit = 0;
}

SimpleDeadReckoning::SimpleDeadReckoning(float argEncoderValuePer360Rotation, float argWheelRadius, float argDistanceBetweenTwoWheels, int argDistanceUnit){ // 0: Metric = cm (defalut) 1: Imperial = inch
  _eValue = (360.0/argEncoderValuePer360Rotation) * (3.141592 / 180.0);
  _wRadius = argWheelRadius;
  _wDistance = argDistanceBetweenTwoWheels;
  _unit = argDistanceUnit;
}

SimpleDeadReckoning::SimpleDeadReckoning(){   // defalut
  _eValue = (360.0/122.0) * (3.141592 / 180.0) ;    // click per 360/122 degree
  _wRadius = 3.39;  //  2*r = 6.78;
  _wDistance = 15.5;  // 15.5 cm between two wheels
  _unit = 0;        // Metric (cm) unit 
}

  
void SimpleDeadReckoning::updateLocation(float argLeftEncoder, float argRightEncoder, float argTheta){  
  _pLeftOdom = _leftOdom ;
  _pRightOdom = _rightOdom;
  _pTheta = _theta;  
  _pCheckTime = _checkTime;
  
  _leftOdom = argLeftEncoder ;
  _rightOdom = argRightEncoder;
  _theta = argTheta * (3.141592 / 180);
  _checkTime = millis();


//  float vL = (((_leftOdom - _pLeftOdom)*_eValue) / ((_checkTime - _pCheckTime)/1000.0)) * (3.141592 / 180);
//  float vR = (((_rightOdom - _pRightOdom )*_eValue)  / ((_checkTime - _pCheckTime)/1000.0)) *(3.141592 / 180) ;
//  float wK = (vL - vR) / _wDistance;                              // encoder based delta theta calucation
//  float deltaTheta = wK * ((_checkTime - _pCheckTime)/1000.0) ;   // encoder based delta theta calucation

  float vL = ((_leftOdom - _pLeftOdom)*_eValue) ;
  float vR = ((_rightOdom - _pRightOdom )*_eValue) ;
  float vK = (vL + vR) / 2;
  float deltaTheta = _theta - _pTheta ;
  float deltaS = vK * _wRadius;

  
  _pXLocation = _xLocation;
  _pYLocation = _yLocation; 
  _xLocation = _pXLocation + (deltaS * cos( _pTheta + (deltaTheta/2.0) )) ;
  _yLocation = _pYLocation + (deltaS * sin( _pTheta + (deltaTheta/2.0) )) ;



}



void SimpleDeadReckoning::updateOdometry(void){  
  _theta += 1 ;
}

float SimpleDeadReckoning::getTheta(void){
  return _theta;
}
float SimpleDeadReckoning::getLeftOdom(void){
  return _leftOdom;
}
float SimpleDeadReckoning::getRightOdom(void){
  return _rightOdom;
}
float SimpleDeadReckoning::getXLocation(void){
  return _xLocation;
}
float SimpleDeadReckoning::getYLocation(void){
  return _yLocation;
}
void SimpleDeadReckoning::setXLocation(float argX){
  _xLocation = argX ;
}
void SimpleDeadReckoning::setYLocation(float argY){
  _yLocation = argY;
}
long SimpleDeadReckoning::getCheckTime(void){
  return _checkTime;
}
long SimpleDeadReckoning::getLastCheckTime(void){
  return _pCheckTime;
}
