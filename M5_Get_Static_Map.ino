#include <M5Stack.h>
#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>

String COORDS = "22.283795,114.158056";
uint8_t zoom = 13;

void buttons_test() {
  bool needRedraw = false;
  while (M5.BtnA.isPressed()) {
    if (zoom > 0) zoom -= 1;
    Serial.println("-1 zoom: " + String(zoom));
    delay(200);
    M5.update();
    needRedraw = true;
  }
  while (M5.BtnC.isPressed()) {
    if (zoom < 20) zoom += 1;
    Serial.println("+1 zoom: " + String(zoom));
    delay(200);
    M5.update();
    needRedraw = true;
  }

  if (M5.BtnB.isPressed()) {
  }
  if (needRedraw) drawMap();
}

boolean checkConnection() {
  int count = 0;
  Serial.print("Waiting for Wi-Fi connection");
  while (count < 30) {
    if (WiFi.status() == WL_CONNECTED) {
      Serial.println("\nConnected!");
      return (true);
    }
    delay(500);
    Serial.print(".");
    count++;
  }
  Serial.println("Timed out.");
  return false;
}

void drawMap() {
  HTTPClient http;
  Serial.print("[HTTP] begin...\n");
  // configure traged server and url
  http.begin("http://maps.googleapis.com/maps/api/staticmap?center=" + COORDS + "&zoom=" + String(zoom) + "&size=320x260&format=jpg-baseline&markers=color:blue%7Csize:mid%7Clabel:H%7C" + COORDS + "");
  Serial.print("[HTTP] GET...");
  // start connection and send HTTP header
  int httpCode = http.GET();
  // httpCode will be negative on error
  if (httpCode > 0) {
    // HTTP header has been send and Server response header has been handled
    Serial.printf(" code: %d\n", httpCode);
    // file found at server
    if (httpCode == 200) {
      int ln = http.getSize();
      Serial.println("\nPayload: " + String(ln));
      File file = SD.open("/MAP.JPG", FILE_WRITE);
      if (!file) {
        Serial.println("Failed to open file for writing");
      } else {
        Serial.println("writeToStream (file)");
        http.writeToStream(&file);
        file.close();
        Serial.println("drawJpgFile (file)");
        M5.Lcd.drawJpgFile(SD, "/MAP.JPG", 0, 0, 320, 240);
      }
    }
  } else {
    Serial.printf("[HTTP] GET... failed, error %i: %s\n", httpCode, http.errorToString(httpCode).c_str());
  }
  http.end();
}

void setup() {
  Serial.begin(115200);
  M5.begin();
  // Lcd display
  M5.Lcd.setTextColor(TFT_BLACK, TFT_WHITE);
  M5.Lcd.fillScreen(TFT_WHITE);
  M5.Lcd.setFreeFont(&FreeMono9pt7b);
  //M5.Lcd.setTextSize(1);
  //M5.Lcd.setBrightness(100);
  Serial.println();
  Serial.println();
  Serial.println();
  for (uint8_t t = 4; t > 0; t--) {
    Serial.printf("[SETUP] WAIT %d...\n", t);
    Serial.flush();
    delay(1000);
  }
  String wifi_ssid = "SecondTry";
  String wifi_password = "didier0barbas";
  Serial.print("WIFI-SSID: ");
  Serial.println(wifi_ssid);
  Serial.print("WIFI-PASSWD: ");
  Serial.println(wifi_password);

  WiFi.begin(wifi_ssid.c_str(), wifi_password.c_str());
  bool x = false;
  while (!x) {
    Serial.write(' ');
    x = checkConnection();
  }
  Serial.println(WiFi.softAPIP());
  drawMap();
}

void loop() {
  buttons_test();
  M5.update(); // 好importantですね！
  // If not the buttons status is not updated lo.
}
