// This version starts out with printf() functionality that is not
// available on the UNO and similar, but works on the Adafruit M0 boards.
#include "RTClib.h"   // RTClib by Adafruit needed for %f in printf()

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);              // set the serial port speed
  while(!Serial && millis() < 5000); // wait for the USB to wake up
  Serial.printf("\n\nHello World\n\n");
}

void loop() {
  // put your main code here, to run repeatedly:
  unsigned t = micros();
  Serial.printf("%9u us since start, %10.6f Hello Again\n",
                t, sin(t/10000000.));
  delay(500);   // try to avoid using delay()
}
