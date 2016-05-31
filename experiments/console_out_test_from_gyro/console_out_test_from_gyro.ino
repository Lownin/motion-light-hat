#include <Wire.h>
#include <L3G.h>

L3G gyro;

float pos=0;
unsigned long last_time, this_time, diff_time;

void setup() {
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
}

void loop() {
  gyro.read();
  this_time=millis();
  diff_time=this_time-last_time;
  last_time=this_time;
  if (gyro.g.z<1100)
    {pos-=diff_time*gyro.g.z/92160;}
  if (gyro.g.z>1100)
    {pos+=diff_time*gyro.g.z/92160;}
  Serial.print("Z: "); Serial.print((int)gyro.g.z);   Serial.print(" ");
  Serial.print("POS: "); Serial.println((float)pos);   Serial.print(" ");
  delay(10);
}
