#include "Arduino.h"
#include "settings_store.h"
#include <EEPROM.h>

#define ARRAY_LENGTH 12
#define STARTING_EEPROM_ADDRESS 10
#define EEPROM_VERSION 221


void writeIntArrayIntoEEPROM(int address, int numbers[], int arraySize)
{
  int addressIndex = address;
  for (int i = 0; i < arraySize; i++) 
  {
    EEPROM.write(addressIndex, numbers[i] >> 8);
    EEPROM.write(addressIndex + 1, numbers[i] & 0xFF);
    addressIndex += 2;
  }
}
void readIntArrayFromEEPROM(int address, int numbers[], int arraySize)
{
  int addressIndex = address;
  for (int i = 0; i < arraySize; i++)
  {
    numbers[i] = (EEPROM.read(addressIndex) << 8) + EEPROM.read(addressIndex + 1);
    addressIndex += 2;
  }
}



SettingsStore::SettingsStore(double *SetPoint, double *Kp, double *Ki, double *Kd, double *Kp2, double *Ki2, double *Kd2, bool *PidDirection, int *SampleTime, int *pid2Band, int *SettingAdjustmentMultiple)
{
  _SetPoint = SetPoint;
  _Kp = Kp;
  _Ki = Ki;
  _Kd = Kd;
  _Kp2 = Kp2;
  _Ki2 = Ki2;
  _Kd2 = Kd2;
  _PidDirection = PidDirection;
  _SampleTime = SampleTime;
  _pid2Band = pid2Band;
  _SettingAdjustmentMultiple = SettingAdjustmentMultiple;
  EEPROM.begin(100);

}

bool SettingsStore::save(){
  // write to variables
  // Serial.println("Saving settings to EEPROM");
  int numbers[ARRAY_LENGTH] = {
              EEPROM_VERSION,
              int(*_SetPoint),
              int(*_Kp),
              int(*_Ki*100),
              int(*_Kd),
              int(*_Kp2),
              int(*_Ki2*100),
              int(*_Kd2),
              int(*_PidDirection),
              *_SampleTime,
              *_pid2Band,
              *_SettingAdjustmentMultiple
   };
  writeIntArrayIntoEEPROM(STARTING_EEPROM_ADDRESS, numbers, ARRAY_LENGTH);
  // EEPROM.write(SAVED_STATUS_ADDRESS, 1);
  EEPROM.commit();
}

bool SettingsStore::load(){
  
  int eepromValues[ARRAY_LENGTH];
  readIntArrayFromEEPROM(STARTING_EEPROM_ADDRESS, eepromValues, ARRAY_LENGTH);

  if(eepromValues[0] != EEPROM_VERSION){
    // Serial.println("EEPROM version does not match, cannot load");
    return false;
  }
  // Serial.println("Loading settings from EEPROM");
  // retrieve data and save to variables
  // Serial.println("------------------");
  // Serial.println(eepromValues[0]);
  // Serial.println(eepromValues[1]);
  // Serial.println(eepromValues[2]);
  // Serial.println(eepromValues[3]);
  // Serial.println(eepromValues[4]);
  // Serial.println(eepromValues[5]);
  // Serial.println(eepromValues[6]);
  // Serial.println(eepromValues[7]);
  // Serial.println(eepromValues[8]);
  // Serial.println(eepromValues[9]);
  // Serial.println(eepromValues[10]);
  // Serial.println(eepromValues[11]);
  // Serial.println("------------------");

  *_SetPoint =                  eepromValues[1];
  *_Kp =                        eepromValues[2];
  *_Ki =                        eepromValues[3]/100.0;
  *_Kd =                        eepromValues[4];
  *_Kp2 =                       eepromValues[5];
  *_Ki2 =                       eepromValues[6]/100.0;
  *_Kd2 =                       eepromValues[7];
  *_PidDirection =              bool(eepromValues[8]);
  *_SampleTime =                eepromValues[9];
  *_pid2Band =                  eepromValues[10];
  *_SettingAdjustmentMultiple = eepromValues[11];
  return true;


}

void SettingsStore::reset(){
  int numbers[ARRAY_LENGTH];
  writeIntArrayIntoEEPROM(STARTING_EEPROM_ADDRESS, numbers, ARRAY_LENGTH);
  Serial.println("EEPROM settings cleared");
}

