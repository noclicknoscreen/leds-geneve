/*********************************************************************************
   Driver de leds pour le plafonier du hall DIALIN
   Projet Geneve
   2017 - Noclick.noscreen_ - Pierre-Gilles Levallois - Sebastien Albert

*********************************************************************************/
#include <Adafruit_NeoPixel.h>
#include <Zones.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

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

const int PINS[NUM_ZONES] = {PIN_ZONE_C1, PIN_ZONE_C2, PIN_ZONE_C3, PIN_ZONE_C4, PIN_ZONE_C5, PIN_ZONE_C6, PIN_ZONE_C8};

Adafruit_NeoPixel pixels_C1 = Adafruit_NeoPixel(NUM_LEDS_C1, PIN_ZONE_C1, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pixels_C2 = Adafruit_NeoPixel(NUM_LEDS_C2, PIN_ZONE_C2, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pixels_C3 = Adafruit_NeoPixel(NUM_LEDS_C3, PIN_ZONE_C3, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pixels_C4 = Adafruit_NeoPixel(NUM_LEDS_C4, PIN_ZONE_C4, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pixels_C5 = Adafruit_NeoPixel(NUM_LEDS_C5, PIN_ZONE_C5, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pixels_C6 = Adafruit_NeoPixel(NUM_LEDS_C6, PIN_ZONE_C6, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pixels_C7 = Adafruit_NeoPixel(NUM_LEDS_C7, PIN_ZONE_C7, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pixels_C8 = Adafruit_NeoPixel(NUM_LEDS_C8, PIN_ZONE_C8, NEO_GRB + NEO_KHZ800);

Zone zone1 = Zone(&pixels_C1, 1, NUM_LEDS_C1);
Zone zone2 = Zone(&pixels_C2, 1, NUM_LEDS_C2);
Zone zone3 = Zone(&pixels_C3, 1, NUM_LEDS_C3);
Zone zone4 = Zone(&pixels_C4, 1, NUM_LEDS_C4);
Zone zone5 = Zone(&pixels_C5, 1, NUM_LEDS_C5);
Zone zone6 = Zone(&pixels_C6, 1, NUM_LEDS_C6);
Zone zone7 = Zone(&pixels_C7, 1, NUM_LEDS_C7);
Zone zone8 = Zone(&pixels_C8, 1, NUM_LEDS_C8);

Zone * ZONES[NUM_ZONES] = {&zone1, &zone2, &zone3, &zone4, &zone5, &zone6, &zone7, &zone8};

//Adafruit_NeoPixel * STRIPS[NUM_ZONES] = {&pixels_C1, &pixels_C2, &pixels_C3, &pixels_C4, &pixels_C5, &pixels_C6, &pixels_C7, &pixels_C8};

int brightness_value = 255; // Between 0 and 100 %

int refreshMode = ZONE_MODE;
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

  for (int i = 0; i < NUM_ZONES; i++) {
    Serial.print("Init Zone #"); Serial.println(i);
    pinMode(PINS[i], LOW);
    STRIPS[i]->begin();
    STRIPS[i]->setBrightness(brightness_value);
    ZONES[i]->setZoneBrightness(brightness_value);
    // Faire la somme de tous les pixels
    totalNumLeds += STRIPS[i]->numPixels();
  }

  SetAllZonesToColor(0, 0, 0);
  showAllZones();

  choix = 0;
  Serial.println("___END SETUP___");
}

/*********************************************************************************
   BOUCLE
 *********************************************************************************/
void loop() {

  board_blinking(500);
  //SetAllZonesToColor(255, 255, 255);
  
  zone1.setZoneColor(255, 0, 0);
  delay(500);
  zone1.setZoneColor(0, 0, 0);
  delay(500);
  showAllZones();
  
  int knxA, knxB, knxC, knxD;
  // Pin de pilotage des scénarii
  knxA = digitalRead(PIN_SC1);
  knxB = digitalRead(PIN_SC2);
  knxC = digitalRead(PIN_SC3);
  knxD = digitalRead(PIN_SC4);

  //if (millis() % 1000 == 0) {
  Serial.print(millis());Serial.print(" ");
  Serial.print("KNX State (Default State 1, enclenché 0) :");
  Serial.print("A=");Serial.print(knxA);Serial.print(" ");
  Serial.print("B=");Serial.print(knxB);Serial.print(" ");
  Serial.print("C=");Serial.print(knxC);Serial.print(" ");
  Serial.print("D=");Serial.print(knxD);Serial.print(" ");
  Serial.print("Choix=");Serial.print(choix);Serial.print(" ");
  Serial.println();
  //}
  
/*
  // choix du scénario
  switch (choix) {
    case 1 :
      if (firstTime) {
        Serial.println("Scénario 1 : Blanc violacé");
        firstTime = false;
        holdTime = 100;
        intervall = 10;
        refreshMode = STRIP_MODE;
      }
      if (elapsedTime > holdTime && intervall > 0) {
        setColorOneLedEvery(intervall, 255, 255, 255);
        elapsedTime -= holdTime;
        intervall--;
      }
      break;
    case 2 :
      if (firstTime) {
        Serial.println("Scénario 2 : Blanc pur");
        firstTime = false;
        refreshMode = ZONE_MODE;
      }
      SetAllZonesToColor(255, 255, 255);
      break;
    case 3 :
      if (firstTime) {
        Serial.println("Scénario 3 : Blanc tirant sur le vert");
        firstTime = false;
        refreshMode = ZONE_MODE;
      }
      SetAllZonesToColor(random(100, 150), 255, 230);
      break;
    case 4 :
      if (firstTime) {
        Serial.println("Scénario 4 : Blanc pur");
        firstTime = false;
        refreshMode = ZONE_MODE;
      }
      SetAllZonesToColor(255, 255, 255);
    case 5 :
      if (firstTime) {
        Serial.println("Scénario #5, All the lights to black.");
        firstTime = false;
        // All the light OFF
        SetAllZonesToColor(0, 0, 0);
        for (int i = 0; i < NUM_STRIPS; i++) {
          SetAllStripsToColor(0, 0, 0);
        }
      }
      break;
    default :
      choix = 1;
  }
  
  if (refreshMode == ZONE_MODE) {
    showAllZones();
  } else {
    showAllStrips();
  }
  */
/*
  // Reading choice
  read_choice();
  */
}

/*****************************************************************
   Setting all zones to back.
 *****************************************************************/
void SetAllZonesToColor(int r, int g, int b) {
  // Tout à noir
  for (int i = 0; i < NUM_ZONES; i++) {
    ZONES[i]->setZoneColor(r, g, b);
  }
}

/*****************************************************************
  Showing all zones
 *****************************************************************/
void showAllZones() {
  // Néopixel
  for (int i = 0; i < NUM_ZONES; i++) {
    ZONES[i]->show();
  }
}


