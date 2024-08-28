// If you get an error here, check the previous tabs for mismatched { } and ( )
// that spilled over from the code you wrote in the main tab or the get tab!
void setup217Kit(){
  // Initialize the Serial port for writing output to the monitor or plotter...
  Serial.begin(115200);
  // Wait up to 10 seconds for it to be ready
  while(!Serial && millis() < 10000) delay(100);
  //uno.begin();

  // Setup some digital IO pins
  pinMode(LED_PIN, OUTPUT);           // the small LED beside the reset button
  pinMode(BUTTON_PIN, INPUT_PULLUP);  // the button on pin 12 and the 
  pinMode(LOW_PIN, OUTPUT);           // other side of the button to ground on pin 10
  digitalWrite(LOW_PIN, LOW);
  pinMode(HI_PIN, OUTPUT);            // set pin 2 high to support the 2020 kit configuration
  digitalWrite(HI_PIN, HIGH);
  pinMode(A2, OUTPUT);                // set pin A2 low to provide a ground for the pot
  digitalWrite(A2, LOW);

  #ifndef ARDUINO_ARCH_AVR            // Check at compile time that it's not an UNO!
    analogReadResolution(16);         // use the 16 bit resolution everywhere in MECH 217, except for UNO!
  #endif
  analogWrite(A0, 1023);              // make A0 a 3.3 V output for the pot until the code changes it
  //uno.i2cScan();                      // you may want to turn this on to see what's attached to I2C
}
