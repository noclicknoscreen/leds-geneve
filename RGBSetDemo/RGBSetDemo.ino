#include<FastLED.h>
#define NUM_LEDS 100
#define FRAMES_PER_SECOND  120

CRGBArray<NUM_LEDS> leds;

void setup() {
  FastLED.addLeds<NEOPIXEL, 3>(leds, NUM_LEDS).setCorrection(Tungsten100W);
  fill_solid(leds, NUM_LEDS, 0x000000);
  FastLED.show();
  FastLED.delay(1000);
  
  fill_solid(leds, NUM_LEDS, 0xffffff);
  FastLED.show();
  
  Serial.begin(9600);
}

void loop() {
  /*
  fill_solid(leds, NUM_LEDS, 0xffffff);
  FastLED.show()*/
  FastLED.delay(1000 / FRAMES_PER_SECOND);
  Serial.println(FastLED.getFPS(), DEC);

}

