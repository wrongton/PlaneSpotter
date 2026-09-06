#include <stdio.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

#include "secrets.h"
#include "config.h"

#define LILYGO_LGFX_USE_T_DISPLAY_S3
#include <LilyGo_LovyanGFX.h>

LilyGo_T_Display_S3 display;

unsigned long lastUpdate = 0;

void fetchAircraft() {
  WiFiClientSecure client;
  client.setInsecure();  // Prototype only: TLS certificate is not verified.

  HTTPClient https;

  // Current proof-of-concept ADSB.lol radius endpoint.
  String url = String("https://api.adsb.lol/v2/lat/") +
               String(OBSERVER_LAT, 4) +
               "/lon/" + String(OBSERVER_LON, 4) +
               "/dist/" + String(API_RADIUS);

  Serial.println();
  Serial.println("Requesting ADS-B data:");
  Serial.println(url);

  display.setTextColor(TFT_WHITE, TFT_BLACK);
  display.drawString("Requesting ADS-B data:", 7, 70, 2);
  display.drawString(url, 7, 90, 1);

  if (!https.begin(client, url)) {
    Serial.println("HTTPS begin failed");
    display.setTextColor(TFT_RED, TFT_BLACK);
    display.drawString("HTTPS begin failed", 7, 110, 2);
    return;
  }

  https.addHeader("User-Agent", API_USER_AGENT);

  int httpCode = https.GET();

  Serial.printf("HTTP status: %d\n", httpCode);

  char output[32];
  snprintf(output, sizeof(output), "HTTP status: %d", httpCode);
  display.drawString(output, 7, 110, 2);

  if (httpCode > 0) {
    String payload = https.getString();

    Serial.println("--- response ---");
    Serial.println(payload);
    Serial.println("--- end ---");

    // Temporary proof-of-concept output. Full JSON is better viewed over Serial.
    display.drawString(payload, 7, 130, 1);
  } else {
    Serial.printf(
      "HTTP GET failed: %s\n",
      https.errorToString(httpCode).c_str());
  }

  https.end();
}

void setup() {
  Serial.begin(115200);

  display.begin(1, 220);
  display.fillScreen(TFT_BLACK);
  display.drawRect(0, 0, display.width(), display.height(), TFT_CYAN);
  display.setTextColor(TFT_WHITE, TFT_BLACK);
  display.drawString("PLANE SPOTTER", 7, 10, 2);
  display.drawString("booting...", 7, 30, 1);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  int timeoutCounter = 0;
  while (WiFi.status() != WL_CONNECTED && timeoutCounter < 20) {
    delay(500);
    Serial.print(".");
    timeoutCounter++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    display.setTextColor(TFT_YELLOW, TFT_BLACK);
    display.drawString("wi-fi: connected", 7, 50, 2);

    Serial.println();
    Serial.println("Wi-Fi connected");
    Serial.print("IP: ");
    Serial.println(WiFi.localIP());
    Serial.print("RSSI: ");
    Serial.println(WiFi.RSSI());
  } else {
    display.setTextColor(TFT_RED, TFT_BLACK);
    display.drawString("wi-fi: failed", 7, 50, 2);
    Serial.println();
    Serial.println("Wi-Fi connection failed");
  }
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    return;
  }

  unsigned long now = millis();

  if (lastUpdate == 0 || now - lastUpdate >= REFRESH_INTERVAL_MS) {
    lastUpdate = now;
    fetchAircraft();
  }
}
