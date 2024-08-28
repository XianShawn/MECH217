#include <Servo.h>
uint8_t servoPin = 9;     // make sure these match your wiring
uint8_t feedbackPin = A3;
uint8_t ledPin = 13;
unsigned long timeLastPrint = 0;
unsigned long timeNow = 0;

Servo myservo;            // create servo object to control a servo

void setup() {
  Serial.begin(115200);
  while(!Serial && micros() < 10000000);
  analogReadResolution(16);
  Serial.print("\n\nServoWatch\n\nanalogRead() values in mV as you move by hand: \n");
  Serial.print("Connect the output lead to A3 analog input.\nA3_mV\n");
}

void loop() {
  timeNow = micros();
  // read the feedback and print some diagnostic results here
  double a = analogRead(A3) / 65535. * 3300.; // value in millivolts
  if(timeNow - timeLastPrint > 20000 && timeNow < 30000000){
    Serial.println(a);
    timeLastPrint = timeNow;
  }
}
