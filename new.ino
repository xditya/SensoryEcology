#define ROOM_TEMP 27

#define PIN_MQ135 A2
// #define TEMP_PIN 10
// #define DHTTYPE DHT11
#define RELAY_PIN_1 13 // temperature - fan relay
#define RELAY_PIN_2 12 // gas - fan relay

#include <Arduino.h>
// #include <SoftwareSerial.h>

// light sensor
#include <hp_BH1750.h>
hp_BH1750 BH1750;

// gas detector
#include <MQ135.h>
MQ135 mq135_sensor(PIN_MQ135);

// temperature sensor
// #include "DHT.h"
// DHT dht(TEMP_PIN, DHTTYPE);

// SoftwareSerial espSerial(10, 11); // RX, TX

void setup()
{
  Serial.begin(9600);
  // espSerial.begin(57600);

  Serial.println("Started.");

  // initialize light sensor
  bool avail = BH1750.begin(BH1750_TO_GROUND);
  if (!avail)
      Serial.println("No BH1750 sensor found!");

  // initialze temperature sensor
  // dht.begin();

  // led initialize
  int i;
  for (i = 3; i <= 9; i++)
      pinMode(i, OUTPUT);

  // relays
  pinMode(RELAY_PIN_1, OUTPUT);
  pinMode(RELAY_PIN_2, OUTPUT);

  // turn fans off
  digitalWrite(RELAY_PIN_1, HIGH);
}

void loop()
{
  // light sensor
  BH1750.start();
  float lux = BH1750.getLux();
  Serial.print("Light Intensity: ");
  Serial.println(lux);
  delay(2000);

  int i;
  if (lux > 1000)
  {
      for (i = 3; i <= 9; i++)
          digitalWrite(i, LOW);
      digitalWrite(9, HIGH);
  }
  else if (lux > 500)
  {
      for (i = 3; i <= 7; i++)
          digitalWrite(i, LOW);
      for (i = 8; i <= 9; i++)
          digitalWrite(i, HIGH);
  }
  else if (lux > 100)
  {
      for (i = 3; i <= 7; i++)
          digitalWrite(i, LOW);
      for (i = 8; i <= 10; i++)
          digitalWrite(i, HIGH);
  }
  else if (lux > 50)
  {
      for (i = 3; i <= 6; i++)
          digitalWrite(i, LOW);
      for (i = 7; i <= 10; i++)
          digitalWrite(i, HIGH);
  }
  else if (lux > 20)
  {
      for (i = 3; i <= 5; i++)
          digitalWrite(i, LOW);
      for (i = 6; i <= 10; i++)
          digitalWrite(i, HIGH);
  }
  else
  {
      digitalWrite(0, HIGH);
      for (i = 4; i <= 10; i++)
          digitalWrite(i, HIGH);
  }

  // // temperature sensor
  // static float lastTemp = 0; // save last know temperature to use incase read fails
  // float t = dht.readTemperature();
  // static bool last = false;
  // if (isnan(t)) {
  //   // Serial.println(F("Failed to read from DHT sensor!"));
  //   last = true;
  //   return;
  // }
  // else {
  //   Serial.print(t);
  //   lastTemp = t;
  // }
  // Serial.print("Temperature: ");
  // Serial.print(t);
  // if (last == true)
  //   Serial.print(" (last)");
  // Serial.println();

  // gas detector
  float ppm = mq135_sensor.getPPM();
  Serial.print("Air Quality: ");
  Serial.print(ppm);
  Serial.println("ppm");
  // LOW = fan on & vice versa
  if (ppm > 100)
  {
      Serial.println(">100");
      digitalWrite(RELAY_PIN_2, LOW);
      delay(500); // dont spam the relay
  }
  else
  {
      digitalWrite(RELAY_PIN_2, HIGH);
  }

  // espSerial.println(lux); //, ppm);
}
