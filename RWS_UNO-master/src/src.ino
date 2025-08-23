/********************************************************************* 
 * Development and testing sketch for the library 
 *   Elements that are shown in examples may have been removed
 *   
 ********************************************************************/
#include "RWS_UNO.h"
#define VREF 3.29 // voltage measured from the board with a multimeter

RWS_UNO uno = RWS_UNO();
unsigned b[5000];

void setup()
{
  
  unsigned c[5000];
  uno.begin();
  Serial.print("\n\nRWS Arduino Extensions Testing\n\n");
  Serial.print(uno.bytesFree()); Serial.print(" bytes free in memory, approx.\n\n");
//  Serial.print(uno.setTimeZone(-5)); Serial.print(" is being set as the time zone\n");
//  Serial.print(uno.getTimeZone()); Serial.print(" is the current time zone setting\n\n");

  uno.setVReg(VREF);
  uno.setVRef(2.2);
  uno.setVRef(VREF);
  Serial.print("Voltages for Regulator: ");
  PL(uno.getVReg(), 3);
  Serial.print("         ADC Reference: ");
  PL(uno.getVRef(), 3);
  Serial.print("               Battery: ");
  PL(uno.getVBat(), 3);
  Serial.print("               Battery: ");
  Serial.print(uno.getBat());
  PL("%");

}

void loop()
{

  Serial.print("          Analog on A0: ");
  PL(analogRead(A0));
  Serial.print("         Voltage on A0: ");
  PL(uno.getV(A0), 3);
  Serial.print("               Battery: ");
  PL(uno.getVBat(), 3);
  Serial.print("               Battery: ");
  Serial.print(uno.getBat());
  PL("%");
  Serial.print(ADC_RESOLUTION); PL(" bit ADC Resolution");

  uint32_t timeNow = micros();
  // take care of business

  uno.i2cScan();
  
  delay(6234);           // wait for next loop
}
