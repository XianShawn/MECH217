// 2021-04-14 Modified from Cal Starter to work better with M0 processors 
#include <Servo.h>
uint8_t servoPin = 9;     // make sure these match your wiring
uint8_t feedbackPin = A3;
uint8_t ledPin = 13;
unsigned long timeLastPrint = 0;
unsigned long timeNow = 0;

Servo myservo;            // create servo object to control a servo

// a list of angles with 999 to indicate the end of the list
int angles[] = {0,1,2,3,5,10,15,20,30,40,50,60,70,80,90,100,110,120,130,140,150,160,165,170,175,178,179,180,999};
float aValues[30]     = {0};  //make sure this is big enough if you add to the list!
float aValuesDown[30] = {0};  //make sure this is big enough if you add to the list!
float a10 = 0, a170 = 0;

double goAngle(int a, unsigned wait = 1000) {
  myservo.attach(servoPin);       // bring the servo under control of the microcontroller
  myservo.write(a);               // send it to the current angle
  delay(wait);                    // wait long enough for it to get there
  myservo.detach();               // stop driving the servo
  double val = 0;                 // get an average reading from the potentiometer
  for (int j = 0; j < 100; j++){
    val += analogRead(feedbackPin);
  }
  val /= 100;
  return val;
}

double getAngle(int n = 1) {
  double val = 0;                 // get an average reading from the potentiometer
  for (int j = 0; j < n; j++){
    val += analogRead(feedbackPin);
  }
  val /= n;

  // convert that reading into an angle based on a170 and a10
  double frac = (val - a10) / (a170-a10); // fractional distance between the two points
  double ang = 10 + frac * 160.;          // convert to an angle
  return ang;
}


void setup() {
  Serial.begin(115200);
  while(!Serial && micros() < 10000000);
  analogReadResolution(16);

  // Start by getting some calibration values near the ends of the scale.
  // Make the wait extra long to be sure there's time enough to get there.
  a10 = goAngle(10, 3000);
  Serial.print("The analogRead() value for  10 degrees was ");
  Serial.println(a10);
  a170 = goAngle(170, 3000);
  Serial.print("The analogRead() value for 170 degrees was ");
  Serial.println(a170);
  Serial.print("These two values will be used to convert readings to angles.\n\n");
  
  Serial.print("\n\nServoCalList\n\nAverage analogRead() values for various angles: \n");
  Serial.print("(Going up and then going down...)\n");
  // Step slowly through a series of angles
  int iMax = 0;
  for(int i = 0;  angles[i] <= 180; i++){ 
    iMax = i;
    aValues[i] = goAngle(angles[i]);
    Serial.print(angles[i]); Serial.print(", "); Serial.println(aValues[i]);
  }
  Serial.println();
  for(int i = iMax; i >= 0; i--){ 
    aValuesDown[i] = goAngle(angles[i]);
    Serial.print(angles[i]); Serial.print(", "); Serial.println(aValuesDown[i]);
  }
  myservo.attach(servoPin);       // bring the servo under control of the microcontroller
}

void loop() {
  timeNow = micros();
  // put some code here to calculate a desired position value
  // If sqWave() or triWave() is in the range 0 - 3300 mV, then (50 /1000.) will 
  // scale it to 0 to 165 degrees, which is close enough for practice.
  unsigned pos = 0;   // an angular position between 0 and 180 degrees
//  pos = triWave(0.05) * 50 / 1000.;     // a slow moving triangular wave 
  pos = triSqSin(1.0) * 50 / 1000.;  // custom time series function at 1 Hz
  
  // send the servo to that position
  myservo.write(pos);
  
  // read the feedback and print some diagnostic results here
  double a = getAngle();
  if(timeNow - timeLastPrint > 20000){
    Serial.print(timeNow/1000000.,3); Serial.print(", ");
    Serial.print(pos); Serial.print(", "); 
    Serial.println(a);
    timeLastPrint = timeNow;
  }
  
  while(millis() > 120000) delay(200);  // stop doing things after a couple of minutes
}
