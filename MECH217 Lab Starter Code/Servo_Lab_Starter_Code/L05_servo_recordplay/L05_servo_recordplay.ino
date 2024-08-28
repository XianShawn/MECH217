// Example code for recording and playing back servo motion with a 
// analog feedback servo modified slightly by RWS 2017-11-09, Adding:
//      - Serial communication speed set to 115200
//      - button pins set to 8 and 10 for easy jumper activation
//      - documentation printing lines to prompt for setup
//      - printed time stamps from start of record / playback
//      - prints estimated record time based on SAMPLE_DELAY
// http://www.adafruit.com/products/1404


#include <Servo.h>
#include <EEPROM.h>

#define CALIB_MAX 512
#define CALIB_MIN 100
#define SAMPLE_DELAY 15 // in ms, 50ms seems good, takes about 26 seconds of recording

uint8_t recordButtonPin = 10;
uint8_t playButtonPin = 8;
uint8_t servoPin = 9;
uint8_t feedbackPin = A0;
uint8_t ledPin = 13;

Servo myServo;  
  
void setup() {
  Serial.begin(115200);
  pinMode(recordButtonPin, INPUT_PULLUP);
  digitalWrite(recordButtonPin, HIGH);
  pinMode(playButtonPin, INPUT_PULLUP);
  digitalWrite(playButtonPin, HIGH);
  pinMode(ledPin, OUTPUT);
  
  Serial.println("\n\nServo RecordPlay\n");
  Serial.println("Connect Servo power (red) to +5 and black to ground.");
  Serial.print("Connect Servo control (orange) on pin ");
  Serial.println(servoPin);
  Serial.println("Connect buttons or just use a jumper to record and play back from EEPROM.");
  Serial.print("To record a sequence of motions, pull to ground and then release pin ");
  Serial.println(recordButtonPin);
  Serial.println("After releasing, move the servo by hand until the recording time runs out.");
  Serial.print("To play back the recording, pull to ground and then release pin ");
  Serial.println(playButtonPin);
  Serial.print("Each recording or playback session will last about ");
  Serial.print(SAMPLE_DELAY * 26.15 / 50.,0);
  Serial.println(" seconds.\n");
}

void loop() {
 if (! digitalRead(recordButtonPin)) {
   delay(10);
   // wait for released
   while (! digitalRead(recordButtonPin));
   delay(20);
   // OK released!
   recordServo(servoPin, feedbackPin, recordButtonPin);
 }
 
  if (! digitalRead(playButtonPin)) {
   delay(10);
   // wait for released
   while (! digitalRead(playButtonPin));
   delay(20);
   // OK released!
   playServo(servoPin, playButtonPin);
 }
}

void playServo(uint8_t servoPin, uint8_t buttonPin) {
  uint16_t addr = 0;
  Serial.println("Playing");
  unsigned long start = micros();
  myServo.attach(servoPin);
  while (digitalRead(buttonPin)) {    
    uint8_t x = EEPROM.read(addr);
    Serial.print((micros()-start)/1000000.,4); Serial.print(" s Read EE: "); Serial.print(x);
    if (x == 255) break;
    // map to 0-180 degrees
    x = map(x, 0, 254, 0, 180);
    Serial.print(" -> "); Serial.println(x);
    myServo.write(x);
    delayMicroseconds(SAMPLE_DELAY*1000L);
    addr++;
    if (addr == 512) break;
  }
  Serial.println("Done");
  myServo.detach();
  delay(250);  
}

void recordServo(uint8_t servoPin, uint8_t analogPin, uint8_t buttonPin) {
  uint16_t addr = 0;
  Serial.println("Recording");
  digitalWrite(ledPin, HIGH);
  unsigned long start = micros();
  pinMode(analogPin, INPUT); 
  while (digitalRead(buttonPin)) {
     uint16_t a = analogRead(analogPin);
     Serial.print((micros()-start)/1000000.,4);Serial.print(" s Read analog: "); Serial.print(a);
     if (a < CALIB_MIN) a = CALIB_MIN;
     if (a > CALIB_MAX) a = CALIB_MAX;
     a = map(a, CALIB_MIN, CALIB_MAX, 0, 254);
     Serial.print(" -> "); Serial.println(a);
     EEPROM.write(addr, a);
     addr++;
     if (addr == 512) break;
     delayMicroseconds(SAMPLE_DELAY*1000L);
  }
  if (addr != 512) EEPROM.write(addr, 255);

  digitalWrite(ledPin, LOW);

  Serial.println("Done");
  delay(250);
}
