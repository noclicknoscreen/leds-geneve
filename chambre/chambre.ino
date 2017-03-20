/*********************************************************************************
   Driver de leds pour le plafonier de l'entree
   Projet Geneve
   2017 - Noclick.noscreen_ - Pierre-Gilles Levallois

   TODO :
    - scénario qui fait une vague (sinusoidale de la brillance)
      entre les différentes zones de A à F
    - Scénario qui augmente et diminue doucement une zone.
    - Allumer la LED 13 pour dire quel scénario est en cours
      (1 cligno puis temps mort, 2 cli ou 3 cli)
    - Ajouter une lecture de pin digitale pour le contact sec)

*********************************************************************************/
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

// Nombre de zones
#define NUM_ZONES 24
#define NUM_STRIPS 8

// Signaux de pilotage des zones
# define PIN_1 3
# define PIN_2 4
# define PIN_3 5
# define PIN_4 6
# define PIN_5 9
# define PIN_6 10
# define PIN_7 21
# define PIN_8 22

// Definir les 6 zones a eclairer
// Valeurs réelles
#define NUM_LEDS_A 15               // OK
#define NUM_LEDS_B 44               // OK
#define NUM_LEDS_C 43               // OK
#define NUM_LEDS_D 44 // 28 + 16    // OK
#define NUM_LEDS_E 33 // 27 + 6     // OK
#define NUM_LEDS_F 9  // 2 + 7      // OK
#define NUM_LEDS_G 19               // OK
#define NUM_LEDS_H 9                // OK 
#define NUM_LEDS_I 8                // OK 
#define NUM_LEDS_J 5 // 3 + 2       // OK
#define NUM_LEDS_K 8 //             // OK
#define NUM_LEDS_L 6 // 2 + 4       // OK
#define NUM_LEDS_M 5 // 2 + 3       // OK 
#define NUM_LEDS_N 9 // 4 + 5       // OK
#define NUM_LEDS_O 8                // OK
#define NUM_LEDS_P 6 // 3 + 3       // OK
#define NUM_LEDS_Q 20               // OK
#define NUM_LEDS_R 16 // 8 + 8      // OK
#define NUM_LEDS_S 26               // OK
#define NUM_LEDS_T 29               // OK 
#define NUM_LEDS_U 40 // 19 + 21    // OK
#define NUM_LEDS_V 44               // OK
#define NUM_LEDS_W 41 // 30 + 11    // OK
#define NUM_LEDS_X 15               // OK

