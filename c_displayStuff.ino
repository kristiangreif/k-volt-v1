int option = 1;
int c;
bool state = false;

// Funkcia na konfiguráciu displeja

void connectDisplay(){   
      display.init();   
      display.flipScreenVertically();
      display.setFont(Dialog_plain_12);
  }

// Funkcia na zmenu obrazovky

void changeScreen(){
  if(!externalBatteryConnected()){
    c += 1;
    if(c>9){
      c = 0;
    }
  }
  else{
    if(c == 9 && state){
      changeCapacity(1.0);
    }
    else{
      c += 1;
      }
    if(c>12){
      c = 0;
    }
    }
  }

// Funkcia na zmenu možnosti 

void changeItem(){
  option += 1;
  if(externalBatteryConnected() && c == 4){
    if(option>4){
      option = 1;
      }
    }
   else{
    if(option>2){
      option = 1;
      }
    }
   if(externalBatteryConnected() && c == 9){
    if(!state){
      state = true;
      }
    else{
      state = false;
      }
    }
  }

// Funkcia na potvrdenie možnosti

void selectItem(){
  if(c == 0){
    if(option==1){
      wifiConnect();
      c = 7;
      }
    else{
      wifiDisconnect(); 
      c = 7;
      }
   }
      
   if(c == 6){
    if(option==1){
      digitalWrite(D7, HIGH);;
      c = 7;
      }
    else{
      digitalWrite(D7, LOW);
      c = 7;
      }
    }

  if(externalBatteryConnected()){
    switch(c){
      case 4:
        if(option==1){
           readyResults.externalBatteryFullVoltage = readyResults.LiIonFullVoltage;
           c = 9;
           
           break;
         }
        if(option==2){
          readyResults.externalBatteryFullVoltage = readyResults.LiFePO4_FullVoltage;
          c = 9;
          
          break;
        }
        if(option==3){
          readyResults.externalBatteryFullVoltage = readyResults.LeadAcid_FullVoltage;
          c = 9;
          
          break;
        }
        if(option==4){
          readyResults.externalBatteryFullVoltage = readyResults.LiPoFullVoltage;
          c = 9;
          
          break;
        }
      case 5:
        if(option==1){
          choice = 1;
          digitalWrite(D8, LOW);
          c = 7;
  
          break;
          }
        else{
          choice = 2;
          digitalWrite(D8, HIGH);
          c = 7;
  
          break;
          }

       case 9:
        if(state){
          changeCapacity(5.0);
          }

          break;
      }
    }
}

// Funkcia na monitorovanie čísla obrazovky a jej zobrazenie

void checkScreen(){
    
  if(!externalBatteryConnected()){
    switch(c)
    {
      case 0:
        wifiScreen();
  
        break;
  
      case 1:
        basicsScreen();
  
        break;
        
      case 2:
        percentageScreen();
  
        break;
  
      case 3:
        ahScreen();
  
        break;

      case 4:
        whScreen();
  
        break;
  
      case 5:
        expTimeScreen();
  
        break;
  
      case 6:
        inverterScreen();
  
        break;
  
      case 7:
        statusScreen();
  
        break;

      case 8:
        temperatureScreen();
  
        break;

      case 9:
        powerScreen();
  
        break;
      }
  }
  else{
     switch(c)
    {
      case 0:
        wifiScreen();
  
        break;
  
      case 1:
        basicsScreen();
  
        break;
        
      case 2:
        percentageScreen();
  
        break;
  
      case 3:
        temperatureScreen();
  
        break;
  
      case 4:
        externalBatteryScreen();
  
        break;
  
      case 5:
        carBatteryDisconnectScreen();
  
        break;
  
      case 6:
        inverterScreen();
  
        break;

      case 7:
        statusScreen();
  
        break;

      case 8:
        powerScreen();
  
        break;

      case 9:
        setCapacityScreen();
  
        break;

      case 10:
        ahScreen();
  
        break;

      case 11:
        whScreen();
  
        break;

      case 12:
        expTimeScreen();
  
        break;
      }
      if(readyResults.externalBatteryFullVoltage == 0.0){
        c = 4;
      }

      if(carBatteryVoltageLow() && choice == 0){
        batteryDisconnectRequest();
        c = 5;
       }
    }
    if(temperatureHigh()){
      highTemperatureScreen();
    }
 }

// Funkcia na zmenu kapacity externej batérie

void changeCapacity(float amount){
  readyResults.externalBatteryCapacity += amount;
  if(readyResults.externalBatteryCapacity > 200.0){
    readyResults.externalBatteryCapacity = 0.0;
    }
  }

