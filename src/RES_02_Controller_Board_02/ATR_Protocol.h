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
