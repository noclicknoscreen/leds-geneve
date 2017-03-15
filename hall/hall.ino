/*********************************************************************************
   Driver de leds pour le plafonier de l'entree
   Projet Geneve
   2017 - Noclick.noscreen_ - Pierre-Gilles Levallois

   TODO :
    - scénario qui fait une vague (sinusoidale de la brillance)
      entre les différentes zones de A à F
    - Scénario qui augmente et diminue doucement une zone.
    - Ajouter une lecture de pin digitale pour le contact sec)

*********************************************************************************/
#include <Adafruit_NeoPixel.h>
#include <Zones.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

// Nombre de zones
#define NUM_ZONES 6

// on board Led
#define TEENSY_LED 13
// Signaux de pilotage des zones
# define PIN_ZONE_A 3
# define PIN_ZONE_B 4
# define PIN_ZONE_C 5
# define PIN_ZONE_D 6
# define PIN_ZONE_E 9
# define PIN_ZONE_F 10

// Signaux de pilotage des scénarii
#define PIN_SC1 7
#define PIN_SC2 8
#define PIN_SC3 A0
#define PIN_SC4 A1

// Definir les 6 zones a eclairer
// Valeurs réelles
/*
  # define NUM_LEDS_A 80 // 90  // 30x2 + 15x2
  # define NUM_LEDS_B 70 //76  // 23x2 + 15x2
  # define NUM_LEDS_C 99 // 106 // 38x2 + 15x2
  # define NUM_LEDS_D 86 //90  // 30x2 + 15x2
  # define NUM_LEDS_E 88  // 29x2 + 15x2
  # define NUM_LEDS_F 58 // 88  // 29x2 + 15x2
*/
/* Valeurs pour la maquette */
# define NUM_LEDS_A 22
# define NUM_LEDS_B 18
# define NUM_LEDS_C 26
# define NUM_LEDS_D 22
# define NUM_LEDS_E 22
# define NUM_LEDS_F 21

const int PINS[] = {PIN_ZONE_A, PIN_ZONE_B, PIN_ZONE_C, PIN_ZONE_D, PIN_ZONE_E, PIN_ZONE_F};

