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
#include "FastLED.h"

FASTLED_USING_NAMESPACE

// Nombre de zones
#define NUM_ZONES 25

// Signaux de pilotage des zones
# define PIN_1 6
# define PIN_2 16
# define PIN_3 17
# define PIN_4 20
# define PIN_5 22
# define PIN_6 23
# define PIN_7 24
# define PIN_8 25

// Definir les 6 zones a eclairer
// Valeurs réelles
#define NUM_LEDS_3B 43
#define NUM_LEDS_2C 33
#define NUM_LEDS_2B 19
#define NUM_LEDS_1A 54
#define NUM_LEDS_4A 34
#define NUM_LEDS_3A 35
#define NUM_LEDS_4A 7
#define NUM_LEDS_4C 38
#define NUM_LEDS_4B 0 // ? Non défini ?
#define NUM_LEDS_4B 4
#define NUM_LEDS_4C 51
#define NUM_LEDS_3A 43
#define NUM_LEDS_4A 23
#define NUM_LEDS_1A 41
#define NUM_LEDS_2B 18
#define NUM_LEDS_2C 30
#define NUM_LEDS_2A 7

const int PINS[] = {PIN_1, PIN_2, PIN_3, PIN_4, PIN_5, PIN_6, PIN_7, PIN_8};
const int NUM_LEDS[] = {
  NUM_LEDS_3B, NUM_LEDS_2C, NUM_LEDS_2B, NUM_LEDS_1A,
  NUM_LEDS_4A, NUM_LEDS_3A, NUM_LEDS_G, NUM_LEDS_H,
  NUM_LEDS_I, NUM_LEDS_J, NUM_LEDS_K, NUM_LEDS_L,
  NUM_LEDS_M, NUM_LEDS_N, NUM_LEDS_O, NUM_LEDS_P,
  NUM_LEDS_Q, NUM_LEDS_Q2, NUM_LEDS_R, NUM_LEDS_S,
  NUM_LEDS_T, NUM_LEDS_U, NUM_LEDS_V, NUM_LEDS_W,
  NUM_LEDS_X
};

CRGB leds0[NUM_LEDS_A];
CRGB leds1[NUM_LEDS_B];
CRGB leds2[NUM_LEDS_C];
CRGB leds3[NUM_LEDS_D];
CRGB leds4[NUM_LEDS_E];
CRGB leds5[NUM_LEDS_F];
CRGB leds6[NUM_LEDS_G];
CRGB leds7[NUM_LEDS_H];
CRGB leds8[NUM_LEDS_I];
CRGB leds9[NUM_LEDS_J];
CRGB leds10[NUM_LEDS_K];
CRGB leds11[NUM_LEDS_L];
CRGB leds12[NUM_LEDS_M];
CRGB leds13[NUM_LEDS_N];
CRGB leds14[NUM_LEDS_O];
CRGB leds15[NUM_LEDS_P];
CRGB leds16[NUM_LEDS_Q];
CRGB leds17[NUM_LEDS_Q2];
CRGB leds18[NUM_LEDS_R];
CRGB leds19[NUM_LEDS_S];
CRGB leds20[NUM_LEDS_T];
CRGB leds21[NUM_LEDS_U];
CRGB leds22[NUM_LEDS_V];
CRGB leds23[NUM_LEDS_W];
CRGB leds24[NUM_LEDS_X];


CRGB * leds[] = {
  leds0, leds1, leds2, leds3, leds4, leds5, leds6, leds7, leds8, leds9, leds10,
  leds11, leds12, leds13, leds14, leds15, leds16, leds17, leds18, leds19, leds20,
  leds21, leds22, leds23, leds24
};
// Variable de couleur.
CRGB couleur;
CRGB couleur_zones[NUM_ZONES];

// Choix du scénario
char choix = '1';

// Pour les calculs de pulsations
float heartBeat;
float heartFreq = 10000;

int brightness_value = 40; // Between 0 and 100 %

#define FRAMES_PER_SECOND 120

