// An example demonstrating how to control the Adafruit Dot Star RGB LED
// included on board the ItsyBitsy M0 board. For more, follow the link
// https://learn.adafruit.com/introducing-itsy-bitsy-m0/pinouts#spi-flash-and-dotstar-2989068-14

#include <Adafruit_DotStar.h>
// one pixel, data on pin 41, clock on pin 40 for the Itsy Bitsy M0 Express
Adafruit_DotStar m0dotStar(1, 41, 40, DOTSTAR_BRG);

void m0dotStarOff() { // really useful to cut glare
  m0dotStar.begin(); // Initialize pins for output
  m0dotStar.setPixelColor(0, 255,255,255);
  m0dotStar.setBrightness(0);
  m0dotStar.show();  // Turn all LEDs off ASAP    
}

void m0dotStarOn() {
  m0dotStar.begin(); // Initialize pins for output
  m0dotStar.setPixelColor(0, 255,255,255);
  m0dotStar.setBrightness(255);
  m0dotStar.show();  // Turn all LEDs off ASAP    
}
