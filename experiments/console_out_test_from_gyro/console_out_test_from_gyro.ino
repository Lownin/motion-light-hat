#include <Wire.h>
#include <L3G.h>

L3G gyro;

int pos=0;
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
  if (gyro.g.x>200||gyro.g.x<-200)
    {pos+=diff_time*gyro.g.x;}
  Serial.print("Z: "); Serial.print(gyro.g.z);   Serial.print(" ");
  Serial.print("POS: "); Serial.println(pos);   Serial.print(" ");
  delay(10);
}