Adafruit_NeoPixel leds1 = Adafruit_NeoPixel( NUM_LEDS_A, PIN_1 ,  NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel leds2 = Adafruit_NeoPixel( NUM_LEDS_B + NUM_LEDS_C, PIN_2 ,  NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel leds3 = Adafruit_NeoPixel( NUM_LEDS_D + NUM_LEDS_E, PIN_3 ,  NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel leds4 = Adafruit_NeoPixel( NUM_LEDS_F + NUM_LEDS_G + NUM_LEDS_H + NUM_LEDS_I + NUM_LEDS_J + NUM_LEDS_K + NUM_LEDS_L + NUM_LEDS_M + NUM_LEDS_N + NUM_LEDS_O + NUM_LEDS_P + NUM_LEDS_Q + NUM_LEDS_R, PIN_4 ,  NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel leds5 = Adafruit_NeoPixel( NUM_LEDS_S + NUM_LEDS_T, PIN_5 ,  NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel leds6 = Adafruit_NeoPixel( NUM_LEDS_U, PIN_6 ,  NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel leds7 = Adafruit_NeoPixel( NUM_LEDS_V + NUM_LEDS_W, PIN_7 ,  NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel leds8 = Adafruit_NeoPixel( NUM_LEDS_X, PIN_8 ,  NEO_GRB + NEO_KHZ800);

const int PINS[NUM_STRIPS] = {PIN_1, PIN_2, PIN_3, PIN_4, PIN_5, PIN_6, PIN_7, PIN_8};
const int NUM_LEDS[] = {
  NUM_LEDS_A, NUM_LEDS_B, NUM_LEDS_C, NUM_LEDS_D,
  NUM_LEDS_E, NUM_LEDS_F, NUM_LEDS_G, NUM_LEDS_H,
  NUM_LEDS_I, NUM_LEDS_J, NUM_LEDS_K, NUM_LEDS_L,
  NUM_LEDS_M, NUM_LEDS_N, NUM_LEDS_O, NUM_LEDS_P,
  NUM_LEDS_Q, NUM_LEDS_R, NUM_LEDS_S, NUM_LEDS_T,
  NUM_LEDS_U, NUM_LEDS_V, NUM_LEDS_W, NUM_LEDS_X
};
int totalNumLeds = 0;

Adafruit_NeoPixel * STRIPS[NUM_ZONES] = {&leds1, &leds2, &leds3, &leds4, &leds5, &leds6, &leds7, &leds8};


// Choix du scénario
int choix = 0;
int brightness_value = 255;

/*********************************************************************************
   Setup
 *********************************************************************************/
void setup()
{
  Serial.begin(9600);

  for (int i = 0; i < NUM_STRIPS; i++) {
    Serial.print("Init Zone #"); Serial.println(i);
    pinMode(PINS[i], LOW);
    STRIPS[i]->begin();
    STRIPS[i]->setBrightness(brightness_value);
    setStripColor(STRIPS[i], 0, 0, 0);

    // Faire la somme de tous les pixels
    totalNumLeds += STRIPS[i]->numPixels();

  }

  choix = 0;
  delay(1000);
}

/*********************************************************************************
   BOUCLE
 *********************************************************************************/
void loop() {


  // RAINBOW ALL OVER THE SKY -----------------------------------------------------
  float period = 5000.0f;
  float timeRatio = fmod(millis(), period) / period;
  Serial.print("Time = ");
  Serial.print(timeRatio);  

  //timeRatio = (sin(timeRatio * TWO_PI) + 1) * 0.5f;
  
  Serial.print(", after Time = ");
  Serial.println(timeRatio);  

  for (float i = 0; i < totalNumLeds; i++) {
    
    float toWheel = fmod(i + timeRatio * totalNumLeds, totalNumLeds) / totalNumLeds;
    
    //toWheel += timeRatio;
    // Start color = 0 (change it)
    // Stop color = 255 (Change it)
    toWheel = 0 + toWheel * 255;
    
    setOnePixelOfAll(i / 502, Wheel(toWheel));
    
  }
  
  showAllStrips();

}

/*****************************************************************
   Setting all zones to back.
 *****************************************************************/
void SetAllZonesToColor(int r, int g, int b) {
  // Tout à noir
  for (int i = 0; i < NUM_STRIPS; i++) {
    setStripColor(STRIPS[i], r, g, b);
  }
}

void setStripColor(Adafruit_NeoPixel * strip, int r, int g, int b) {
  for (int i = 0; i < strip->numPixels(); i++) {
    strip->setPixelColor(i, r, g, b);
  }

}

/*****************************************************************
  Showing all zones
 *****************************************************************/
void showAllStrips() {
  // Néopixel
  for (int i = 0; i < NUM_STRIPS; i++) {
    STRIPS[i]->show();
  }
}


/*****************************************************************
  // Slightly different, this makes the rainbow equally distributed throughout
 *****************************************************************/
void rainbowCycle(Adafruit_NeoPixel * strip, uint8_t wait) {
  uint16_t i, j;

  for (j = 0; j < 256 * 5; j++) { // 5 cycles of all colors on wheel
    for (i = 0; i < strip->numPixels(); i++) {
      strip->setPixelColor(i, Wheel(((i * 256 / strip->numPixels()) + j) & 255));
    }
    strip->show();
    delay(wait);
  }
}

/*****************************************************************
  // Input a value 0 to 255 to get a color value.
  // The colours are a transition r - g - b - back to r.
 *****************************************************************/
uint32_t Wheel(byte WheelPos) {
  Adafruit_NeoPixel strip = Adafruit_NeoPixel();
  WheelPos = 255 - WheelPos;
  if (WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if (WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}

void setOnePixelOfAll(float ratio, uint32_t c) {

  int idxStrip = 0;
  
  int tempTotalA = 0;
  int tempTotalB = 0;
  
  float idxPixel = ratio * (float)totalNumLeds;

  for (idxStrip = 0; idxStrip < NUM_STRIPS - 1; idxStrip++) {

    tempTotalA += STRIPS[idxStrip]->numPixels();
    
    if (tempTotalA > idxPixel) {
      // On sort
      break;
    }
    
    tempTotalB += STRIPS[idxStrip]->numPixels();
    
  }

/*
  Serial.print("ratio = " );
  Serial.print(ratio);
  
  Serial.print(", tempTotalA = " );
  Serial.print(tempTotalA);
  
  Serial.print(", tempTotalB = " );
  Serial.print(tempTotalB);

  Serial.print(", idxPixel = " );
  Serial.print(idxPixel);

  Serial.print(", idxRelatif = " );
  Serial.print(idxPixel - tempTotalB);

  Serial.print(", idxStrip = " );
  Serial.println(idxStrip);
  
  Serial.println("--------------");
*/
  // Chooooose the color
  STRIPS[idxStrip]->setPixelColor(idxPixel - tempTotalB, c);

}


