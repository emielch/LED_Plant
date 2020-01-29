#include <Colore.h>
#include <OctoWS2811_Dither.h>
#include <EEPROM.h>

#define BEAM_AM 40
Beam beams[BEAM_AM];

const int ledsPerStrip = 25;
DMAMEM int displayMemory[ledsPerStrip * 6];
COL_RGB drawingMemory[ledsPerStrip * 8];
COL_RGB writingMemory[ledsPerStrip * 8];
const int config = WS2811_GRB | WS2811_800kHz;
OctoWS2811_Dither leds(ledsPerStrip, displayMemory, writingMemory, drawingMemory, config);

uint16_t stem[] = {175,25,50,75,100,125,150,0,176,26,51,76,101,126,151,1,177,27,52,77,102,127,152,2,178,28,53,78,103,128,153,3,179,29,54,79,104,129,154,4,180,30,55,80,105,130,155,5,181,31,56,81,106,131,156,6,182,32,57,82,107,132,157,7};
uint16_t flower[] = {183,33,58,83,108,133,158,8,184,34,59,84,109,134,159,9,185,35,60,85,110,135,160,10,186,36,61,86,111,136,161,11,187,37,62,87,112,137,162,12,188,38,63,88,113,138,163,13,189,39,64,89,114,139,164,14,190,40,65,90,115,140,165,15,191,41,66,91,116,141,166,16,192,42,67,92,117,142,167,17,193,43,68,93,118,143,168,18,194,44,69,94,119,144,169,19,195,45,70,95,120,145,170,20,196,46,71,96,121,146,171,21,197,47,72,97,122,147,172,22,198,48,73,98,123,148,173,23,199,49,74,99,124,149,174,24};


Segment seg[] = {
  Segment(sizeof(stem) / sizeof(uint16_t), stem),
  Segment(sizeof(flower) / sizeof(uint16_t), flower),
  //   Segment(0,ledsPerStrip * 8 - 1),  // 0
  //   Segment(0,ledsPerStrip-1),  // 0
  //   Segment(ledsPerStrip,ledsPerStrip*2-1),  // 0
  //   Segment(ledsPerStrip*2,ledsPerStrip*3-1),  // 0
  //   Segment(ledsPerStrip*3,ledsPerStrip*4-1),  // 0
  //   Segment(ledsPerStrip*4,ledsPerStrip*5-1),  // 0
  //   Segment(ledsPerStrip*5,ledsPerStrip*6-1),  // 0
  //   Segment(ledsPerStrip*6,ledsPerStrip*7-1),  // 0
  //   Segment(ledsPerStrip*7,ledsPerStrip*8-1),  // 0
};

const int LED_AM = ledsPerStrip * 8;
byte segAm = sizeof(seg) / sizeof(Segment);
Colore colore( LED_AM, seg, segAm, beams, BEAM_AM, &set_ledLib, &get_ledLib, &show_ledLib, &reset_ledLib );

Beam* currBeam;

unsigned int randomSeedN = 0;
void updateRandomSeed(){
  EEPROM.get(0,randomSeedN);
  randomSeedN++;
  EEPROM.put(0,randomSeedN);
  randomSeed(randomSeedN);
}

void setup() {
  Serial.begin(9600);
  updateRandomSeed();
  leds.begin();

  // let the LEDs do something...
  //seg[0].setRainbow(0.5,1,40);
  //seg[0].setBlendMode(ADD);

  //  Color c(2,0,0,RGB_MODE);
  //  seg[1].setStaticColor(c);
}

elapsedMillis sincePulse = 0;
int nextPulse = 0;

elapsedMillis sincePrint = 0;



void loop() {
  if (sincePulse > nextPulse) {
    Color c(random(-180, 180), min(100,random(85,170)), 20, HSB_MODE);
    currBeam = seg[0].setBeam(false, random(400, 900)*0.01, PIXEL_SPD, random(15, 25), c);
    sincePulse = 0;
    nextPulse = 60000;
  }

  if (sincePrint > 1000) {
    sincePrint = 0;
    printFramerate();
  }

  colore.update(true);

  if (currBeam != NULL && currBeam->posFactor > 0.9) {
    seg[1].setBeam(false, 15, PIXEL_SPD, 300, currBeam->color);
    currBeam = NULL;
    nextPulse = 0;
  }
}


void printFramerate() {
  Serial.print("FrameRate: ");
  Serial.println(colore.getFPS()); // print framerate
}


/* ---- Library Interface Functions ---- */

void set_ledLib(int pixel, byte r, byte g, byte b) {
  leds.setPixel(pixel, r, g, b);
}

void show_ledLib() {
  leds.show();
}

uint32_t bufsize = ledsPerStrip * 24;
void reset_ledLib() {
  memset(drawingMemory, 0, bufsize);
}

Color get_ledLib(int pixel) {
  Color pixelCol(drawingMemory[pixel].r, drawingMemory[pixel].g, drawingMemory[pixel].b, RGB_MODE);
  return pixelCol;
}
