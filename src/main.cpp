#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ArduinoHttpClient.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <StreamUtils.h>

extern "C"
{
#include "user_interface.h"
}

WiFiClient wifi_client;
StaticJsonDocument<300> doc;
PubSubClient mqttClient;

// The ESP8266 RTC memory is arranged into blocks of 4 bytes. The access methods read and write 4 bytes at a time,
// so the RTC data structure should be padded to a 4-byte multiple.
struct
{
  uint32_t crc32;   // 4 bytes
  uint8_t channel;  // 1 byte,   5 in total
  uint8_t bssid[6]; // 6 bytes, 11 in total
  uint8_t gsmcheck;  // 1 byte,  12 in total
} rtcData;

// Ver src/config_template.h
#include "config_home.h"
#include "functions.h"


void setup() {

  // init general (serial, wifi, gprs, etc)
  serial_init();
  Serial.printf("\n\n::: Serial init %ld\n", millis());
  wifi_init();
  Serial.printf("\n\n::: Wifi init %ld\n", millis());
  mqtt_init();
  Serial.printf("\n\n::: Mqtt init %ld\n", millis());

  // temperatura
  // energia

  // power del supervisor
  double Vvalue = 0;
  Vvalue = bat_voltage(A0);
  doc["power"]["vbat"] = Vvalue;
  doc["power"]["pbat"] = bat_percentage(Vvalue);
  Serial.printf("\n\n::: Bat sensor %ld\n", millis());

  // wifi?
  boolean wifi_conn = wifi_check();
  doc["connectivity"]["wifi"] = wifi_conn;
  Serial.printf("\n\n::: Wifi check %ld\n", millis());

  // conexion gprs
  boolean gprs_conn = false;
  doc["connectivity"]["gsmcheck"] = rtcData.gsmcheck;

  if (rtcData.gsmcheck == 1)
  {
    // Aca hay que chequear el gsm/gprs
    doc["connectivity"]["gprs"] = gprs_conn;
    doc["connectivity"]["gprs tested"] = true;
  }
  else
  {
    doc["connectivity"]["gprs"] = gprs_conn;
    doc["connectivity"]["gprs tested"] = false;
  }


  // internet?
  boolean internet_conn = false;
  doc["connectivity"]["internet"] = internet_conn;

  if(wifi_conn) {
    internet_conn = internet_check(wifi_client, INTERNET1, PORT1);
    if(!internet_conn) {
      internet_conn = internet_check(wifi_client, INTERNET2, PORT2);
    }
    doc["connectivity"]["internet"] = internet_conn;
  }
  else if (gprs_conn)
  {
    /* Mandar por aca la verificacion de internet*/
    Serial.println("No hay wifi, pruebo internet por GPRS");
  }
  Serial.printf("\n\n::: Internet check %ld\n", millis());

  // MQTT?
  boolean mqtt_conn = false;
  if (internet_conn)
  {
    if(wifi_conn){
      mqtt_conn = mqtt_check(wifi_client);
    } 
    else if (gprs_conn)
    {
      // mqtt_conn = mqtt_check(gprs_client);
    }

    doc["connectivity"]["mqtt"] = mqtt_conn;
  }
  Serial.printf("\n\n::: Mqtt check %ld\n", millis());

  // send json
  boolean mqtt_sent = false;
  boolean sms_sent = false;

  doc["connectivity"]["mqtt sent"] = mqtt_sent;
  doc["connectivity"]["sms sent"] = sms_sent;

  if(mqtt_conn) {
    // envio json por mqtt x internet
    mqttClient.beginPublish(MQTT_USER "/" HOSTNAME, measureJson(doc), false);
    serializeJson(doc, mqttClient);
    mqtt_sent = mqttClient.endPublish();

    // mqttClient.beginPublish(MQTT_USER "/" HOSTNAME, measureJson(doc), false);
    // BufferingPrint bufferedClient(mqttClient, 32);
    // serializeJson(doc, bufferedClient);
    // bufferedClient.flush();
    // mqtt_sent = mqttClient.endPublish();
    // https: // arduinojson.org/v6/how-to/use-arduinojson-with-pubsubclient/
    
    doc["connectivity"]["mqtt sent"] = mqtt_sent;
    mqttClient.disconnect();
  }
  Serial.printf("\n\n::: Mqtt send %ld\n", millis());

  if(!mqtt_sent){
    // si no se puede enviar mqtt, aviso por sms...
    // hay que setear sms_sent a true si salio
    Serial.println("no salio");
  }


  serializeJsonPretty(doc, Serial);

  // deep sleep
  Serial.printf("\n\n::: Deep Sleep %ld\n", millis());
  ESP.deepSleep(SLEEPTIME, WAKE_NO_RFCAL);
}

void loop() { 
  // Aca nada, porque se va a deep sleep
}