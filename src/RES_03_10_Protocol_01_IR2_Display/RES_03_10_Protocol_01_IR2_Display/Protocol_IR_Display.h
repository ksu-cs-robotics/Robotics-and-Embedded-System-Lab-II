#ifndef PROTOCOL_H_
#define PROTOCOL_H_             1

#define MSG_GET_ECHO                 0x31   // '1' in message       message echo         
#define MSG_EVALUATE_BINARY          0x32   //'2' in message     
#define MESSAGE_BUFFER_SIZE          64


extern char msgBuffer[MESSAGE_BUFFER_SIZE]; //declare a buffer of size 64
extern int msgBufferPointer; // declare a varibale to point the current buffer position
extern char receiveChar; //to hold the character which will be pressed from the IR Remote

typedef struct{
  unsigned long value;
  char charRemote;
}remoteMessageMap; 

extern remoteMessageMap remoteMessagemapArray[22]; //This array is used to map the key of the IR Remote to a specific character

typedef struct{
  bool checkValidchar;
  char validChar;
}getValidchar;

extern getValidchar vChar; // this vChar is used to check whether the readRemotechar() returns a valid character or not with the actual character

void evaluateCommand();
void updateCommand();
void init_buffer();
void init_receiver();
void init_pin_mode();
void message_mapping();
void printTmpchar();
void turnOff();

void statusCommand();
void setMotorPower();
void setJoyInput();
void displayDigit(char receiveChar);

#endif /* PROTOCOL_H_ */
