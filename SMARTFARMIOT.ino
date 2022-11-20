#define BLYNK_TEMPLATE_ID "TMPLyEFZVWMH"
#define BLYNK_DEVICE_NAME "SMART PLANT"
#define BLYNK_FIRMWARE_VERSION "0.1.0"
#include <Wire.h>
#include <BH1750.h>
#define BLYNK_PRINT Serial
#define APP_DEBUG
#define USE_NODE_MCU_BOARD
#include "BlynkEdgent.h"
#include "DHT.h"
#include <Servo.h>
Servo servo;
#define DHTPIN D5
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
float t, h;
float settemp = 0, sethumi = 0, setlight = 0;
BH1750 lightMeter;
float lux = lightMeter.readLightLevel();
BlynkTimer timer;
byte amm = 0;
BLYNK_WRITE(V8)
{
  amm = param.asInt();
}
void autotemphumi() {
  h = dht.readHumidity();
  t = dht.readTemperature();
  Blynk.virtualWrite(V0, h);
  Blynk.virtualWrite(V1, t);
  if (amm == 1) {
    if (t > settemp && h < sethumi) {
      digitalWrite(D0, HIGH);
    }
    else {
      digitalWrite(D0, LOW);
    }
  }
}
void autobotlight()
{
  float lux = lightMeter.readLightLevel();
  Blynk.virtualWrite(V3, lux);
  if (amm == 1) {
    if (lux < setlight) {
      servo.write(132);
      delay(1000);
    }
    else {
      servo.write(8);
      delay(1000);
    }
  }
}
BLYNK_WRITE(V5)
{
  settemp = param.asInt();
}
BLYNK_WRITE(V6)
{
  sethumi = param.asInt();
}
BLYNK_WRITE(V7)
{
  setlight = param.asInt();
}
BLYNK_WRITE(V4)
{
  if (param.asInt() == 0)

  {
    servo.write(132);
  }
  else
  {
    servo.write(8);
  }//off8 on132
}
BLYNK_WRITE(V2)
{
  int val = param.asInt();
  digitalWrite(D0, val);
}
void setup()
{
  pinMode(D0, OUTPUT);
  digitalWrite(D0, LOW);
  Wire.begin();
  lightMeter.begin();
  dht.begin();
  timer.setInterval(1000L, autobotlight);
  timer.setInterval(1000L, autotemphumi);
  Serial.begin(115200);
  delay(100);
  servo.attach(D8);
  BlynkEdgent.begin();
}
void loop()
{
  BlynkEdgent.run();
  timer.run();
}