// Funkcia na nastevenie kapacity externej batérie

void setCapacityScreen(){
  if(!state){
    display.clear();
    display.setTextAlignment(TEXT_ALIGN_CENTER);
    display.setFont(Dialog_bold_12);
    display.drawString(64, 5, "Set capacity?");
    display.setFont(Dialog_bold_12);
    display.drawString(64, 25, "Yes(double)");
    display.display();
    }
  else{
    display.clear();
    display.drawString(64, 25, String(readyResults.externalBatteryCapacity) + "Ah");
    display.display();
    }
  }

// Obrazovka s možnosťou zapnutia/vypnutia meniča

void inverterScreen(){
  display.clear();
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.setFont(Dialog_bold_12);
  display.drawString(64, 5, "Output state:");
  display.setFont(Dialog_plain_12);
  if(option == 1){
    display.setFont(Dialog_bold_12);
    }
  display.drawString(64, 25, "Enable");
  display.setFont(Dialog_plain_12);
  if(option == 2){
    display.setFont(Dialog_bold_12);
    }
  display.drawString(64, 45, "Disable");
  display.display();
  }

// Obrazovka s varovaním o vysokej teplote

void highTemperatureScreen(){
  display.clear();
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.setFont(Dialog_plain_12);
  display.drawString(64, 5, "The temperature");
  display.drawString(64, 20, "inside is too high.");
  display.drawString(64, 35, "Disconnecting load!");
  display.display();
  delay(3000);
  }

// Obrazovka na zobrazenie aktuálneho výkonu z batérie

void powerScreen(){
  display.clear();
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.setFont(Dialog_plain_16);
  display.drawString(64, 10, "Power:");
  display.setFont(Dialog_bold_25);
  display.drawString(64, 30, String(int(readyResults.power)) + "W");
  display.display();
  }

// Obrazovka s varovaním o nízkom napätí autobatérie

void batteryDisconnectRequest(){
  requestCounter++;
  if(requestCounter<2){
    display.clear();
    display.setTextAlignment(TEXT_ALIGN_CENTER);
    display.setFont(Dialog_plain_12);
    display.drawString(64, 5, "The voltage of");
    display.drawString(64, 20, "the external battery");
    display.drawString(64, 35, "is <= 11V.");
    display.display();
    delay(3000);
    display.clear();
    display.drawString(64, 5, "If your battery");
    display.drawString(64, 20, "is a car battery,");
    display.drawString(64, 35, "you may want to");
    display.drawString(64, 50, "disconnect it.");
    display.display();
    delay(3000);
  }
}

// Obrazovka s možnosťami pre autobaterku

void carBatteryDisconnectScreen(){
  if(carBatteryVoltageLow()){
    display.clear();
    display.setTextAlignment(TEXT_ALIGN_CENTER);
    display.setFont(Dialog_plain_16);
    display.drawString(64, 5, "Ext. Bat. Action:");
    display.setFont(Dialog_plain_12);
    if(option == 1){
        display.setFont(Dialog_bold_12);
        }
      display.drawString(64, 25, "Disconnect");
      display.setFont(Dialog_plain_12);
    if(option == 2){
      display.setFont(Dialog_bold_12);
      }
      display.drawString(64, 45, "Leave connected");
      display.display();
    }
      
   else{
    display.clear();
    display.setTextAlignment(TEXT_ALIGN_CENTER);
    display.setFont(Dialog_plain_16);
    display.drawString(64, 10, "Not Available");
    display.display();
    }
 }

// Obrazovka pre výber typu externej batérie pri zapojení

void externalBatteryScreen(){
    display.clear();
    display.setTextAlignment(TEXT_ALIGN_CENTER);
    display.setFont(Dialog_bold_12);
    display.drawString(64, 5, "Select type:");
    display.setFont(Dialog_plain_12);
    if(option == 1){
      display.setFont(Dialog_bold_12);
      }
    display.drawString(64, 25, "Li-ion 3S");
    display.setFont(Dialog_plain_12);
    if(option == 2){
      display.setFont(Dialog_bold_12);
      }
    display.drawString(64, 45, "LiFePO4 4S");
    if(option > 2){
      display.clear();
      display.setFont(Dialog_bold_12);
      display.drawString(64, 5, "Select type:");
      display.setFont(Dialog_plain_12);
      if(option == 3){
        display.setFont(Dialog_bold_12);
        }
      display.drawString(64, 25, "Pb 12v");
      display.setFont(Dialog_plain_12);
      if(option == 4){
        display.setFont(Dialog_bold_12);
        }
      display.drawString(64, 45, "LiPo 3S");
      }
    display.display();
  }
  
