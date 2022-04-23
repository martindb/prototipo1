#ifndef FUNCTIONS
#define FUNCTIONS

void serial_init() {
  Serial.begin(74880);
  Serial.setTimeout(2000);
  // Wait for serial to initialize.
  while (!Serial)
  {
  }
}

void wifi_init() {
  Serial.printf("\nConnecting to: %s ", SSID);
  WiFi.mode(WIFI_STA);
  WiFi.begin(SSID, PASSWORD);
}

void mqtt_init() {
  mqttClient.setServer(MQTT_BROKER_ADRESS, MQTT_PORT);
}

boolean wifi_check() {
  unsigned int timeout = 15;
  unsigned int counter = 0;
  while (WiFi.status() != WL_CONNECTED)
  {
    counter++;
    delay(1000);
    Serial.print(".");
    if(counter > timeout) {
      Serial.printf("\nCan't connect to wifi %s\n", SSID);
      return false;
    }
  }
  Serial.printf("\nWifi connected. IP address: %s\n", WiFi.localIP().toString().c_str());
  return true;
}

boolean mqtt_check(WiFiClient &client) {
  unsigned int timeout = 15;
  unsigned int counter = 0;
  mqttClient.setClient(client);
  while (!mqttClient.connected())
  {
    mqttClient.connect(HOSTNAME, MQTT_USER, MQTT_PASS);
    counter++;
    Serial.print(".");
    delay(1000);
    if (counter > timeout)
    {
      Serial.printf("\nCan't connect to mqtt %s\n", MQTT_BROKER_ADRESS);
      return false;
    }
  }
  Serial.printf("\nConnected to mqtt %s\n", MQTT_BROKER_ADRESS);
  return true;
}

boolean internet_check(WiFiClient &client, String host, int port) {
  HttpClient http_client = HttpClient(client, host, port);
  http_client.beginRequest();
  http_client.get("/");
  http_client.endRequest();
  int statusCode = http_client.responseStatusCode();
  if (statusCode >= 200 && statusCode < 400)
  {
    Serial.printf("\nInternet OK (%s:%d - %d)\n", host.c_str(), port, statusCode);
    return true;
  } else {
    Serial.printf("\nInternet NOK (%s:%d - %d)\n", host.c_str(), port, statusCode);
    return false;
  }
}


#endif