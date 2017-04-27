/*********************************************************************************
   Driver de leds pour le plafonier du hall DIALIN
   Projet Geneve
   2017 - Noclick.noscreen_ - Pierre-Gilles Levallois - Sebastien Albert

*********************************************************************************/
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

// Mode TEST : Décommenter cette ligne
//define TEST_MODE 2
// 0 - Test des scénariis un par un (1-5)
// 2 = Test des Strips
//---------------------------------------

// Nombre de zones
#define NUM_ZONES 8
#define NUM_STRIPS 8

// Signaux de pilotage des scénarii
#define PIN_SC1 7
#define PIN_SC2 8
#define PIN_SC3 A0
#define PIN_SC4 A1

#include <ncns-leds-lib.h>

// on board Led
#define TEENSY_LED 13

// Signaux de pilotage des zones
# define PIN_ZONE_C1 3
# define PIN_ZONE_C2 4
# define PIN_ZONE_C3 5
# define PIN_ZONE_C4 6
# define PIN_ZONE_C5 9
# define PIN_ZONE_C6 10
# define PIN_ZONE_C7 21  // 
# define PIN_ZONE_C8 22  // 

// Definir les 6 zones a eclairer
// Valeurs réelles
# define NUM_LEDS_C1 150 // Remettre 43
# define NUM_LEDS_C2 150
# define NUM_LEDS_C3 150
# define NUM_LEDS_C4 150
# define NUM_LEDS_C5 150
# define NUM_LEDS_C6 150 // OK
# define NUM_LEDS_C7 150
# define NUM_LEDS_C8 150 // OK

const int PINS[NUM_STRIPS] = {PIN_ZONE_C1, PIN_ZONE_C2, PIN_ZONE_C3, PIN_ZONE_C4, PIN_ZONE_C5, PIN_ZONE_C6, PIN_ZONE_C8};

Adafruit_NeoPixel pixels_C1 = Adafruit_NeoPixel(NUM_LEDS_C1, PIN_ZONE_C1, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pixels_C2 = Adafruit_NeoPixel(NUM_LEDS_C2, PIN_ZONE_C2, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pixels_C3 = Adafruit_NeoPixel(NUM_LEDS_C3, PIN_ZONE_C3, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pixels_C4 = Adafruit_NeoPixel(NUM_LEDS_C4, PIN_ZONE_C4, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pixels_C5 = Adafruit_NeoPixel(NUM_LEDS_C5, PIN_ZONE_C5, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pixels_C6 = Adafruit_NeoPixel(NUM_LEDS_C6, PIN_ZONE_C6, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pixels_C7 = Adafruit_NeoPixel(NUM_LEDS_C7, PIN_ZONE_C7, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pixels_C8 = Adafruit_NeoPixel(NUM_LEDS_C8, PIN_ZONE_C8, NEO_GRB + NEO_KHZ800);

int brightness_value = 255; // Between 0 and 100 %
int intervall;

/*********************************************************************************
   Setup
 *********************************************************************************/
void setup()
{
#if defined (__AVR_ATtiny85__)
  if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
#endif
  Serial.begin(9600);
  Serial.println("___ENTERING SETUP___");

  STRIPS[0] = &pixels_C1;
  STRIPS[1] = &pixels_C2;
  STRIPS[2] = &pixels_C3;
  STRIPS[3] = &pixels_C4;
  STRIPS[4] = &pixels_C5;
  STRIPS[5] = &pixels_C6;
  STRIPS[6] = &pixels_C7;
  STRIPS[7] = &pixels_C8;

  pinMode(TEENSY_LED, HIGH);

  // Pin de pilotage des scénarii
  pinMode(PIN_SC1, INPUT_PULLUP); // dry contact
  pinMode(PIN_SC2, INPUT_PULLUP); // dry contact
  pinMode(PIN_SC3, INPUT_PULLUP); // dry contact
  pinMode(PIN_SC4, INPUT_PULLUP); // dry contact

  for (int i = 0; i < NUM_STRIPS; i++) {
    Serial.print("Init Zone #"); Serial.println(i);
    pinMode(PINS[i], LOW);
    STRIPS[i]->begin();
    STRIPS[i]->setBrightness(brightness_value);
    // Faire la somme de tous les pixels
    totalNumLeds += STRIPS[i]->numPixels();
  }

  SetAllStripsToColor(0, 0, 0);
  showAllStrips();
  choix = 5;
  Serial.println("___END SETUP___");
}

/*********************************************************************************
   BOUCLE
 *********************************************************************************/
void loop() {
  board_blinking(500);

  // Mode TEST
  // Tester toutes les zones à chaque appuie sur la touche 'T'
#ifdef TEST_MODE
  if (TEST_MODE != 0) {
    testerZonesEtStrips();
  }
#endif

#ifdef TEST_MODE
  if (TEST_MODE == 0) {
#endif

    //SetAllZonesToColor(255, 255, 255);

    // choix du scénario
    switch (choix) {
      case 5 :
        if (firstTime) {
          Serial.println("Scénario #5, All the lights to black.");
          firstTime = false;
          // All the light OFF
          SetAllStripsToColor(0, 0, 0);
        }
        break;
      default :
        white();
    }

    showAllStrips();

#ifdef TEST_MODE
  } // end if TEST_MODE == 0
#endif

  // Reading choice
  read_choice();
}


void white() {
  if (firstTime) {
    Serial.print("Scénario #"); Serial.print(choix); Serial.println(", All the lights to white.");
    firstTime = false;
    // All the light OFF
    SetAllStripsToColor(255, 255, 255);
  }
}

