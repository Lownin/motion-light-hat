#include <Adafruit_NeoPixel.h>
#define PIN 6
#include <Wire.h> 
#include <Adafruit_L3GD20.h>
Adafruit_L3GD20 gyro;

float pos=0;
unsigned long last_time, this_time, diff_time;

// Parameter 1 = number of pixels in strip
// Parameter 2 = pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(60, PIN, NEO_GRB + NEO_KHZ800);


void setup() {
  Serial.begin(9600);
  while (!Serial && (millis ()  <= 3000)) ;
  if (!gyro.begin(gyro.L3DS20_RANGE_250DPS))
    {
    Serial.println("Oops ... unable to initialize the L3GD20. Check your wiring!");
    while (1);
  }
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  strip.setBrightness(10);
  last_time=millis();
}

void loop() {
//  colorClear(strip.Color(0, 0, 0), 50); // Off
//  colorWipe(strip.Color(255, 0, 0), 1); // Red
//  colorWipe(strip.Color(0, 255, 0), 1); // Blue
//  colorWipe(strip.Color(0, 0, 255), 1); // Green
  rainbowCycle(500);
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j, g;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {
      gyro.read();
      this_time=millis();
      diff_time=this_time-last_time;
      last_time=this_time;
      if (gyro.data.z<1100)
        {pos+=(diff_time+20)*gyro.data.z/92160;}
      if (gyro.data.z>1100)
        {pos-=diff_time*gyro.data.z/92160;}
      g=((int)pos);
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + g) & 255));
//      Serial.print("Z: "); Serial.println((int)gyro.data.z); Serial.print(" ");
      Serial.print("POS: "); Serial.println((int)g); Serial.print(" ");
    }
    strip.show();
//    delay(10);
  }
}

// Eliminate all color for fun!
void colorClear(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
