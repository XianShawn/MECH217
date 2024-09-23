// This sketch will contain the core code for your work in the course.
// Make sure you keep an up to date copy, and correct any problems revealed 
// in your assignments. This code will be the basis for some test answers.

// If you get an error here, check the previous tabs for mismatched { } and ( )
// that spilled over from the code you wrote in the main tab or the get tab!
#include <RTClib.h>           // Because Itsy Bitsy M0 needs it for printf("%8.3f...

// Global Variables are available throughout the code and don't lose their values like locals
unsigned long timeLast = 0;  // the last time we went through the loop, microseconds
unsigned long timeNow = micros();      // the time we started this loop, microseconds
unsigned long dt = timeNow - timeLast; // the time difference since last loop, microseconds

#define N_ANALOG     6        // total number of analog pins to allow for
#define BUTTON_PIN  12        // the pin with the button
#define LED_PIN     13        // the pin with the built-in LED
#define LOW_PIN 10            // so you can put the button between 10 and 12
#define HI_PIN 2
#define vRef 3.300  // this voltage value should be adjusted to match your own hardware
#define rRef 10000  // this resistance value should be adjusted to match your own hardware

double AtomV = vRef / 65.535;     // convert 16 bit unsigned from ADC to mV 
double AtoV  = vRef / 65535.;     // convert 16 bit unsigned from ADC to V 

void setup() {
  Serial.begin(115200);     // set the serial port speed
  while(!Serial && millis() < 5000);
  Serial.printf("Time [s], A3, A3 Smoothed\n"); // provide headings to name the outputs
  pinMode(12,INPUT_PULLUP);
  pinMode(13,OUTPUT);
  analogReadResolution(16);
}

void loop() {
  timeLast = timeNow;      // save the old value before getting the new one
  timeNow = micros();      // the time we started this loop, microseconds
  dt = timeNow - timeLast; // the time difference since last loop, microseconds
  
  //unsigned long timeNow2 = t2();         // just a test to show time passes
  //unsigned long timeNow3 = t3();         // move along
  float timeNowS = timeNow / 1000000.;   // time now in seconds, float so it will have decimals

  // put a slowly changing square wave on A0 for testing (connect A0 and A3 to read it)
  if(timeNow % 2000000 < 1000000) analogWrite(A0,1000); else analogWrite(A0,100);

  // get input and calculate values
  int d = digitalRead(12);            
  unsigned a = analogRead(A3);        
  byte led = digitalRead(13);         
  unsigned long tau = 1000000;  //time constant, we used microseconds
  // since dt usually less than tau, we must cast the calculation as (float) to avoid e.g., 3333/1000000 = 0
  float w = (float) dt/tau; // weight to give the latest reading of a in smoothing
  static float as = 0;      // exponentially smoothed version of a, static it stays
  if(w < 1.0) 
    as = w * a + (1-w) * as; // weighted average will smooth the data to reduce noise
  else as = a;               // use the latest value if it has been a long time, dt > tau
  if(led == 1) digitalWrite(13,0);
  else digitalWrite(13,1);

  Serial.printf("%8.3f, %5u, %9.3f\n", timeNowS, a, as);

  if(digitalRead(BUTTON_PIN)) delay(50);  // if the button is pushed, delay 50    
}
