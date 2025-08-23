#include "RWS_UNO.h"

RWS_UNO uno = RWS_UNO();

void setup()
{
  uno.begin(115200);

  Serial.print("\n\nIdentifying different boards at compile time\n\n");
  
  P("UNO or something bigger and faster?\n");
#if(F_CPU > 16000000L)
  PL("  F_CPU > 16000000L, so this is a faster machine, probably 32 bit.");
#else
  PL("  F_CPU <= 16000000L, so probably an AVR machine like an UNO.");
#endif
#ifdef ARDUINO_ARCH_AVR
  PL("  ARDUINO_ARCH_AVR defined, so probably an AVR machine like an UNO.");
#else
  PL("  ARDUINO_ARCH_AVR not defined, so probably something bigger / faster.");
#endif
  P("\nA fast CPU clock can give some things away. 48 MHz could be an M0,\nwhile 120 MHz is more likely an M4, but not for sure.\n");
#ifdef F_CPU
  P("  F_CPU defined, so we know the speed in MHz: ");
  PL(F_CPU / 1000000L);
#endif
  P("\nSome tags give us specific product lines or architectures.\n");
#ifdef TEENSYDUINO
  PL("  TEENSYDUINO defined, so it is a Teensy from PJRC and we probably won't know much more.");
#endif
#ifdef ARDUINO_ARCH_SAMD
  PL("  ARDUINO_ARCH_SAMD defined, so probably an M0 or an M4.");
#endif
#ifdef ARDUINO_ARCH_ESP8266
  PL("  ARDUINO_ARCH_ESP8266 defined");
#endif
#ifdef ARDUINO_ARCH_ESP32
  PL("ARDUINO_ARCH_ESP32 defined");
#endif
  P("\nOther flags tell us about the IDE and the sketch, and the time it was compiled.\n");
#ifdef ARDUINO
  P("  ARDUINO defined, so we know the IDE Version Number: ");
  int a = ARDUINO / 10000; 
  int b = (ARDUINO - a * 10000) / 100;
  int c = ARDUINO % 100;
  P(a); P("."); P(b); P("."); PL(c); 
#endif
#ifdef __SKETCH_NAME__
  P("  __SKETCH_NAME__ defined, so we know the name is: ");
  PL(__SKETCH_NAME__);
#endif
#ifdef __DATE__
  P("  __DATE__ defined, so we know the compile date is: ");
  PL(__DATE__);
#else
  P("  __DATE__ not defined, so we don't know when the compile took place. Try again!\n");
#endif
#ifdef __TIME__
  P("  __TIME__ defined, so we know the compile time is: ");
  P(__TIME__);
  P(", probably local time.\n");
#endif

PL();
}

 
void loop()
{
  delay(13234);           // wait for next scan
}