/*********************************************************************************
   Framework FastLED
*********************************************************************************/
//void setPixel(int Pixel, byte red, byte green, byte blue, int NumZone) {
void setPixel(int Pixel, CRGB c, int NumZone) {
  // FastLED
  leds[NumZone][Pixel] = c;
}

#include "scenarii-leds.h"

/*********************************************************************************
   Setup
 *********************************************************************************/
void setup()
{
  Serial.begin(9600);
  // Correction intéressantes : Candle (intime, plus sur les rouges), Tungsten40W, Tungsten100W, ClearBlueSky (tire sur le vert/bleu)
  FastLED.addLeds<NEOPIXEL, PIN_ZONE_A>(leds0, NUM_LEDS[0]).setCorrection( Tungsten100W );
  FastLED.addLeds<NEOPIXEL, PIN_ZONE_B>(leds1, NUM_LEDS[1]).setCorrection( Tungsten100W );
  FastLED.addLeds<NEOPIXEL, PIN_ZONE_C>(leds2, NUM_LEDS[2]).setCorrection( Tungsten100W );
  FastLED.addLeds<NEOPIXEL, PIN_ZONE_D>(leds3, NUM_LEDS[3]).setCorrection( Tungsten100W );
  FastLED.addLeds<NEOPIXEL, PIN_ZONE_E>(leds4, NUM_LEDS[4]).setCorrection( Tungsten100W );
  FastLED.addLeds<NEOPIXEL, PIN_ZONE_F>(leds5, NUM_LEDS[5]).setCorrection( Tungsten100W );
  //  zoneA = LedsZone(PIN_ZONE_A, 1, ZONE_BG, ZONE_CW, 0, NUM_LEDS[0], 29, 14).init();
  //  zoneB = LedsZone(PIN_ZONE_B, 2, ZONE_BG, ZONE_CW, 0, NUM_LEDS[1], 22, 14).init();
  //  zoneC = LedsZone(PIN_ZONE_C, 3, ZONE_BG, ZONE_CW, 0, NUM_LEDS[2], 37, 14).init();
  //  zoneD = LedsZone(PIN_ZONE_D, 4, ZONE_BG, ZONE_CW, 0, NUM_LEDS[3], 30, 14).init();
  //  zoneE = LedsZone(PIN_ZONE_E, 5, ZONE_BG, ZONE_CW, 0, NUM_LEDS[4], 28, 14).init();
  //  zoneF = LedsZone(PIN_ZONE_F, 6, ZONE_BG, ZONE_CW, 0, NUM_LEDS[5], 28, 14).init();

  FastLED.setBrightness(brightness_value);

  tout_eteindre();

  // Init de toutes les zones
  for (uint8_t i = 0; i < NUM_ZONES; i++) {
    // couleur au hasard
    allColor(randomColor(), i);
    delay(30);
    showStrip();
  }

  // Faire un tableau de couleurs pour certains scénarii
  for (int n = 0; n < NUM_ZONES; n++) {
    couleur_zones[n] = randomColor();
  }

  choix = '0';
}

/*********************************************************************************
   BOUCLE
 *********************************************************************************/
