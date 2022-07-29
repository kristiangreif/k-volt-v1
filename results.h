
// Zoskupenie premenných s hlavnými výsledkami a konštantami

struct readyResults{
  float voltage;
  float current;
  float expectedTime;
  float capacity;
  float percentage;
  float temperature;
  float power;
  
  float externalBatteryFullVoltage;
  float externalBatteryCapacity;

  const float internalCapacity = 11.0;
  const float LiFePO4_FullVoltage = 13.8;
  const float LeadAcid_FullVoltage = 14.0;
  const float LiIonFullVoltage = 12.6;
  const float LiPoFullVoltage = 13.0;
  const float emptyBatteryVoltage = 10.2;
  };

typedef struct readyResults ReadyResults;
