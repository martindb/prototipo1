#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ArduinoHttpClient.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

WiFiClient wifi_client;
StaticJsonDocument<300> doc;
PubSubClient mqttClient;

// Ver src/config_template.h
#include "config_home.h"
#include "functions.h"


void setup() {

  // init general (serial, wifi, gprs, etc)
  serial_init();
  wifi_init();
  mqtt_init();

  // conexion gprs
  boolean gprs_conn = false;
  doc["connectivity"]["gprs"] = gprs_conn;

  // temperatura
  // energia

  // power del supervisor
  double Vvalue = 0;
  Vvalue = bat_voltage(A0);
  doc["power"]["vbat"] = Vvalue;
  doc["power"]["pbat"] = bat_percentage(Vvalue);

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
    if(wifi_conn){
      mqtt_conn = mqtt_check(wifi_client);
    } 
    else if (gprs_conn)
    {
      // mqtt_conn = mqtt_check(gprs_client);
    }

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
    //mqtt_sent = mqttClient.publish("pepe", "Hola mundo");
    mqttClient.beginPublish("pepe", measureJson(doc), false);
    serializeJson(doc, mqttClient);
    mqtt_sent = mqttClient.endPublish();
    // https: // arduinojson.org/v6/how-to/use-arduinojson-with-pubsubclient/
    
    doc["connectivity"]["mqtt sent"] = mqtt_sent;
    mqttClient.disconnect();
  }

  if(!mqtt_sent){
    // si no se puede enviar mqtt, aviso por sms...
    // hay que setear sms_sent a true si salio
    Serial.println("no salio");
  }


  serializeJsonPretty(doc, Serial);

  // deep sleep
  ESP.deepSleep(SLEEPTIME);
}

void loop() { 
  // Aca nada, porque se va a deep sleep
}