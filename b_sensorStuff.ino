int requestCounter, choice;

// Funkcia na konfiguráciu vstupov a výstupov

void setupIOs(){
   pinMode(D7, OUTPUT);
   pinMode(D0, INPUT);
   pinMode(D4, INPUT);
   pinMode(D8, OUTPUT);
   ads.setGain(GAIN_ONE); 
   ads.begin();
   for(int j = 0; j < inputs; j++){
    analog[j] = ResponsiveAnalogRead(true);
    analog[j].setAnalogResolution(32768);
    delay(1);
    }
   for(int j = 0; j < 2; j++){
    resultsFilter[j] = ResponsiveAnalogRead(true);
    resultsFilter[j].setAnalogResolution(2000);
    delay(1);
    }
  }

// Funkcia na získavanie hodnôt zo senzorov

int *getData(){ 
  static const int inputPin[inputs] = {0, 1};
  static int smoothedReading[inputs];
  
  for(int i = 0; i < (inputs-1); i++) {
      analog[i].update(ads.readADC_SingleEnded(inputPin[i]));
      smoothedReading[i] = analog[i].getValue();
      delay(1);
    }
    ads.setGain(GAIN_SIXTEEN);
    analog[2].update(ads.readADC_Differential_2_3());
    smoothedReading[2] = analog[2].getValue();
    delay(10);
    ads.setGain(GAIN_ONE);
    return smoothedReading;
  }

// Funkcia na transformáciu analógových hodnôt na zodpovedjúce jednotky

float *transformRawData(int *smoothedReading){
  static float rawResults[inputs];
  static const float mvPerBit_gain1 = 0.000125;
  static const float uvPerBit_gain16 = 7.8125;
  
  rawResults[0] = (smoothedReading[1]*mvPerBit_gain1)*5;
  rawResults[1] = ((smoothedReading[2]+((smoothedReading[2]/0.73)*0.27))*(uvPerBit_gain16/10))/375;
  rawResults[2] = Thermistor(smoothedReading[0]);
  
  return rawResults;
  }

// Funkcia na vyhladenie základných výsledkov

float *smoothResults(float *rawResults){
  static float smoothedResults_1[inputs];
  static float smoothedResults_2[inputs];
  
  for(int i = 0; i<inputs; i++) {
    smoothedResults_1[i] = smoothedResults_1[i] * .90 + rawResults[i] * .10;
    smoothedResults_2[i] = smoothedResults_2[i] * .95 + smoothedResults_1[i] * .05;
    delay(1);
  }
  
  return smoothedResults_2;
  }

// Funkcia na vypočítavanie hodnôt zo základných výsledkov

void calculateData(float *smoothedResults){
   readyResults.voltage = smoothedResults[0]; 
   readyResults.current = smoothedResults[1];
   readyResults.temperature = smoothedResults[2];
   readyResults.power = readyResults.voltage*readyResults.current;

   
   if(!externalBatteryConnected()){
     readyResults.externalBatteryFullVoltage = 0.0;
     readyResults.externalBatteryCapacity = 0.0;
     choice = 0;
     digitalWrite(D8, LOW);
     readyResults.percentage = ((readyResults.voltage-readyResults.emptyBatteryVoltage)/(readyResults.LiFePO4_FullVoltage-readyResults.emptyBatteryVoltage))*100;
     readyResults.capacity = (readyResults.percentage/100)*readyResults.internalCapacity;
   }
   
   else{
    if(choice != 1){
      digitalWrite(D8, HIGH);
    }
    readyResults.percentage = ((readyResults.voltage-readyResults.emptyBatteryVoltage)/(readyResults.externalBatteryFullVoltage-readyResults.emptyBatteryVoltage))*100;
    resultsFilter[0].update((readyResults.percentage/100)*readyResults.externalBatteryCapacity);
    readyResults.capacity = resultsFilter[0].getValue();
   }
    
   if(readyResults.current != 0.0){
       resultsFilter[1].update(roundf((readyResults.capacity/readyResults.current)*100)/100);
       readyResults.expectedTime = resultsFilter[1].getValue();
   }
}

// Funkcia na ochranu systému 

void protection(){
  if(readyResults.voltage <= readyResults.emptyBatteryVoltage || readyResults.current >= 10.0 || temperatureHigh()){
     digitalWrite(D7, LOW);
    }
  }

// Funkcia na monitorovanie konektora pre externú batériu

bool externalBatteryConnected(){
  if(!digitalRead(D0)){
    return true;
    }

   else{
    return false;
    }
  }


// Funkcia na ohlásenie nízkeho napätia autobatérie

bool carBatteryVoltageLow(){
  if(externalBatteryConnected() && readyResults.externalBatteryFullVoltage == readyResults.LeadAcid_FullVoltage && readyResults.voltage <= 11.0){
    return true;
    }
  else{
    requestCounter = 0;
    return false;
    }
  }

// Funkcia na ohlásenie vysokej teploty v systéme 

bool temperatureHigh(){
  if(readyResults.temperature >= 35.0){
    return true;
    }

  else{
    return false;
    }
  }

// Funkcia na monitorovanie teploty v systéme

float Thermistor(int RawADC){
  float Temp;

  if((RawADC-1) != 0){
    Temp = log(10000.0 * ((26400.0 / RawADC - 1)));
    Temp = 1 / (0.001129148 + (0.000234125 + (0.0000000876741 * Temp * Temp )) * Temp );
    Temp = roundf((Temp - 273.15)*10)/10;            // convert from Kelvin to Celsius
  }
  
  return Temp;
}
