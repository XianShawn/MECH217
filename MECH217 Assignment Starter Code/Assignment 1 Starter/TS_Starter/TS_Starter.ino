
void setup() {
  // put your setup code here, to run once:
  // Initialize the Serial port for writing output to the monitor or plotter...
  Serial.begin(115200);
  
  // ... and wait for it to be ready
  while(!Serial && micros() < 10000000);
  Serial.print("\n\nTS12345678\n\nt[s], mvOut, mvIn\n\n");
  
  // Set analog read resolution to 16 bits for best resolution
  analogReadResolution(16);
  
  m0dotStarOff();   // Turns off the bright LED in the middle of the board
}

void loop() {
  // put your main code here, to run repeatedly:
  // get the latest value from your function and assign it to a variable mvOut
  double mvOut = 0;   // the output from the time series function in millivolts
  mvOut = sqWave(1.5);
  mvOut = sinWave(1.5);
  mvOut = triWave(1.5);
  //mvOut = triSqSin(1.5);  // uncomment this one once your function works
  // the value of mvOut will be whatever came back from the last function called

  // write out to A0 so it provides a voltage corresponding to mvOut
  unsigned aOut = mvOut / 3300 * 1023;  // scale to 0-1023 for output
  analogWrite(A0,aOut);                 // write to A0 (wired to A3)

  // read the latest value in from A3 and convert it to millivolts as variable mvIn
  unsigned aIn = analogRead(A3);        // digital value from A3
  double mvIn = aIn * 3300. / 65535;     // scaled from 0-65535 to 0-3300 mV

  // print time in seconds, mvOut and mvIn to the Serial output 
  // so you can display them on the monitor or plotter
  Serial.print(micros()/1000000.,3);
  Serial.print(", ");
  Serial.print(mvOut);
  Serial.print(", ");
  Serial.println(mvIn);

  // delay about 20 milliseconds so you can see what's going on 
  // and get the full waveform on the plotter.
  delay(20);
}


