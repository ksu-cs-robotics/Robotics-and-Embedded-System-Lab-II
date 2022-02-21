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



//
///*******************************************************************************
//* Update the odometry
//*******************************************************************************/
//void updateOdometry(void)
//{
//  odom.header.frame_id = odom_header_frame_id;
//  odom.child_frame_id  = odom_child_frame_id;
//
//  odom.pose.pose.position.x = odom_pose[0];
//  odom.pose.pose.position.y = odom_pose[1];
//  odom.pose.pose.position.z = 0;
//  odom.pose.pose.orientation = tf::createQuaternionFromYaw(odom_pose[2]);
//
//  odom.twist.twist.linear.x  = odom_vel[0];
//  odom.twist.twist.angular.z = odom_vel[2];
//}
//
///*******************************************************************************
//* Update the joint states 
//*******************************************************************************/
//void updateJointStates(void)
//{
//  static float joint_states_pos[WHEEL_NUM] = {0.0, 0.0};
//  static float joint_states_vel[WHEEL_NUM] = {0.0, 0.0};
//  //static float joint_states_eff[WHEEL_NUM] = {0.0, 0.0};
//
//  joint_states_pos[LEFT]  = last_rad[LEFT];
//  joint_states_pos[RIGHT] = last_rad[RIGHT];
//
//  joint_states_vel[LEFT]  = last_velocity[LEFT];
//  joint_states_vel[RIGHT] = last_velocity[RIGHT];
//
//  joint_states.position = joint_states_pos;
//  joint_states.velocity = joint_states_vel;
//}
//
///*******************************************************************************
//* CalcUpdateulate the TF
//*******************************************************************************/
//void updateTF(geometry_msgs::TransformStamped& odom_tf)
//{
//  odom_tf.header = odom.header;
//  odom_tf.child_frame_id = odom.child_frame_id;
//  odom_tf.transform.translation.x = odom.pose.pose.position.x;
//  odom_tf.transform.translation.y = odom.pose.pose.position.y;
//  odom_tf.transform.translation.z = odom.pose.pose.position.z;
//  odom_tf.transform.rotation      = odom.pose.pose.orientation;
//}
//
///*******************************************************************************
//* Update motor information
//*******************************************************************************/
//void updateMotorInfo(int32_t left_tick, int32_t right_tick)
//{
//  int32_t current_tick = 0;
//  static int32_t last_tick[WHEEL_NUM] = {0, 0};
//  
//  if (init_encoder)
//  {
//    for (int index = 0; index < WHEEL_NUM; index++)
//    {
//      last_diff_tick[index] = 0;
//      last_tick[index]      = 0;
//      last_rad[index]       = 0.0;
//
//      last_velocity[index]  = 0.0;
//    }  
//
//    last_tick[LEFT] = left_tick;
//    last_tick[RIGHT] = right_tick;
//
//    init_encoder = false;
//    return;
//  }
//
//  current_tick = left_tick;
//
//  last_diff_tick[LEFT] = current_tick - last_tick[LEFT];
//  last_tick[LEFT]      = current_tick;
//  last_rad[LEFT]       += TICK2RAD * (double)last_diff_tick[LEFT];
//
//  current_tick = right_tick;
//
//  last_diff_tick[RIGHT] = current_tick - last_tick[RIGHT];
//  last_tick[RIGHT]      = current_tick;
//  last_rad[RIGHT]       += TICK2RAD * (double)last_diff_tick[RIGHT];
//}
//
///*******************************************************************************
//* Calculate the odometry
//*******************************************************************************/
//bool calcOdometry(double diff_time)
//{
//  float* orientation;
//  double wheel_l, wheel_r;      // rotation value of wheel [rad]
//  double delta_s, theta, delta_theta;
//  static double last_theta = 0.0;
//  double v, w;                  // v = translational velocity [m/s], w = rotational velocity [rad/s]
//  double step_time;
//
//  wheel_l = wheel_r = 0.0;
//  delta_s = delta_theta = theta = 0.0;
//  v = w = 0.0;
//  step_time = 0.0;
//
//  step_time = diff_time;
//
//  if (step_time == 0)
//    return false;
//
//  wheel_l = TICK2RAD * (double)last_diff_tick[LEFT];
//  wheel_r = TICK2RAD * (double)last_diff_tick[RIGHT];
//
//  if (isnan(wheel_l))
//    wheel_l = 0.0;
//
//  if (isnan(wheel_r))
//    wheel_r = 0.0;
//
//  delta_s     = WHEEL_RADIUS * (wheel_r + wheel_l) / 2.0;
//  // theta = WHEEL_RADIUS * (wheel_r - wheel_l) / WHEEL_SEPARATION;  
//  orientation = sensors.getOrientation();
//  theta       = atan2f(orientation[1]*orientation[2] + orientation[0]*orientation[3], 
//                0.5f - orientation[2]*orientation[2] - orientation[3]*orientation[3]);
//
//  delta_theta = theta - last_theta;
//
//  // compute odometric pose
//  odom_pose[0] += delta_s * cos(odom_pose[2] + (delta_theta / 2.0));
//  odom_pose[1] += delta_s * sin(odom_pose[2] + (delta_theta / 2.0));
//  odom_pose[2] += delta_theta;
//
//  // compute odometric instantaneouse velocity
//
//  v = delta_s / step_time;
//  w = delta_theta / step_time;
//
//  odom_vel[0] = v;
//  odom_vel[1] = 0.0;
//  odom_vel[2] = w;
//
//  last_velocity[LEFT]  = wheel_l / step_time;
//  last_velocity[RIGHT] = wheel_r / step_time;
//  last_theta = theta;
//
//  return true;
//}
