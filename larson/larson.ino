/*  Larson.ino - Larson Scanner
    Using OctoWS2811 from 
    http://www.pjrc.com/teensy/td_libs_OctoWS2811.html
    Copyright (c) 2013 Heron Ordoñez
*/

#include <OctoWS2811.h> 
// Begin configuration
#define DURATION 750 //time to make a full scan
#define STRIPLENGTH  9
#define SPREAD  2 //the "fade"
int colorTable[3] = {0xff0000, 0x110000,0x030000}; 

/* The color table has the values for the scanner colors. the 0 position
is for the "main" pixel and it is required. If you have a "spread" value, add
as many values defining the colors you want. These colors will be applied to both
sides of the fading. Try it out!
*/

// End configuration

int pause = DURATION / STRIPLENGTH;

DMAMEM int displayMemory[STRIPLENGTH*6];
int drawingMemory[STRIPLENGTH*6];

const int config =  WS2811_GRB | WS2811_800kHz ;

OctoWS2811 leds(STRIPLENGTH, displayMemory, drawingMemory, config);

void setup() {
  leds.begin();
  leds.show();
}

void loop() {
  int i;
   
  for( i = 0; i < STRIPLENGTH -1  ; i++) {
    setFrame( i );
  }
  for (i = STRIPLENGTH - 1 ;i > 0; i--) {
   setFrame(i); 
  }
}

void setFrame(int pos)
{
  Serial.println("Frame at" + String(pos));
  int pixelColor = 0;
  
  for (int i=0; i <= STRIPLENGTH; i++) {
    if(i >= (pos - SPREAD) && i <= (pos + SPREAD)) {
      pixelColor = colorTable[abs(pos - i)];
    } else  {
      pixelColor = 0x000000;
    }
    leds.setPixel(i, pixelColor);
  }
  leds.show();
  delay(pause);
}
