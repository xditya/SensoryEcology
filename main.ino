#define ROOM_TEMP 32
#define PIN_MQ135 A0

#include <Arduino.h>

// light sensor
#include <hp_BH1750.h>
hp_BH1750 BH1750;

// temperature sensor
#include <DHT11.h>
DHT11 dht11(12); // pin 12 for temperature sensor

// gas detector
#include <MQ135.h>
MQ135 mq135_sensor(PIN_MQ135);

void setup()
{
    Serial.begin(9600);

    // initialize light sensor
    bool avail = BH1750.begin(BH1750_TO_GROUND);
    if (!avail)
    {
        Serial.println("No BH1750 sensor found!");
        while (true)
        {
        };
    }

    // led initialize
    int i;
    for (i = 3; i <= 10; i++)
        pinMode(i, OUTPUT);

    // relay - temperature
    pinMode(13, OUTPUT);
}

void loop()
{
    // light sensor
    BH1750.start();
    float lux = BH1750.getLux();
    Serial.print("Light Intensity: ");
    Serial.println(lux);
    int i;

    if (lux > 1000)
    {
        for (i = 3; i <= 9; i++)
            digitalWrite(i, LOW);
        digitalWrite(10, HIGH);
    }
    else if (lux > 500)
    {
        for (i = 3; i <= 8; i++)
            digitalWrite(i, LOW);
        for (i = 9; i <= 10; i++)
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

    // temperature sensor
    int temperature = dht11.readTemperature();
    Serial.println(temperature);
    if (temperature != DHT11::ERROR_CHECKSUM && temperature != DHT11::ERROR_TIMEOUT)
    {
        if ((temperature - ROOM_TEMP) >= 1)
        {
            Serial.print("Temperature changed to ");
            Serial.println(temperature);
            digitalWrite(13, HIGH);
        }
        else
        {
            digitalWrite(13, LOW);
        }
    }
    else
    {
        Serial.println(DHT11::getErrorString(temperature));
    }

    // gas detector
    float ppm = mq135_sensor.getPPM();
    Serial.print("Air Quality: ");
    Serial.print(ppm);
    Serial.println("ppm");
    if (ppm > 100)
    {
        digitalWrite(13, HIGH);
    }
    else
    {
        digitalWrite(13, LOW);
    }
}