// Obrazovka s možnosťou zapnutia/vypnutia WiFi

void wifiScreen(){
  display.clear();
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.setFont(Dialog_bold_12);
  display.drawString(64, 5, "Wifi:");
  display.setFont(Dialog_plain_12);
  if(option == 1){
    display.setFont(Dialog_bold_12);
    }
  display.drawString(64, 25, "ON");
  display.setFont(Dialog_plain_12);
  if(option == 2){
    display.setFont(Dialog_bold_12);
    }
  display.drawString(64, 45, "OFF");
  display.display();
  }

// Obrazovka s kapacitou batérie v Ah

void ahScreen(){
  display.clear();
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.setFont(Dialog_plain_16);
  display.drawString(64, 10, "Capacity:");
  display.setFont(Dialog_bold_25);
  display.drawString(64, 30, String(readyResults.capacity) + "Ah");
  display.display();
  }

// Obrazovka s kapacitou batérie vo Wh

void whScreen(){
  display.clear();
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.setFont(Dialog_plain_16);
  display.drawString(64, 10, "Capacity:");
  display.setFont(Dialog_bold_25);
  display.drawString(64, 30, String(int(readyResults.capacity*readyResults.voltage)) + "Wh");
  display.display();
  }

// Obrazovka so základnými statusmi

void statusScreen(){
  display.clear();
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.setFont(Dialog_plain_12);
  display.drawString(5, 5, "Battery: ");
  display.setFont(Dialog_bold_12);
  if(externalBatteryConnected()){
     display.drawString(60, 5, "Ext");
  }
  else{
    display.drawString(60, 5, "Int");
    }
    
  display.setFont(Dialog_plain_12);
  display.drawString(5, 25, "Output: ");
  display.setFont(Dialog_bold_12);
  
  if(digitalRead(D7)){
     display.drawString(60, 25, "On");
  }
  else{
    display.drawString(60, 25, "Off");
    }
    
  display.setFont(Dialog_plain_12);
  display.drawString(5, 45, "Wifi: ");
  display.setFont(Dialog_bold_12);
  
  if(internet){
    display.drawString(35, 45, "Online");
  }
  else{
    display.drawString(35, 45, "Offline");
    }
    
  display.display();
  }

// Obrazovka so zostávajúcim časom batérie do vybitia

void expTimeScreen(){
  display.clear();
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.setFont(Dialog_plain_16);
  display.drawString(64, 10, "Expected Time:");
  display.setFont(Dialog_bold_16);
  if(readyResults.expectedTime >= 1.0){
    display.drawString(64, 30, String(int(readyResults.expectedTime)) + "h");   
  }
  else{
     display.drawString(64, 30, "0h " + String(int(readyResults.expectedTime*60)) + "min");
    }
  display.display();
  }

// Obrazovka s percentami batérie

void percentageScreen(){
  display.clear();
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.setFont(Dialog_plain_16);
  display.drawString(64, 10, "Battery Level:");
  display.setFont(Dialog_bold_25);
  display.drawString(64, 30, String(int(readyResults.percentage)) + "%");
  display.display();
}

// Obrazovka s teplotou v systéme

void temperatureScreen(){
  display.clear();
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.setFont(Dialog_plain_16);
  display.drawString(64, 10, "Temperature:");
  display.setFont(Dialog_bold_25);
  display.drawString(64, 30, String(int(readyResults.temperature)) + "°C");
  display.display();
  }

// Obrazovka so základnými informéciami

void basicsScreen(){
  display.clear();
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.setFont(Dialog_plain_16);
  display.drawString(64, 5, "Basic Info:");
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.setFont(Dialog_plain_12);
  display.drawString(5, 25, "Voltage: ");
  display.setFont(Dialog_bold_12);
  display.drawString(70, 25, String(readyResults.voltage) + "V");
  display.setFont(Dialog_plain_12);
  display.drawString(5, 45, "Current: ");
  display.setFont(Dialog_bold_12);
  display.drawString(70, 45, String(readyResults.current) + "A");
  display.display();
  }


//void progressScreen(){
//  for (int i; i <= 100; i++){
//     display.clear();
//     display.drawProgressBar(0, 32, 120, 10, i);
//     display.setTextAlignment(TEXT_ALIGN_CENTER);
//     display.drawString(64, 15, String(i) + "%");
//     delay(30);
//     display.display();
//    }
//}

// Logo WiFi

void drawWifi() {
   display.drawXbm(34, 24, WiFi_Logo_width, WiFi_Logo_height, WiFi_Logo_bits);
}
