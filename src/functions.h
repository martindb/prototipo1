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

boolean internet_check(Client &client, String host, int port) {
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

double bat_voltage(int pin) {
  double v = 0;
  for (unsigned int i = 0; i < 10; i++)
  {
    v = v + analogRead(pin);
    delay(5);
  }
  v = v / 10 / 1024 * 5.2;
  v = (int)(v * 100 + 0.5) / 100.0;
  return v;
}

int bat_percentage(double voltage) {

  float fVoltageMatrix[22][2] = {
      {4.2, 100},
      {4.15, 95},
      {4.11, 90},
      {4.08, 85},
      {4.02, 80},
      {3.98, 75},
      {3.95, 70},
      {3.91, 65},
      {3.87, 60},
      {3.85, 55},
      {3.84, 50},
      {3.82, 45},
      {3.80, 40},
      {3.79, 35},
      {3.77, 30},
      {3.75, 25},
      {3.73, 20},
      {3.71, 15},
      {3.69, 10},
      {3.61, 5},
      {3.27, 0},
      {0, 0}};

  int i, perc;

  perc = 100;

  for (i = 20; i >= 0; i--)
  {
    if (fVoltageMatrix[i][0] >= voltage)
    {
      perc = fVoltageMatrix[i + 1][1];
      break;
    }
  }
  return perc;
}

#endif