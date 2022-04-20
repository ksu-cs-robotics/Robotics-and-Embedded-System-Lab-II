/*********************************************************************
*   Protocol description:  
*           this is a simple link frame protocoal with the below stucture and rules
*           1) Structure   {Preamble bits}{message type}{payload}{tailer}
*           2) max buffer length  (64 byte)
*           3) {Preamble bits} 3byte='<','@',' ' 
*           4) {message type} "odom", "cmd", "gen", and others
*           5) {payload} any string massage with ' '(space) delimiter 
*           6) {tailer} 3byte='#','>','\n'   ==> 
*           if you want to implement CRC on tailer, sendter need to add message length right after {Preamble bits} 
*           and then replace {tailer} with CRC. In addition, receiver need to check message length and then read  
*           rest of payroad & CRC bit based on the length informaiton for a message evaluation 
*                      
***********************************************************************/

#ifndef ATR_Protocol_h
#define ATR_Protocol_h

#define ODOM_MSG                         "odom"  // pose x, y, z, speed liner, angular
#define CMD_MSG                          "cmd"   // joy_x, joy_y, sw1, sw2, sw3, sw4(left), sw5(up), sw6(down), sw7 (left)
#define GENERAL_MSG                      "gen"


void evaluateCommand(char *argBuffer){
  String message(argBuffer);
  char* command = strtok(argBuffer, " ");
  int temp = 0;
  String type = "none";
  while (command != 0){
    if (temp == 1){
      if (String(command) == ODOM_MSG){
//          Serial.println("Odom message");
          Serial.println(message);
      }else if (String(command) == CMD_MSG){
          Serial.println("Command message");
      }else if (String(command) == GENERAL_MSG){
          Serial.println("Command message");
      }else{
          Serial.println("Unknown type");
      }
    }
    temp += 1;
    command = strtok(NULL, " ");
  }
}



#endif
