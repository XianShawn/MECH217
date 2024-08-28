void sendA0(double mvOut){
  // Write out to A0 so it provides a voltage corresponding to mvOut
  // Doesn't work with UNO or other boards that have no DAC on A0
  unsigned res = 1023;  // DAC resolution is 10 bit for M0 (SAMD21)
#ifdef __SAMD51__       //      [compile time test for board type]
  res = 4095;           //    and 12 bit for M4 (SAMD51) processors
#endif
  analogWrite(A0, mvOut / 3300 * res); // scale for output
}

double sqWave(double freq){
  // Return a square wave at frequency freq 
  // with amplitude almost covering the full 0 to 3300 mV range
  unsigned per = (1/freq) * 1000000;      // period of waveform [us]
  unsigned t = micros();                  // time right now [us]
  if(t % per < per / 2) return 100;       // low in first half period
  else return 3200;                       // otherwise high
}

double triWave(double freq){
  // Return a square wave at frequency freq 
  // with amplitude almost covering the full 0 to 3300 mV range
  unsigned per = (1/freq) * 1000000;      // period of waveform [us]
  double perD = per;                      // a double precision version...
  unsigned t = micros();                  // time right now [us]
  double frac = (t % per) / perD;         // fraction of the way through the period
  return 200. + 2800. * frac;
}

double sinWave(double freq){
  // Return a square wave at frequency freq 
  // with amplitude almost covering the full 0 to 3300 mV range
  unsigned per = (1/freq) * 1000000;      // period of waveform [us]
  double perD = per;                      // a double precision version...
  unsigned t = micros();                  // time right now [us]
  double frac = (t % per) / perD;         // fraction of the way through the period
  return 1600. + 1100. * sin(2 * 3.14159 * frac);
}


double triSqSin(double freq){         // return a signal level in millivolts
  double retVal = 1600.;
  return retVal; 
}
