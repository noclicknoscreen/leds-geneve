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
#include <Zones.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

// Mode TEST : Décommenter cette ligne
//#define TEST_MODE 2
// 1 = Test des zones
// 2 = Test des Strips
//---------------------------------------

// Nombre de zones
#define NUM_ZONES 24
#define NUM_STRIPS 8

// Signaux de pilotage des scénarii
#define PIN_SC1 7
#define PIN_SC2 8
#define PIN_SC3 A0
#define PIN_SC4 A1

#include <ncns-leds-lib.h>

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

Zone zoneA = Zone(&leds1, 1, NUM_LEDS_A);
Zone zoneB = Zone(&leds2, 1, NUM_LEDS_B);
Zone zoneC = Zone(&leds2, 1, NUM_LEDS_C);
Zone zoneD = Zone(&leds3, 1, NUM_LEDS_D);
Zone zoneE = Zone(&leds3, 1, NUM_LEDS_E);
Zone zoneF = Zone(&leds4, 1, NUM_LEDS_F);
Zone zoneG = Zone(&leds4, 1, NUM_LEDS_G);
Zone zoneH = Zone(&leds4, 1, NUM_LEDS_H);
Zone zoneI = Zone(&leds4, 1, NUM_LEDS_I);
Zone zoneJ = Zone(&leds4, 1, NUM_LEDS_J);
Zone zoneK = Zone(&leds4, 1, NUM_LEDS_K);
Zone zoneL = Zone(&leds4, 1, NUM_LEDS_L);
Zone zoneM = Zone(&leds4, 1, NUM_LEDS_M);
Zone zoneN = Zone(&leds4, 1, NUM_LEDS_N);
Zone zoneO = Zone(&leds4, 1, NUM_LEDS_O);
Zone zoneP = Zone(&leds4, 1, NUM_LEDS_P);
Zone zoneQ = Zone(&leds4, 1, NUM_LEDS_Q);
Zone zoneR = Zone(&leds4, 1, NUM_LEDS_R);
Zone zoneS = Zone(&leds5, 1, NUM_LEDS_S);
Zone zoneT = Zone(&leds5, 1, NUM_LEDS_T);
Zone zoneU = Zone(&leds6, 1, NUM_LEDS_U);
Zone zoneV = Zone(&leds7, 1, NUM_LEDS_V);
Zone zoneW = Zone(&leds7, 1, NUM_LEDS_W);
Zone zoneX = Zone(&leds8, 1, NUM_LEDS_X);

Zone * ZONES[NUM_ZONES] = {
  &zoneA, &zoneB, &zoneC, &zoneD, &zoneE, &zoneF, &zoneG, &zoneH,
  &zoneI, &zoneJ, &zoneK, &zoneL, &zoneM, &zoneN, &zoneO, &zoneP,
  &zoneQ, &zoneR, &zoneS, &zoneT, &zoneU, &zoneV, &zoneW, &zoneX
};

const int PINS[NUM_STRIPS] = {PIN_1, PIN_2, PIN_3, PIN_4, PIN_5, PIN_6, PIN_7, PIN_8};
const int NUM_LEDS[] = {
  NUM_LEDS_A, NUM_LEDS_B, NUM_LEDS_C, NUM_LEDS_D,
  NUM_LEDS_E, NUM_LEDS_F, NUM_LEDS_G, NUM_LEDS_H,
  NUM_LEDS_I, NUM_LEDS_J, NUM_LEDS_K, NUM_LEDS_L,
  NUM_LEDS_M, NUM_LEDS_N, NUM_LEDS_O, NUM_LEDS_P,
  NUM_LEDS_Q, NUM_LEDS_R, NUM_LEDS_S, NUM_LEDS_T,
  NUM_LEDS_U, NUM_LEDS_V, NUM_LEDS_W, NUM_LEDS_X
};

//Adafruit_NeoPixel * STRIPS[NUM_ZONES] = {&leds1, &leds2, &leds3, &leds4, &leds5, &leds6, &leds7, &leds8};

int brightness_value = 255;
int refreshMode = ZONE_MODE;

// global index for a Volume
int k = 0;
// Couleur
int red = 255;
int green = 255;
int blue = 255;

int intervall = 0;

#ifdef TEST_MODE
int testIdx = 0;
#endif

