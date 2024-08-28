// This blank Starter has all of the basics covered so you can focus on your code
// 2022-01-10 removed dotstar elements because the base M0 feather doesn't need it.
#include "globals.h" 

void setup() {
  // put your setup code here, to run once:
  setup217Kit();
  Serial.print("\n\nMECH 217 Blank Starter\n\n");
}

void loop() {
  // Update the global variables for the time in this loop
  timeLast = timeNow;      // the last time we went through the loop, microseconds
  timeNow = micros();      // the time we started this loop, microseconds
  dt = timeNow - timeLast; // the time difference since last loop, microseconds
  
  // put your main code here, to run repeatedly:
  sendA0(triWave(0.01));            // you probably want to delete this !!!
  Serial.print(analogRead(A3));   // you probably want to delete this !!!
  Serial.print("\n");

  // Leave the rest of this code in place, but comment out when you don't need it
//  Serial.printf("Average dt = %8.1f us, this loop %8.1f us, maximum %8.1f\n", 
//                  uno.dtAvg(), uno.dt(), uno.dtMax());
  delay(20);  // delay only to slow things down so you can see
}
