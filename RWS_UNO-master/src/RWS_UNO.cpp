/**************************************************************************/
/*!
  @file RWS_UNO.cpp

  @section intro Introduction

  An Arduino library for various extensions developed by Rick Sellens (RWS).
  The primary motivation is support of measurement activities in Queen's
  courses MECH 217 and MECH 848. The UNO version includes just the basics.

  2020-01-20: Folded in all the RWS_Arduino functionality except 
  console and logging. RWS_stamp ported to RWS_LOG so the libraries can be 
  used independently.

  @subsection author Author

  Written by Rick Sellens.

  @subsection license License

  CCBY license
*/
/**************************************************************************/
#include "RWS_UNO.h"

RWS_UNO::RWS_UNO() {}
RWS_UNO::~RWS_UNO() {}

/**************************************************************************/
/*!
    @brief Initialization of RWS Extensions
    @param spd Baud rate for Serial connection.
    @param tUTC Current time UTC, unix time in seconds.
    @param tzHours Local time zone in hours difference from UTC.
    @return none
*/
/**************************************************************************/
//void RWS_UNO::begin(){
//  return begin(115200);    // use 115200 as the default speed
//}
//void RWS_UNO::begin(unsigned long spd){
//  return begin(spd,0,0);
//}
void RWS_UNO::begin(unsigned long spd){
  baud = spd;                           // set the serial port speed
  if (spd == 0) baud = 115200;          // default to 115200
  baud = max(300UL,baud);               // limits from IDE 1.8.9 monitor
  baud = min(2000000UL,baud);
  Serial.begin(baud);
  while(!Serial && millis() < 5000);    // wait a while, not forever
  Wire.begin();                         // start i2c
  #if(ADC_RESOLUTION > 10)              // set resolution for non-328 boards
  analogReadResolution(ADC_RESOLUTION);
  #endif
  _analogMax = pow(2, ADC_RESOLUTION) - 1; // set max ADC value for all boards
}


/**************************************************************************/
/*!
    @brief Memory Management functions - approximate number of bytes between the
   heap and the stack. The answer doesn't seem to work on the ESP 32. Don't know why.
    @return Bytes of free memory between the heap and the stack.
*/
unsigned long RWS_UNO::bytesFree() {  // approximate number of bytes between the heap and the stack
  static byte sz = 1;         // malloc a small chunk
  char dummy;                 // a variable on the stack at a higher address
  char *c, *d;                // two byte sized pointers
  d = &dummy;                 // point to the stack variable
  c = (char *) malloc(sz++);  // point to the malloc'd space which should be lower
  unsigned long b = d - c;    // unsigned long version will be wacko if d < c
  if (d < c) b = c - d;       // this might work for ESP...
  free(c);                    // make sure to free the malloc() to avoid leaks
  return b;
}

/**************************************************************************/
/*!
    @brief Generate a pulse with the shape of a Gaussian Normal Distribution.
    @param t time for the pulse value
    @param tMean time when the pulse is maximum
    @param tStd pulse width in seconds expressed as a standard deviation
    @return Pulse height normalized to a max of 1.0.
*/
double RWS_UNO::normPulseTS(double t, double tMean, double tStd) {
  return normpdf((t - tMean) / tStd) / normpdf(0);
}

/**************************************************************************/
/*!
    @brief Value from Gaussian Normal Distribution with a mean of 0, standard
   deviation of 1.
    @param x position in the distribution
    @return PDF value
*/
double RWS_UNO::normpdf(double x) { return exp(-x * x / 2) / sqrt(2 * PI); }

/**************************************************************************/
/*!
    @brief A pseudo random number with a pseudo Gaussian distribution, mean = 0,
   sd = 1 You can test the idea with this python code:

        # a pretty good fake Gaussian from random integers
        n = 10000000
        tot = random.randint(0,1001,n)
        for i in range(0,15): tot += random.randint(0,1001,n)
        tot -= 8000
        tot = tot / 1156
        print(mean(tot),std(tot))
        hist(tot,bins=50)
    @return random number
*/
double RWS_UNO::randn() {
  unsigned long tot = 0;
  for (int i = 0; i < 16; i++)
    tot += random(1001);
  return (tot - 8000.) / 1156.;
}

/**************************************************************************
 *
 * Hardware Management functions
 *
 *
 **************************************************************************/
/**************************************************************************/
/*!
    @brief Voltage Functions - set the Regulator Voltage (measure from board)
    @param v the voltage measured from the power supply.
    @return maximum time between two run() calls since last reset.
*/
double RWS_UNO::setVReg(
    double v) { // set the Regulator Voltage (measure from board)
  if (v < 25 && v > 0.5)
    _vReg = v;
  return getVReg();
}
double RWS_UNO::getVReg() { // get the Regulator Voltage
  return _vReg;
}

/**************************************************************************/
/*!
    @brief Voltage Functions - set the appropriate Reference Voltage (for ADC)

    Sets the analog reference to be used for measurements, and the values
    needed for getV() to do conversions from integer values. The user sketch
    must look after setting any internal or external reference using
    analogReference() with board specific reference names, e.g. AR_INTERNAL.
    @param v the requested reference voltage
    @return analog reference voltage.
*/
double RWS_UNO::setVRef(
    double v) { // set the appropriate Reference Voltage (for ADC)
  _vRef = v;    // use the supplied value, probably _vReg for default
  return getVRef();
}

/**************************************************************************/
/*!
    @brief Request the current reference voltage value. Note that it cannot
    test the current setting and relies on previous calls to setVref to know
    the current value and will be wrong if the user sketch makes independent
    calls to analogReference().
    @return Vref
*/
/**************************************************************************/
double RWS_UNO::getVRef() { // get the Reference Voltage
  return _vRef;
}

/**************************************************************************/
/*!
    @brief Voltage Functions - get the current battery voltage from a Feather

    Voltages may be inaccurate if the power supply drops below normal. This
    will happen when the board is LiPo battery powered and the battery is
    dropping towards 3.6 volts and discharge so the regulator output drops
    below 3.3 volts and the default Vref gets lower. getVBat() will then start
    rising as the battery discharges further.
    @return estimated battery voltage.
*/
double
RWS_UNO::getVBat() { // get the current battery voltage from a Feather
  if (getVRef() < 2.0)
    return -1.0; // can't read
  #ifdef BAT_PIN
    return getV(BAT_PIN) * BAT_FACTOR;
  #else
    return 0;
  #endif  
}

/**************************************************************************/
/*!
    @brief Voltage Functions - get the current battery percentage from a Feather

    Voltages may be inaccurate if the power supply drops below normal. This
    will happen when the board is LiPo battery powered and the battery is
    dropping towards 3.6 volts and discharge so the regulator output drops
    below 3.3 volts and the default Vref gets lower. getVBat() will then start
    rising as the battery discharges further, as will getBat().
    @return estimated battery state of charge.
*/
int RWS_UNO::getBat() { // get a battery percentage charge estimate
  int pc = (getVBat() - 3.62) / (4.15 - 3.62) * 100.0;
  pc = max(pc, 0);
  pc = min(pc, 100);
  return pc;
}

/**************************************************************************/
/*!
    @brief Voltage Functions - get the current voltage from an ADC pin.
    @param pin Pin number to read the analog voltage from, e.g. A3
    @return ADC voltage.
*/
double RWS_UNO::getV(uint8_t pin) { // get a voltage from an analog input
  return analogRead(pin) * _vRef / _analogMax;
}