/*********************************************************************************
   Setup
 *********************************************************************************/
void setup()
{
  Serial.begin(9600);

  STRIPS[0] = &leds1;
  STRIPS[1] = &leds2;
  STRIPS[2] = &leds3;
  STRIPS[3] = &leds4;
  STRIPS[4] = &leds5;
  STRIPS[5] = &leds6;
  STRIPS[6] = &leds7;
  STRIPS[7] = &leds8;

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
    setStripColor(STRIPS[i], 0, 0, 0);

    // Faire la somme de tous les pixels
    totalNumLeds += STRIPS[i]->numPixels();

  }

  choix = 5;
}

/*********************************************************************************
   BOUCLE
 *********************************************************************************/
void loop() {
  board_blinking(500);

  // Mode TEST
  // Tester toutes les zones à chaque appuie sur la touche 'T'
#ifdef TEST_MODE
  testerZonesEtStrips();
#endif

#ifndef TEST_MODE

  // choix du scénario
  switch (choix) {
    case 1 :
      /*
         Scénario statique sans trop de changement
      */
      if (firstTime) {
        Serial.println("Scénario 1 : Blanc allumage progressif");
        firstTime = false;
        holdTime = 10;
        intervall = 2;
        refreshMode = STRIP_MODE;
      }
      if (elapsedTime > holdTime && intervall > 0) {
        setColorOneLedEvery(intervall, 100, 160, 190);
        elapsedTime -= holdTime;
        intervall--;
      }
      break;
    case 2 :
      /*
         Du bord vers le centre, puis du centre vers le bord, avec une autre couleur
         On rattaque ensuite avec la couleur avec laquelle on a fini
      */
      {
        if (firstTime) {
          red = random(0, 255);
          green = random(230, 255);
          blue = random(0, 100);
          firstTime = false;
          refreshMode = STRIP_MODE;
          holdTime = 4000;
        }

        if (elapsedTime > holdTime) {
          for (int i = 0; i < int(NUM_ZONES / 2); i++) {
            ZONES[i]->setZoneColor( red, green, blue );
            ZONES[NUM_ZONES - i]->setZoneColor( red, green, blue );
            delay(50);
          }
          red = random(0, 255);
          green = random(230, 255);
          blue = random(0, 100);
          for (int i = int(NUM_ZONES / 2); i >= 0; i--) {
            ZONES[i]->setZoneColor( red, green, blue );
            ZONES[NUM_ZONES - i]->setZoneColor( red, green, blue );
            delay(50);
          }
          elapsedTime -= holdTime;
          holdTime = random(10000, 20000);
        }

        break;
      }
    case 3 :
      if (firstTime) {
        Serial.println("Scénario #3 : Sinusoidal brightness on a led train");
        firstTime = false;
        refreshMode = STRIP_MODE;
        red = random(200, 230);
        green = random(180, 230);
        blue = random(0, 40);
      } else {
        sinusoidalTheaterChase(60, 20000.0f, red, green, blue);
      }
      break;
      /*      {
        // init for the first time
        if (firstTime) {
          Serial.println("Scénario #3, random green hues in random zones, with a randomized hild time.");
          holdTime = 1000;
          refreshMode = ZONE_MODE;
          red = random(0, 255);
          green = random(230, 255);
          blue = random(0, 100);
          firstTime = propagate_color(5000, red, green, blue);
        }

        if (elapsedTime > holdTime) {
          k = random(0, NUM_ZONES);
          red = random(0, 255);
          green = random(230, 255);
          blue = random(0, 100);
          colorWipeZone(k, red, green, blue);
          elapsedTime -= holdTime;
          holdTime = random(1000, 10000);
          Serial.print("hoding time :"); Serial.print(holdTime / 1000, DEC); Serial.println(" seconds");
        }
        break;
      }
      */
    case 4 :
      if (firstTime) {
        Serial.println("Scénario 4 : Constrained Rainbow 100 to 190");
        firstTime = false;
        refreshMode = STRIP_MODE;
      } else {
        constrainedRainbow(100, 190, 10000.0f);
      }
    case 5 :
      if (firstTime) {
        Serial.println("Scénario #5, All the lights to black.");
        firstTime = false;
        // All the light OFF
        refreshMode = STRIP_MODE;
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

  // Reading choice
  read_choice();
#endif

}


void colorWipeZone(int zoneNum, int r, int g, int b) {
  Serial.print("Volume #"); Serial.println(zoneNum);
  Serial.print(r); Serial.print(":"); Serial.print(g); Serial.print(":"); Serial.println(b);
  ZONES[zoneNum]->setZoneColor(r, g, b);
}

/*
   Propagate color in all zones
*/
boolean propagate_color(int freq, int r, int g, int b) {
  float perCent = fmod(millis(), freq) / freq;
  perCent *= 100.0f;

  float zoneA_r, zoneA_g, zoneA_b,
        zoneB_r, zoneB_g, zoneB_b,
        zoneC_r, zoneC_g, zoneC_b,
        zoneD_r, zoneD_g, zoneD_b,
        zoneE_r, zoneE_g, zoneE_b,
        zoneF_r, zoneF_g, zoneF_b;

  zoneA_r = clampMap(perCent,  0,  16,   0, r);
  zoneB_r = clampMap(perCent,  16,  32,   0, r);
  zoneC_r = clampMap(perCent,  32, 48,   0, r);
  zoneD_r = clampMap(perCent,  48,  64,   0, r);
  zoneE_r = clampMap(perCent,  64,  78,   0, r);
  zoneF_r = clampMap(perCent,  78,  96,   0, r);
  zoneA_g = clampMap(perCent,  0,  16,   0, g);
  zoneB_g = clampMap(perCent,  16,  32,   0, g);
  zoneC_g = clampMap(perCent,  32, 48,   0, g);
  zoneD_g = clampMap(perCent,  48,  64,   0, g);
  zoneE_g = clampMap(perCent,  64,  78,   0, g);
  zoneF_g = clampMap(perCent,  78,  96,   0, g);
  zoneA_b = clampMap(perCent,  0,  16,   0, b);
  zoneB_b = clampMap(perCent,  16,  32,   0, b);
  zoneC_b = clampMap(perCent,  32, 48,   0, b);
  zoneD_b = clampMap(perCent,  48,  64,   0, b);
  zoneE_b = clampMap(perCent,  64,  78,   0, b);
  zoneF_b = clampMap(perCent,  78,  96,   0, b);

  zoneA.setZoneColor(zoneA_r, zoneA_g, zoneA_b);
  zoneB.setZoneColor(zoneB_r, zoneB_g, zoneB_b);
  zoneC.setZoneColor(zoneC_r, zoneC_g, zoneC_b);
  zoneD.setZoneColor(zoneD_r, zoneD_g, zoneD_b);
  zoneE.setZoneColor(zoneE_r, zoneE_g, zoneE_b);
  zoneF.setZoneColor(zoneF_r, zoneF_g, zoneF_b);

  if (zoneF_b >= b) {
    return true;
  }
  return false;
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


#ifdef TEST_MODE
void testerZonesEtStrips() {
  if (TEST_MODE == 1) {
    char c = Serial.read();
    if (c == 'T') {
      testIdx++;
      if (testIdx >= NUM_ZONES) {
        testIdx = 0;
      }
      Serial.print("============= Begin Test Zone #");
      Serial.print(testIdx);
      Serial.println(" ===============");
    }
    Serial.print("Testing Zone #"); Serial.print(testIdx);
    Serial.print(NUM_LEDS[testIdx]); Serial.println(" leds");Serial.print(", ");
    SetAllZonesToColor(0, 0, 0);
    showAllZones();
    delay(500);
    ZONES[testIdx]->setZoneColor(255, 255, 0);
    showAllZones();
    delay(500);
  }
  else {
    char c = Serial.read();
    if (c == 'T') {
      testIdx++;
      if (testIdx >= NUM_STRIPS) {
        testIdx = 0;
      }
      Serial.print("============= Begin Test Strip #");
      Serial.print(testIdx);
      Serial.println(" ===============");
    }
    Serial.print("Testing Strip #"); Serial.print(testIdx);Serial.print(", ");
    Serial.print(STRIPS[testIdx]->numPixels());
    Serial.println(" leds");
    SetAllStripsToColor(0, 0, 0);
    showAllStrips();
    delay(500);
    setStripColor(STRIPS[testIdx], 255, 255, 0);
    showAllStrips();
    delay(500);
  }
}
#endif

