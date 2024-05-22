#define ROOM_TEMP 32

#include <Arduino.h>

// light sensor
#include <hp_BH1750.h>
hp_BH1750 BH1750;

// temperature sensor
#include <DHT11.h>
DHT11 dht11(12); // pin 12 for temperature sensor

void setup()
{
  Serial.begin(9600);

  // initialize light sensor
  bool avail = BH1750.begin(BH1750_TO_GROUND);
  if (!avail) {
    Serial.println("No BH1750 sensor found!");
    while (true) {};                                        
  }

  // led initialize
  int i;
  for (i=0; i<=7; i++)
    pinMode(i, OUTPUT);

  // next: #TODO
  pinMode(13, OUTPUT);
}

void loop()
{
  digitalWrite(13, HIGH);

  // light sensor
  BH1750.start();
  float lux = BH1750.getLux();
  Serial.print("Light Intensity: ");
  Serial.println(lux);
  int i;

  if (lux > 1000) {
    for (i=0; i<=6; i++)
      digitalWrite(i, LOW);
    digitalWrite(7, HIGH);
  }
  else if (lux > 500) {
    for (i=0; i<=5; i++)
      digitalWrite(i, LOW);
    for (i=6; i<=7; i++)
      digitalWrite(i, HIGH);
  }
  else if (lux > 100) {
    for (i=0; i<=4; i++)
      digitalWrite(i, LOW);
    for (i=5; i<=7; i++)
      digitalWrite(i, HIGH);
  }
  else if (lux > 50) {
    for (i=0; i<=3; i++)
      digitalWrite(i, LOW);
    for (i=4; i<=7; i++)
      digitalWrite(i, HIGH);
  }
  else if (lux > 20) {
    for (i=0; i<=2; i++)
      digitalWrite(i, LOW);
    for (i=3; i<=7; i++)
      digitalWrite(i, HIGH);
  }
  else {
    digitalWrite(0, HIGH);
    for (i=1; i<=7; i++)
      digitalWrite(i, HIGH);
  }

  // temperature sensor
  int temperature = dht11.readTemperature();
  if (temperature != DHT11::ERROR_CHECKSUM && temperature != DHT11::ERROR_TIMEOUT) {
        if (temperature != ROOM_TEMP) {
          Serial.print("Temperature changed to ");
          Serial.println(temperature);
        }
    } else {
        Serial.println(DHT11::getErrorString(temperature));
    }
}
