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
  _eValue = (360.0/310.0) * (3.141592 / 180.0) ;    // click per 360/310degree
  _wRadius = 3.39;  //  2*r = 6.78;
  _wDistance = 15.5;  // 15.5 cm between two wheels
  _unit = 0;        // Metric (cm) unit 
}

void SimpleDeadReckoning::updateLocation(float argLeftEncoder, float argRightEncoder){  
  _pLeftOdom = _leftOdom ;
  _pRightOdom = _rightOdom;
  _pTheta = _theta;  
  _pCheckTime = _checkTime;
  
  _leftOdom = argLeftEncoder ;
  _rightOdom = argRightEncoder;
  _checkTime = millis();


//  float vL = (((_leftOdom - _pLeftOdom)*_eValue) / ((_checkTime - _pCheckTime)/1000.0)) * (3.141592 / 180);
//  float vR = (((_rightOdom - _pRightOdom )*_eValue)  / ((_checkTime - _pCheckTime)/1000.0)) *(3.141592 / 180) ;
//  float wK = (vL - vR) / _wDistance;                              // encoder based delta theta calucation
//  float deltaTheta = wK * ((_checkTime - _pCheckTime)/1000.0) ;   // encoder based delta theta calucation

  float vL = ((_leftOdom - _pLeftOdom)*_eValue) ;
  float vR = ((_rightOdom - _pRightOdom )*_eValue) ;
  float vK = (vL + vR) / 2;    // delta_s of roboty
  _theta = (vL + vR)/ _wDistance;  // * (3.141592 / 180);
  float deltaTheta = _theta - _pTheta ;
  float deltaS = vK * _wRadius;

  
  _pXLocation = _xLocation;
  _pYLocation = _yLocation; 

// Euler integration
//  _xLocation = _pXLocation + cos(_pTheta)*deltaS;
//  _yLocation = _pYLocation + sin(_pTheta)*deltaS;
  
// Runge-Kutta integration (2nd oder = mid-point)
  _xLocation = _pXLocation + (deltaS * cos( _pTheta + (deltaTheta/2.0) )) ;
  _yLocation = _pYLocation + (deltaS * sin( _pTheta + (deltaTheta/2.0) )) ;


// Runge-Kutta integration (4th order)- compute odometric pose using 4nd order Runge-Kutta integration
//  double K1 =  deltaTheta;  // Euler integration
//  double K2 =  (deltaTheta / 2.0);
//  double K3 =  (K2 / 2.0);
//  double K4 =  K3;
//  double Yn_plus_1 = _pTheta + ((K1+ (2*(K2)) + (2*(K3)) + K4) / 6);
//
//  _xLocation = _pXLocation + (deltaS * cos(Yn_plus_1));
//  _yLocation = _pYLocation  + (deltaS * sin(Yn_plus_1));


  // compute odometric pose using exact intergration
//  _xLocation = _pXLocation+ ((deltaS / deltaTheta) * ( cos(_theta) - cos(_pTheta)) );
//  _yLocation = _pYLocation + ((deltaS / deltaTheta)  * ( sin(_theta) - sin(_pTheta)) );



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
  float vK = (vL + vR) / 2;    // delta_s of roboty
  float deltaTheta = _theta - _pTheta ;
  float deltaS = vK * _wRadius;

  
  _pXLocation = _xLocation;
  _pYLocation = _yLocation; 

// Euler integration
//  _xLocation = _pXLocation + cos(_pTheta)*deltaS;
//  _yLocation = _pYLocation + sin(_pTheta)*deltaS;
  
// Runge-Kutta integration (2nd oder = mid-point)
  _xLocation = _pXLocation + (deltaS * cos( _pTheta + (deltaTheta/2.0) )) ;
  _yLocation = _pYLocation + (deltaS * sin( _pTheta + (deltaTheta/2.0) )) ;


// Runge-Kutta integration (4th order)- compute odometric pose using 4nd order Runge-Kutta integration
//  double K1 =  deltaTheta;  // Euler integration
//  double K2 =  (deltaTheta / 2.0);
//  double K3 =  (K2 / 2.0);
//  double K4 =  K3;
//  double Yn_plus_1 = _pTheta + ((K1+ (2*(K2)) + (2*(K3)) + K4) / 6);
//
//  _xLocation = _pXLocation + (deltaS * cos(Yn_plus_1));
//  _yLocation = _pYLocation  + (deltaS * sin(Yn_plus_1));


  // compute odometric pose using exact intergration
//  _xLocation = _pXLocation+ ((deltaS / deltaTheta) * ( cos(_theta) - cos(_pTheta)) );
//  _yLocation = _pYLocation + ((deltaS / deltaTheta)  * ( sin(_theta) - sin(_pTheta)) );


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