Adafruit_NeoPixel pixels_A = Adafruit_NeoPixel(NUM_LEDS_A, PIN_ZONE_A, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pixels_B = Adafruit_NeoPixel(NUM_LEDS_B, PIN_ZONE_B, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pixels_C = Adafruit_NeoPixel(NUM_LEDS_C, PIN_ZONE_C, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pixels_D = Adafruit_NeoPixel(NUM_LEDS_D, PIN_ZONE_D, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pixels_E = Adafruit_NeoPixel(NUM_LEDS_E, PIN_ZONE_E, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pixels_F = Adafruit_NeoPixel(NUM_LEDS_F, PIN_ZONE_F, NEO_GRB + NEO_KHZ800);

Zone zoneA = Zone(&pixels_A, 1, NUM_LEDS_A);
Zone zoneB = Zone(&pixels_B, 1, NUM_LEDS_B);
Zone zoneC = Zone(&pixels_C, 1, NUM_LEDS_C);
Zone zoneD = Zone(&pixels_D, 1, NUM_LEDS_D);
Zone zoneE = Zone(&pixels_E, 1, NUM_LEDS_E);
Zone zoneF = Zone(&pixels_F, 1, NUM_LEDS_F);

Zone * ZONES[NUM_ZONES] = {&zoneA, &zoneB, &zoneC, &zoneD, &zoneE, &zoneF};

// Choix du scénario
int choix = 0;

// Pour les calculs de pulsations
float heartBeat;
float heartFreq = 10000;

int brightness_value = 240; // Between 0 and 100 %

// False when current scénario is repeated for the second time
bool firstTime = true;
// Counter to set interruptibles Delays
elapsedMillis elapsedTime;
// Time to hold a scenario before, changing (to be compare to elapsedTime
uint16_t holdTime = 0;
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

  // Pin de pilotage des scénarii
  pinMode(PIN_SC1, INPUT_PULLUP); // dry contact
  pinMode(PIN_SC2, INPUT_PULLUP); // dry contact
  pinMode(PIN_SC3, INPUT_PULLUP); // dry contact
  pinMode(PIN_SC4, INPUT_PULLUP); // dry contact

  pinMode(PIN_ZONE_A, LOW);
  pinMode(PIN_ZONE_B, LOW);
  pinMode(PIN_ZONE_C, LOW);
  pinMode(PIN_ZONE_D, LOW);
  pinMode(PIN_ZONE_E, LOW);
  pinMode(PIN_ZONE_F, LOW);

  pinMode(TEENSY_LED, HIGH);

  // Interruptions sur les pins de choix de scénario
  //RISING/HIGH/CHANGE/LOW/FALLING
  attachInterrupt (PIN_SC1, choix1, FALLING);
  attachInterrupt (PIN_SC2, choix2, FALLING);
  attachInterrupt (PIN_SC3, choix3, FALLING);
  attachInterrupt (PIN_SC4, choix4, FALLING);

  pixels_A.begin(); // This initializes the NeoPixel library.
  pixels_B.begin(); // This initializes the NeoPixel library.
  pixels_C.begin(); // This initializes the NeoPixel library.
  pixels_D.begin(); // This initializes the NeoPixel library.
  pixels_E.begin(); // This initializes the NeoPixel library.
  pixels_F.begin(); // This initializes the NeoPixel library.

  pixels_A.setBrightness(brightness_value);
  pixels_B.setBrightness(brightness_value);
  pixels_C.setBrightness(brightness_value);
  pixels_D.setBrightness(brightness_value);
  pixels_E.setBrightness(brightness_value);
  pixels_F.setBrightness(brightness_value);

  zoneA.setZoneBrightness(brightness_value);
  zoneB.setZoneBrightness(brightness_value);
  zoneC.setZoneBrightness(brightness_value);
  zoneD.setZoneBrightness(brightness_value);
  zoneE.setZoneBrightness(brightness_value);
  zoneF.setZoneBrightness(brightness_value);

  SetAllZonesToBlack();
  showAllZones();

  choix = 0;
  firstTime = true;
  holdTime = 1000;
  Serial.println("___END SETUP___");
}

/*********************************************************************************
   BOUCLE
 *********************************************************************************/
void loop() {

  board_blinking(500);

  // choix du scénario
  switch (choix) {
    case 0 : // Scénario de test des zones
      if (propagate_white(5000)) {
        choix = 1;
      }
      break;
    case 1 :
      zoneA.setZoneColor(255, 255, 255);
      zoneB.setZoneColor(175, 60, 190);
      zoneC.setZoneColor(100, 107, 189);
      zoneD.setZoneColor(175, 82, 90);
      zoneE.setZoneColor(255, 255, 255);
      zoneF.setZoneColor(175, 60, 190);
      break;
    case 2 :
      {
        // init
        if (firstTime) {
          Serial.println("---------------------------------------- First Time for scénario 2 ! ------------------------------------------");
          holdTime = 1000;
          int k = random(0, NUM_ZONES - 1);
          int r = random(0, 255);
          int g = random(0, 255);
          int b = random(0, 255);
          Serial.print(r); Serial.print(":"); Serial.print(g); Serial.print(":"); Serial.println(b);
        }

        if (elapsedTime > holdTime) {
          int k = random(0, NUM_ZONES - 1);
          int r = random(0, 255);
          int g = random(0, 255);
          int b = random(0, 255);
          Serial.print(r); Serial.print(":"); Serial.print(g); Serial.print(":"); Serial.println(b);
          Serial.print("hoding time :"); Serial.print(holdTime/1000, DEC); Serial.println(" seconds");
          elapsedTime -= holdTime;
          holdTime = random(1000, 10000);
          ZONES[k]->setZoneColor(r, g, b);
          firstTime = false;
        }
      }
      break;
    case 3 :
      break;
    case 4 :
      break;
    default :
      Serial.println("No scenario selected !");
      board_blinking(100);
  }
  showAllZones();
}

void initScenario() {
  // Debouncing uninterruptible delay
  cli();
  delay(200);
  Serial.print("Scénario "); Serial.println(choix);
  firstTime = true;
  sei();
}

/*
   Propagate White
*/
boolean propagate_white(int freq) {
  float perCent = fmod(millis(), freq) / freq;
  perCent *= 100.0f;

  float zoneAValue, zoneBValue, zoneCValue, zoneDValue, zoneEValue, zoneFValue;

  zoneAValue = clampMap(perCent,  0,  16,   0, 255);
  zoneBValue = clampMap(perCent,  16,  32,   0, 255);
  zoneCValue = clampMap(perCent,  32, 48,   0, 255);
  zoneDValue = clampMap(perCent,  48,  64,   0, 255);
  zoneEValue = clampMap(perCent,  64,  78,   0, 255);
  zoneFValue = clampMap(perCent,  78,  96,   0, 255);

  zoneA.setZoneColor(zoneAValue, zoneAValue, zoneAValue);
  zoneB.setZoneColor(zoneBValue, zoneBValue, zoneBValue);
  zoneC.setZoneColor(zoneCValue, zoneCValue, zoneCValue);
  zoneD.setZoneColor(zoneDValue, zoneDValue, zoneDValue);
  zoneE.setZoneColor(zoneEValue, zoneEValue, zoneEValue);
  zoneF.setZoneColor(zoneFValue, zoneFValue, zoneFValue);
  if (zoneFValue >= 255) {
    return true;
  }
  return false;
}

/*
   Binking function for onboard LED
*/
void board_blinking(int freq) {
  int myMillis = millis();
  myMillis %= 2 * freq;
  if (myMillis >= freq) {
    digitalWrite(TEENSY_LED, HIGH);
  } else {
    digitalWrite(TEENSY_LED, LOW);
  }
}

/*****************************************************************
   Interruptions
 *****************************************************************/
void choix1() {
  choix = 1;
  initScenario();
}

void choix2() {
  choix = 2;
  initScenario();
}

void choix3() {
  choix = 3;
  initScenario();
}

void choix4() {
  choix = 4;
  initScenario();
}

/*****************************************************************
   Framwork
 *****************************************************************/
float clampMap(float value, float inMin, float inMax, float outMin, float outMax) {
  return constrain(map(constrain(value,  inMin,  inMax), inMin, inMax, outMin, outMax), outMin, outMax);
}

void showAllZones() {
  // Néopixel
  zoneA.show();
  zoneB.show();
  zoneC.show();
  zoneD.show();
  zoneE.show();
  zoneF.show();
}

void showAllStrips() {
  pixels_A.show();
  pixels_B.show();
  pixels_C.show();
  pixels_D.show();
  pixels_E.show();
  pixels_F.show();
}

void SetAllZonesToBlack() {
  // Tout à noir
  zoneA.setZoneColor(0, 0, 0);
  zoneB.setZoneColor(0, 0, 0);
  zoneC.setZoneColor(0, 0, 0);
  zoneD.setZoneColor(0, 0, 0);
  zoneE.setZoneColor(0, 0, 0);
  zoneF.setZoneColor(0, 0, 0);
}

void setAllStrips() {
  for (int i = 0; i < pixels_A.numPixels(); i++ ) {
    pixels_A.setPixelColor(i, 0, 0, 0);
  }
  for (int i = 0; i < pixels_B.numPixels(); i++ ) {
    pixels_B.setPixelColor(i, 0, 0, 0);
  }
  for (int i = 0; i < pixels_C.numPixels(); i++ ) {
    pixels_C.setPixelColor(i, 0, 0, 0);
  }
  for (int i = 0; i < pixels_D.numPixels(); i++ ) {
    pixels_D.setPixelColor(i, 0, 0, 0);
  }
  for (int i = 0; i < pixels_E.numPixels(); i++ ) {
    pixels_E.setPixelColor(i, 0, 0, 0);
  }
  for (int i = 0; i < pixels_F.numPixels(); i++ ) {
    pixels_F.setPixelColor(i, 0, 0, 0);
  }
}

