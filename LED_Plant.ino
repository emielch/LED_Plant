#include <Colore.h>
#include <OctoWS2811_Dither.h>

#define BEAM_AM 40
Beam beams[BEAM_AM];

const int ledsPerStrip = 25;
DMAMEM int displayMemory[ledsPerStrip * 6];
COL_RGB drawingMemory[ledsPerStrip * 8];
COL_RGB writingMemory[ledsPerStrip * 8];
const int config = WS2811_GRB | WS2811_800kHz;
OctoWS2811_Dither leds(ledsPerStrip, displayMemory, writingMemory, drawingMemory, config);

uint16_t stem[] = {0, 25, 50, 75, 100, 125, 150, 175, 1, 26, 51, 76, 101, 126, 151, 176, 2, 27, 52, 77, 102, 127, 152, 177, 3, 28, 53, 78, 103, 128, 153, 178, 4, 29, 54, 79, 104, 129, 154, 179, 5, 30, 55, 80, 105, 130, 155, 180, 6, 31, 56, 81, 106, 131, 156, 181, 7, 32, 57, 82, 107, 132, 157, 182, 8, 33, 58, 83, 108, 133, 158, 183, 9, 34, 59, 84, 109, 134, 159, 184, 10, 35, 60, 85, 110, 135, 160, 185, 11, 36, 61, 86, 111, 136, 161, 186, 12, 37, 62, 87, 112, 137, 162, 187};
uint16_t flower[] = {13, 38, 63, 88, 113, 138, 163, 188, 14, 39, 64, 89, 114, 139, 164, 189, 15, 40, 65, 90, 115, 140, 165, 190, 16, 41, 66, 91, 116, 141, 166, 191, 17, 42, 67, 92, 117, 142, 167, 192, 18, 43, 68, 93, 118, 143, 168, 193, 19, 44, 69, 94, 119, 144, 169, 194, 20, 45, 70, 95, 120, 145, 170, 195, 21, 46, 71, 96, 121, 146, 171, 196, 22, 47, 72, 97, 122, 147, 172, 197, 23, 48, 73, 98, 123, 148, 173, 198, 24, 49, 74, 99, 124, 149, 174, 199 };


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


void setup() {
  Serial.begin(9600);
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
    Color c(random(-180, 180), 100, 15, HSB_MODE);
    currBeam = seg[0].setBeam(false, random(5, 10), PIXEL_SPD, random(10, 20), c);
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
