void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("Serial Mega");
  Serial1.begin(57600);
  Serial2.begin(115200);
  Serial3.begin(115200);
}

long cTime, rTime, cnt = 0;

void loop() {
  cTime = millis();
  // put your main code here, to run repeatedly:
  if (Serial.available() > 0 ){
    byte tmp = Serial.read();
    Serial1.write(tmp);
    Serial2.write(tmp);
    Serial3.write(tmp);
  }
  if (Serial1.available() > 0 ){
    byte tmp = Serial1.read();
    Serial.write(tmp);
  }
  if (Serial2.available() > 0 ){
    byte tmp = Serial2.read();
    Serial.write(tmp);
  }
  if (Serial3.available() > 0 ){
    byte tmp = Serial3.read();
    Serial.write(tmp);
  }

  if (cTime > rTime){
    rTime = cTime + 150;
    cnt++;
    Serial.println(cnt);
    Serial1.print("<@,"); Serial1.print(cnt); Serial1.print(",200,200,"); Serial1.println(">"); 
  }
}