void loop() {
  if (Serial.available()) {
    choix = Serial.read();
    Serial.print("\nNouveau choix :"); Serial.println(choix);
  }

  // Choix d'une zone au hasard
  uint8_t k = random(0, NUM_ZONES);

  // Choix d'un temps d'arrêt aléatoire compris entre 50 ms et 5000ms
  uint32_t waiting_time = random(500, 10000);

  // Choix d'une couleur au hasard
  couleur = randomColor();

  // Choix de la brightness
  brightness_value = random(0, 100);

  // Debug values
  Serial.println("------------------------------------------------");
  Serial.print("zone : "); Serial.println(k);
  Serial.print("Scénario : "); Serial.println(choix);
  Serial.print("r:"); Serial.print(couleur.r);
  Serial.print(", g:"); Serial.print(couleur.g);
  Serial.print(", b:"); Serial.println(couleur.b);
  Serial.print("Brightness : "); Serial.println(brightness_value);
  Serial.print("FPS : "); Serial.println(FastLED.getFPS(), DEC);
  Serial.print("Temps d'attente : "); Serial.println(waiting_time);

  // choix du scénario
  switch (choix) {
    case '0' : // Scénario de test des zones
      FastLED.setBrightness(25);
      tout_eteindre();
      Serial.println("------------------------ Test du paramétrage des zones ------------------------");
      for (int i = 0; i < NUM_ZONES; i++) {
        Serial.print("zone : "); Serial.println(i);
        testZone(randomColor(), 125, 1, i);
      }
      tout_eteindre();
      FastLED.setBrightness(25);
      // Test de toutes les zones comme un seul ruban
      for (int n = 0; n < NUM_ZONES; n++) {
        for (int i = 0; i < NUM_LEDS[n]; i++) {
          leds[n][i] = 0xf0f0f0; // 0xffad08; //couleur_zones[n];
          showStrip();
        }
        delay(MEDIUM);
      }
      tout_eteindre();
      break;
    case '1' :
      // Changer une zone au hasard
      colorWipe(couleur, 50, k);
      // temps d'arrêt aléatoire compris entre 50 ms et 500ms
      delay(waiting_time);
      break;
    case '2' :
      // faire scintiller des leds
      SnowSparkle(CRGB::Yellow, 50, random(100, 1000), k);
      break;
    case '3' :
      // Disolve color to black
      allRandom(k);
      delay(waiting_time / 2);
      disolve(15, 100, 50, k);
      delay(waiting_time / 2);
      break;
    case '4' :  // Flash
      colorWipe(couleur, 50, k);
      delay(waiting_time / 2);
      flash(couleur, 10, 50, k);
      delay(waiting_time / 2);
      break;
    case '5' :  // ColorWipe With Direction
      colorWipe(couleur, 50, k, BACKWARD);
      delay(waiting_time);
      break;
    case '6' :  // Rainbaw
      for (int i = 0; i < NUM_ZONES; i++) {
        rainbow(2, 30, i);
      }
      break;
    case '7' :
      // theaterChase
      theaterChase(couleur, 3, 50, k);
      // theaterChaseRainbow
      theaterChaseRainbow(1, 50, k);
      break;
    case '8' :
      {
        // theaterChaseRainbow
        CRGB oldColor = leds[k][0];
        lightning(CRGB::White, 20, 50, MEDIUM, k);
        allColor(oldColor, k);
        break;
      }
    case '9' :  // cyclon
      for (int i = 0; i < NUM_ZONES; i++) {
        cylon(couleur, 4, 30, i);
      }
      break;
    case 'A'  :
      {
        CRGB couleur2 = randomColor();
        stripes(couleur, couleur2, 3, k);
        delay(waiting_time);
        break;
      }
    case 'B' :
      {
        //NewKITT(couleur, 4, 50, 50, k);
        CenterToOutside(couleur, NUM_LEDS[k] % 4, 50, 50, k);
        OutsideToCenter(couleur, NUM_LEDS[k] % 4, 50, 50, k);
        allColor(couleur, k);
      }
    case 'C' :
      {
        // Toutes les zones : éclaraige pulsant sinusoïdale
        int total_led = 0;
        // On fait 100 pulsations
        for (int pulse = 10; pulse < 110; pulse++) {
          // --- Calculation of heartBeat
          heartBeat = fmod(millis(), heartFreq) / heartFreq;
          heartBeat = 0.5f * (sin(TWO_PI * heartBeat) + 1);
          brightness_value = 100.0f * heartBeat;
          FastLED.setBrightness(brightness_value);
          for (int n = 0; n < NUM_ZONES; n++) {
            total_led += NUM_LEDS[n];
            for (int i = 0; i < NUM_LEDS[n]; i++) {
              leds[n][i] = 0xffad08; //couleur_zones[n];
            }
          }
          showStrip();
          //delay(FASTER);
        }
        //couleur_zones[random(NUM_ZONES)] = randomColor();
      }
  }

}

void tout_eteindre() {
  // Tout à noir
  for (uint16_t i = 0; i < NUM_ZONES; i++) {
    allColor(CRGB::Black, i);
  }
  showStrip();

}

