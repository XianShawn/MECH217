#include <Adafruit_BMP280.h>
#include <Wire.h> //import Wire library for serial communicaiton between SDA SCL and the BMP280
//#include <globals.h> //Import general sensor library
#include <Adafruit_Sensor.h> //Import BMP280 library so we can use its functions
#include <Adafruit_BMP280.h>

Adafruit_BMP280 bmp;

float getThermT(){
  float avg = 0;

  return avg;
}

//float getLightPercent(){
  
//  double light_percent = 0;
//  // converts the value into the range of 0 and 100. 
//  // A higher brightness is associated with a higher percentage
//  return light_percent;
//}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  while(!Serial);

  if(!bmp.begin(0x76)){
    Serial.println("Could not find a valid BMP sensor");
    while(1);
  }

  // Default BMP280 sensor settings from datasheet 
    bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,  /* Operating Mode. */   
                Adafruit_BMP280::SAMPLING_X2,     /* Temp Oversampling */
                Adafruit_BMP280::SAMPLING_X16,    /*Pressure Oversampling */
                Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                Adafruit_BMP280::STANDBY_MS_500); /* Standby Time */

  analogReadResolution(16); 
}

void loop() {
  // put your main code here, to run repeatedly:
  

     //average 100 pressure readings to get Patm 


    //print your data
    Serial.print(bmp.readTemperature());
    Serial.print(",");

    Serial.print(bmp.readPressure());
    Serial.println();

    delay(2000); // needs to be delected after you can print once every second
}

