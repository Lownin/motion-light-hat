#include <Wire.h>
#include <L3G.h>
#include <Adafruit_NeoPixel.h>

#define PIN 7

L3G gyro;

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(39, PIN, NEO_GRB + NEO_KHZ800);

// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.

int led = 13;
int pos=0;
int pos256=0;
unsigned long last_time, this_time, diff_time;

void setup() {
  pinMode(9, INPUT_PULLUP);
  // initialize the digital pin connected to teensy LED as an output.
  pinMode(led, OUTPUT);
  digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
  while (!Serial && (millis ()  <= 3000)) ;
  Serial.begin(9600);
  Wire.begin();

  if (!gyro.init())
  {
    Serial.println("Failed to autodetect gyro type!");
    while (1);
  }
  last_time=millis();
  gyro.enableDefault();
  strip.setBrightness(30);
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}

void loop() {
  if (digitalRead(9) == HIGH) {
    digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
    rainbowCycle();
  } 
  else {
    digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
    Serial.println("Button pressed!!!");
  }
}

void rainbowCycle() {
  uint16_t i;
  gyro.read();
  this_time=millis();
  delay(10);
  diff_time=this_time-last_time;
  last_time=this_time;
  if (gyro.g.y>0 ||gyro.g.y<-700)
    {pos-=diff_time*gyro.g.y*5;}
  pos256=pos/156250;
  Serial.print("Y: "); Serial.print(gyro.g.y);   Serial.print(" ");
  Serial.print("POS: "); Serial.print(pos);   Serial.print(" ");
  Serial.print("POS256: "); Serial.println(pos256);   Serial.print(" ");

  for(i=0; i< strip.numPixels(); i++) {
    strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + pos256) & 255));
  }

  strip.show();
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
