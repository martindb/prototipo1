#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ArduinoHttpClient.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

// Ver src/config_template.h
#include "config_home.h"
#include "functions.h"


WiFiClient wifi_client;
StaticJsonDocument<200> doc;

void setup() {

  // init general (serial, wifi, gprs, etc)
  serial_init();
  wifi_init();

  // conexion gprs
  boolean gprs_conn = false;
  doc["connectivity"]["gprs"] = gprs_conn;

  // temperatura
  // energia
  // power del supervisor
  
  // wifi?
  boolean wifi_conn = wifi_check();
  doc["connectivity"]["wifi"] = wifi_conn;

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

  // MQTT?
  boolean mqtt_conn = false;
  if (internet_conn)
  {
    doc["connectivity"]["mqtt"] = mqtt_conn;
  }

  // send json
  boolean mqtt_sent = false;
  boolean sms_sent = false;

  doc["connectivity"]["mqtt sent"] = mqtt_sent;
  doc["connectivity"]["sms sent"] = sms_sent;

  if(mqtt_conn) {
    // envio json por mqtt x internet
    // hay que setear mqtt_sent a true si salio
  }

  if(!mqtt_sent){
    // si no se puede enviar mqtt, aviso por sms...
    // hay que setear sms_sent a true si salio
  }


  serializeJsonPretty(doc, Serial);

  // deep sleep
  ESP.deepSleep(SLEEPTIME);
}

void loop() { 
  // Aca nada, porque se va a deep sleep
}