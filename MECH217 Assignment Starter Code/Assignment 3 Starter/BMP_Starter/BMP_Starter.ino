//import Wire library for serial communicaiton between SDA SCL and the BMP280
#include <Wire.h>
//Import general sensor library
#include <Adafruit_Sensor.h>
//Import BMP280 library so we can use its functions
#include <Adafruit_BMP280.h>

Adafruit_BMP280 bmp;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  while(!Serial);

  if(!bmp.begin(0x76)){
    Serial.println("Could not find a valid BMP sensor");
    while(1);
  }

  // Default settings from datasheet
    bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,  /* Operating Mode. */   
                Adafruit_BMP280::SAMPLING_X2,     /* Temp Oversampling */
                Adafruit_BMP280::SAMPLING_X16,    /*Pressure Oversampling */
                Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                Adafruit_BMP280::STANDBY_MS_500); /* Standby Time */

  analogReadResolution(16); 

}

void loop() {
  // put your main code here, to run repeatedly:
  
    Serial.print(F("Temperature = "));
    Serial.print(bmp.readTemperature());
    Serial.println(" *C");

    Serial.print(F("Pressure = "));
    Serial.print(bmp.readPressure());
    Serial.println(" Pa");

    Serial.println();
    delay(2000);
}

float getThermC(      unsigned THERMISTORPIN = A5, 
                      double BCOEFFICIENT = 3950, 
                      double THERMISTORNOMINAL = 10000, 
                      double SERIESRESISTOR = 10000,  
                      double TEMPERATURENOMINAL = 25){
  float sum = 0;
  for(int i = 0; i < 100; i++){
    sum += analogRead(A5);
  }
  float analog = sum/100.;
  
  // convert the value to resistance
  float frac = 65536 / analog - 1;
  float res = SERIESRESISTOR / frac;

  float steinhart;
  steinhart = res / THERMISTORNOMINAL;          // (R/Ro)
  steinhart = log(steinhart);                   // ln(R/Ro)
  steinhart /= BCOEFFICIENT;                    // 1/B * ln(R/Ro)
  steinhart += 1.0 / (TEMPERATURENOMINAL + 273.15); // + (1/To)
  steinhart = 1.0 / steinhart;                  // Invert
  steinhart -= 273.15;                          // convert to C

  return steinhart;
}
