/* 
 This function can read values up to 121360 lux with adaptive calibration for best possible results.
 It first makes a reading with lowest sensitivity (with higest range,up to 121360 lux)
 Then determines the best sensitivity/range according to the initial reading.
 Then makes a reading using obtained sensitivity
 The resulting reading is made with the best possibe sensitivity according to the current light condition.
*/

#include <Wire.h>
#include <BH1750FVI.h>

BH1750FVI bh1750;

void setup() {
  Wire.begin();
  Serial.begin(115200);
  bh1750.begin();
}

void loop() 
  {
  Serial.println(GetSelfCalibratedReading());
  }

long GetSelfCalibratedReading()
  {    
  int CalcDuration = 0;
  float CalcRatio = 0.45;  
  float NewCalcRatio = 0.45; 
  float FirstReading = 0;
  float SecondReading = 0; 
  
  bh1750.Reset();
  CalcDuration = bh1750.setSensitivity(CalcRatio*69);         // set sensitivity using ratio
  bh1750.startOngoingSamples(BH_ContL);                       // set mode
  delay(CalcDuration+2);                                      // just to avoid 0 values since while loop sometimes does not wait properly
  while(!bh1750.sampleIsFresh()) {}                           // wait for sensor to produce a new reading
  FirstReading = (bh1750.getLightLevel('c')/CalcRatio);       // read the calibration-adjusted value, according to the sensitivity setting
  NewCalcRatio = 54612/(FirstReading * 1.05);                 // calculate new ratio    
  if (NewCalcRatio < 0.45) NewCalcRatio = 0.45;
  if (NewCalcRatio > 3.68) NewCalcRatio = 3.68;

  bh1750.Reset();
  CalcDuration = bh1750.setSensitivity(NewCalcRatio*69); 
  bh1750.startOngoingSamples(BH_ContL);
  delay(CalcDuration+2);                                      // just to avoid 0 values since while loop sometimes does not wait properly
  while(!bh1750.sampleIsFresh()) {}                           // wait for sensor to produce a new reading
  SecondReading = (bh1750.getLightLevel('c')/NewCalcRatio);   // read the calibration-adjusted value, according to the sensitivity setting
  bh1750.powerDown();                                         // power off to save energy, it will start working again in next function call 

  return SecondReading;
  }


