#ifndef PROTOCOL_H_
#define PROTOCOL_H_

/************************************** ATR Lab Simple UGV Protocol *******************************************************/
// ATR_SUGV Serial Protocol 0 
#define ATR_VERSION              1

#define ATR_IDENT                0x41   // 'A' out message         ATR protocol version
#define ATR_STATUS               0x42   // 'B' out message         status data 
#define ATR_RAW_IMU              0x43   // 'C' out message         9 DOF
#define ATR_MOTOR                0x44   // 'D' out message         2 motors power
#define ATR_HEADING               0x45   // 'E' out message         Orientation Angle 

#define ATR_SET_AUTO             0x61   // 'a' in message          Auto/Manual 
#define ATR_SET_MOTOR            0x62   // 'b' in message       Manual Motor speed setting      
#define ATR_MSG_ECHO             0x63   // 'c' in message       message echo         

#define MESSAGE_BUFFER_SIZE   64

extern char msgBuffer[MESSAGE_BUFFER_SIZE]; 
extern int msgBufferPointer;

extern int mSpeedR ; // controll message from the remote
extern int mSpeedL ; // controll message from the remote

void evaluateCommand();
void updateCommand();
void init_buffer();

void statusCommand();
void setMotorPower();

#endif /* PROTOCOL_H_ */
