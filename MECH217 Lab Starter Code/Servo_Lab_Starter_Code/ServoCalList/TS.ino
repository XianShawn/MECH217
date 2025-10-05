double sqWave(double freq){
  // Return a square wave at frequency freq 
  // with amplitude almost covering the full 0 to 3300 mV range
  unsigned per = (1/freq) * 1000000;      // period of waveform [us]
  unsigned t = micros();                  // time right now [us]
  if(micros() % per < per / 2) return 100;// low in first half period
  else return 3200;                       // otherwise high
}

double sinWave(double freq){
  // Return a square wave at frequency freq 
  // with amplitude almost covering the full 0 to 3300 mV range
  unsigned per = (1/freq) * 1000000;      // period of waveform [us]
  double perD = per;                      // a double precision version...
  unsigned t = micros();                  // time right now [us]
  return 1600. + 1100. * sin((t % per) / perD * 2 * 3.14159);
}

double triWave(double freq){
  // Return a triangle wave at frequency freq 
  // with amplitude almost covering the full 0 to 3300 mV range
  unsigned per = (1/freq) * 1000000;      // period of waveform [us]
  double perD = per;                      // a double precision version...
  unsigned t = micros();                  // time right now [us]
  return 200. + 2800. * (t % per) / perD;
}

double triSqSin(double freq){   // return a signal level in millivolts
  double retVal = 1600; // Replace this code with your triSqSin function
  return retVal; 
}

