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

boolean internet_check(WiFiClient wifi_client, String host, int port) {
  HttpClient client = HttpClient(wifi_client, host, port);
  client.beginRequest();
  client.get("/");
  client.endRequest();
  int statusCode = client.responseStatusCode();
  if (statusCode >= 200 && statusCode < 400)
  {
    Serial.printf("\nInternet OK (%s:%d - %d)", host.c_str(), port, statusCode);
    return true;
  } else {
    Serial.printf("\nInternet NOK (%s:%d - %d)", host.c_str(), port, statusCode);
    return false;
  }
}


#endif