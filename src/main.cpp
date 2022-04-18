#include <Arduino.h>
#include "ESP8266WiFi.h"

#define ssid "red3"
#define password "otraclave3"

void setup()
{
  Serial.begin(74880);
  Serial.setTimeout(2000);

  // Wait for serial to initialize.
  while (!Serial)
  {
  }

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print("*");
  }

  Serial.println("");
  Serial.println("WiFi connection Successful");
  Serial.print("The IP Address of ESP8266 Module is: ");
  Serial.print(WiFi.localIP()); // Print the IP address

  ESP.deepSleep(10e6);

}

void loop()
{
}