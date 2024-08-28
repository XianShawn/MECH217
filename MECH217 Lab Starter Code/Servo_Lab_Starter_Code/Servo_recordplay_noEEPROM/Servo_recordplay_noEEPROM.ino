// Example code for recording and playing back servo motion with a 
// analog feedback servo modified slightly by RWS 2017-11-09, Adding:
//      - Serial communication speed set to 115200
//      - button pins set to 8 and 10 for easy jumper activation on UNO
//      - documentation printing lines to prompt for setup
//      - printed time stamps from start of record / playback
//      - prints estimated record time based on SAMPLE_DELAY
// http://www.adafruit.com/products/1404
// Modified further by RWS 2019-12-23 to use an array in memory instead of EEPROM
// making it suitable for processors other than the UNO.
//      - set play button to 7 because there is no pin 8 on itsy bitsy
//      - switched feedback pin to A3 to match up with other activities 2021-03-24

#include <Servo.h>
//#include <EEPROM.h>

#define CALIB_MAX 750   // analogRead() value corresponding to 180 degrees
#define CALIB_MIN 150   // analogRead() value corresponding to 0 degrees
#define SAMPLE_DELAY 30 // in ms, 30ms seems good, takes about 15 seconds of recording
#define SAMPLES   512

uint8_t recordButtonPin = 10;
uint8_t playButtonPin = 7;
uint8_t servoPin = 9;
uint8_t feedbackPin = A3;
uint8_t ledPin = 13;
uint8_t eeprom[SAMPLES];  // an array to take the place of the EEPROM

Servo myServo;  
  
void setup() {
  Serial.begin(115200);
  while(!Serial && micros() < 10000000);
  pinMode(recordButtonPin, INPUT_PULLUP);
  digitalWrite(recordButtonPin, HIGH);
  pinMode(12, OUTPUT);  // ground for the pushbutton if connected 10 to 12
  digitalWrite(12, 0);

  pinMode(playButtonPin, INPUT_PULLUP);
  digitalWrite(playButtonPin, HIGH);
  pinMode(ledPin, OUTPUT);
  printInstructions();
}

void loop() {
 if (! digitalRead(recordButtonPin)) {
   delay(10);
   // wait for released
   while (! digitalRead(recordButtonPin));
   delay(20);
   // OK released!
   recordServo(servoPin, feedbackPin, recordButtonPin);
   printInstructions();
 }
 
 if (! digitalRead(playButtonPin)) {
   delay(10);
   // wait for released
   while (! digitalRead(playButtonPin));
   delay(20);
   // OK released!
   playServo(servoPin, playButtonPin);
   printInstructions();
 }
}

void playServo(uint8_t servoPin, uint8_t buttonPin) {
  uint16_t addr = 0;
  Serial.println("Playing");
  unsigned long start = micros();
  myServo.attach(servoPin);
  while (digitalRead(buttonPin)) {    
    uint8_t x = eeprom[addr];   // EEPROM.read(addr);
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
     eeprom[addr] = a;      //EEPROM.write(addr, a);
     addr++;
     if (addr == 512) break;
     delayMicroseconds(SAMPLE_DELAY*1000L);
  }
  if (addr != 512) eeprom[addr] = 255;    //EEPROM.write(addr, 255);

  digitalWrite(ledPin, LOW);

  Serial.println("Done");
  delay(250);
}

void printInstructions() {
  Serial.println("\n\nServo RecordPlay\n");
  Serial.println("Connect Servo power (red) to +5 (USB) and black to ground.");
  Serial.print("Connect Servo control (orange) on pin ");
  Serial.println(servoPin);
  Serial.println("Connect the white analog output line to A3 for feedback.");
  Serial.println("This voltage peaks at 2.5V, so it is OK for 3.3V boards like the Itsy Bitsy M0.");
  Serial.println("Connect buttons or just use a jumper to record and play back from EEPROM.\n");
  Serial.print("To record a sequence of motions, pull to ground and then release pin ");
  Serial.println(recordButtonPin);
  Serial.println("After releasing, move the servo by hand until the recording time runs out.\n");
  Serial.print("To play back the recording, pull to ground and then release pin ");
  Serial.println(playButtonPin);
  Serial.print("Each recording or playback session will last about ");
  Serial.print(SAMPLE_DELAY * 26.15 / 50.,0);
  Serial.println(" seconds.\n");
}
