
// Funkcia na pripojenie k WiFi

void wifiConnect(){
  if(!internet){
      char auth[] = "712ebe45e1a24d40839877d1dc19f2b9";
      EEPROM.begin(512);
      WiFiManager wifiManager;
      display.clear();
      display.setTextAlignment(TEXT_ALIGN_CENTER);
      display.drawString(64, 0, "Connecting");
      drawWifi();
      display.display();
      //wifiManager.resetSettings();
      wifiManager.autoConnect("K-Volt");
      Blynk.begin(auth, WiFi.SSID().c_str(), WiFi.psk().c_str());
      internet = true;
    }
  }

// Funkcia na odpojenie od WiFi

void wifiDisconnect(){
  if(internet){
     WiFi.persistent(false);      
     WiFi.disconnect();          
     WiFi.persistent(true);
     display.clear();
     display.setTextAlignment(TEXT_ALIGN_CENTER);
     display.drawString(64, 25, "Disconnected!");
     display.display();
     delay(2000);
     ESP.restart();
    }
  }

// Funkcia na posielanie hodnôt do aplikácie Blynk

void sendResults(){
  Blynk.virtualWrite(V0, readyResults.voltage);
  Blynk.virtualWrite(V1, readyResults.current);
  Blynk.virtualWrite(V2, readyResults.percentage);
  Blynk.virtualWrite(V3, readyResults.capacity);
  Blynk.virtualWrite(V4, readyResults.expectedTime);
  Blynk.virtualWrite(V5, readyResults.temperature);
  }
