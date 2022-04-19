#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ArduinoHttpClient.h>
#include <PubSubClient.h>

// Ver src/config_template.h
#include "config_home.h"
#include "functions.h"


WiFiClient wifi_client;

void setup() {

  // init general (serial, wifi, gprs, etc)
  serial_init();
  wifi_init();

  // conexion gprs
  boolean gprs_conn = false;

  // temperatura
  // energia
  // power del supervisor
  // json => topico de mqtt

  // wifi?
  boolean wifi_conn = wifi_check();
  
  // internet?
  boolean internet_conn = false;
  if(wifi_conn) {
    internet_conn = internet_check(wifi_client, INTERNET1, PORT1);
    if(!internet_conn) {
      internet_conn = internet_check(wifi_client, INTERNET2, PORT2);
    }
  } else if (gprs_conn)
  {
    /* Mandar por aca la verificacion de internet*/
    Serial.println("No hay wifi, pruebo internet por GPRS");
  } else
  {
    // si no se puede publicar en mqtt, envio sms
    /* Mandar por SMS algo de info (o todo?) */
    Serial.println("No hay wifi ni gprs... mando algo por SMS");
  }
    
  // envio json por mqtt x internet

  // si no se puede enviar mqtt, aviso por sms...

  // deep sleep
  ESP.deepSleep(SLEEPTIME);
}

void loop() { 
  // Aca nada, porque se va a deep sleep
}