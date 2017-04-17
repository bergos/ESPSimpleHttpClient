#include <ESP8266WiFi.h>
#include <ESPSimpleHttpClient.h>
#include <WiFiManager.h>

void setup() {
  Serial.begin(115200);

  WiFiManager wifiManager;

  wifiManager.autoConnect("simple-http-client");
}

void loop() {
  String url = "http://ip.jsontest.com/";

  ESPSimpleHttpClient http;

  http.begin(url);

  http.addHeader("Accept", "application/json");

  if (!http.get()) {
    Serial.println(http.error);
  }

  // PUT request
/*
  String content = "{\"key\":\"value\"}";

  http.addHeader("Content-Type", "application/json");

  if (!http.put(content)) {
    Serial.println(http.error);
  }
*/

  char body[700];

  http.body(body, sizeof(body));

  Serial.println(body);

  http.end();

  delay(10000);
}
