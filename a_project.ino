
// Zahrnutie všetkých potrebných knižníc

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>
#include <EEPROM.h>
#include <Wire.h>
#include <math.h>
#include <Adafruit_ADS1015.h>
#include "ResponsiveAnalogRead.h"
#include "OneButton.h"
#include "SSD1306Wire.h"
#include "results.h"
#include "images.h"
#include "fonts.h"

// Vytvorenie objektov a globálnych premenných

SSD1306Wire  display(0x3c, D3, D5);
Adafruit_ADS1115 ads(0x48);
OneButton button(D6, true);
BlynkTimer timer;

const int inputs = 3;
bool internet = false;

ResponsiveAnalogRead analog[inputs];
ResponsiveAnalogRead resultsFilter[2];

ReadyResults readyResults;

// Konfigurácia zariadení
// Spustí sa na začiatku programu 

void setup() {
  connectDisplay();
  setupIOs();
  button.attachClick(changeScreen);
  button.attachDoubleClick(changeItem);
  button.attachPress(selectItem);
  timer.setInterval(1000L, sendResults);
}

// Hlavný cyklus

void loop() {
  checkScreen();
  calculateData(smoothResults(transformRawData(getData())));
  protection();
  button.tick();
  if (Blynk.connected()) {
    timer.run();
    Blynk.run();
  }
}
