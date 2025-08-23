#include "RWS_UNO.h"

RWS_UNO rws = RWS_UNO();

void setup()
{
  rws.begin(115200);

  Serial.print("\n\nRWS UNO I2C Scan\n\n");
  Serial.print("Results may be unpredictable if there are no I2C devices connected.\n\n");
}

 
void loop()
{
  rws.i2cScan();

  delay(3234);           // wait for next scan
}
