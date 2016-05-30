#include <Wire.h> 
#include <Adafruit_L3GD20.h>

// By default, uses I2C
Adafruit_L3GD20 gyro;

float pos=0;
unsigned long last_time, this_time, diff_time;
//  int16_t gyro.data.x = 0;
//  int16_t gyro.data.y = 0;
//  int16_t gyro.data.z = 0;

void setup() 
{
  while (!Serial && (millis ()  <= 3000)) ;
  Serial.begin(9600);
  if (!gyro.begin(gyro.L3DS20_RANGE_500DPS))
  {
    Serial.println("Oops ... unable to initialize the L3GD20. Check your wiring!");
    while (1);
  }
  last_time=millis();
}

void loop() 
{
  gyro.read();  
  this_time=millis();
  diff_time=this_time-last_time;
  last_time=this_time;
  if (gyro.data.z<1100)
    {pos-=diff_time*gyro.data.z/92160;}
  if (gyro.data.z>1100)
    {pos+=diff_time*gyro.data.z/92160;}
  //Serial.print("X: "); Serial.print((int)gyro.data.x);   Serial.print(" ");
  //Serial.print("Y: "); Serial.print((int)gyro.data.y);   Serial.print(" ");
//  Serial.print("Z: "); Serial.print((int)gyro.data.z);   Serial.print(" ");
//  Serial.print("POS: "); Serial.println((float)pos);   Serial.print(" ");
  Serial.print("Z (0x0F): "); Serial.print((int)gyro.data.z & 0x0F);   Serial.print(" ");
  Serial.print("Z (>> 8): "); Serial.println((int)gyro.data.z >> 8);   Serial.print(" ");
  delay(10);
}
