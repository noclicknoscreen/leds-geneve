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

/*********************************************************************************
   Setup
 *********************************************************************************/
void setup()
{
  Serial.begin(9600);

  // STRIPS = {&leds1, &leds2, &leds3, &leds4, &leds5, &leds6, &leds7, &leds8};
  STRIPS[0] = &leds1;
  STRIPS[1] = &leds2;
  STRIPS[2] = &leds3;
  STRIPS[3] = &leds4;
  STRIPS[4] = &leds5;
  STRIPS[5] = &leds6;
  STRIPS[6] = &leds7;
  STRIPS[7] = &leds8;


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

  constrainedRainbow(10, 70, 10000.0f);
  showAllStrips();

}


