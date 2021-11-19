/*************************************************************************
* 2 CCS811 connected to a NodeMCU ESP32 (I2C protocol)
* Output to Serial
* 
* To avoid both sensors sharing the same I2C address, one of them should
* have ADDRESS pin connected to VDD (3.3V). The pin for the other sensor
* can be left floating or connected to GND.
*
* I2C communication to the sensors works well.
* 
* Fernando S. Pacheco
* Nov-2021
**************************************************************************/

/*
  Based on 
  ccs811basic.ino - Demo sketch printing results of 
  the CCS811 digital gas sensor for monitoring indoor air quality from ams.
  Created by Maarten Pennings 2017 Dec 11
*/

#include <Wire.h>    // I2C library
#include "ccs811.h"  // CCS811 library

// Wiring for ESP32 NodeMCU boards: VDD to 3V3, GND to GND, SDA to 21, SCL to 22
// nWAKE not controlled via host, so connect CCS811.nWAKE to GND
CCS811 ccs811A(-1, CCS811_SLAVEADDR_1); // 1: jumper do pino ADDRESS em VDD
CCS811 ccs811B(-1, CCS811_SLAVEADDR_0); // 0: pino ADDRESS sem ligacao ou GND

uint16_t eco2, etvoc, errstat, raw;

void printSerial() {
  // Print measurement results based on status
  if (errstat==CCS811_ERRSTAT_OK) { 
    Serial.print(eco2);
    Serial.print("\t");
    Serial.print(etvoc);
  } else if( errstat==CCS811_ERRSTAT_OK_NODATA ) {
    Serial.print("CCS811: waiting for (new) data\t");
  } else if( errstat & CCS811_ERRSTAT_I2CFAIL ) { 
    Serial.println("CCS811: I2C error\t");
  } else {
    Serial.print("CCS811: errstat="); Serial.print(errstat,HEX); 
    Serial.print("="); Serial.print(ccs811A.errstat_str(errstat)); 
  }
  Serial.print("\t");
}

void setup() {
  // Enable serial
  Serial.begin(115200);
  Serial.println("");
  delay(8000);
  Serial.println("setup: Starting CCS811 basic demo");
  Serial.print("setup: ccs811 lib  version: "); Serial.println(CCS811_VERSION);

  // Enable I2C
  Wire.begin(); 
  
  // Enable CCS811
  bool ok = ccs811A.begin();
  if (!ok) Serial.println("setup: CCS811 A begin FAILED");

  ok = ccs811B.begin();
  if (!ok) Serial.println("setup: CCS811 B begin FAILED");

  // Print CCS811 versions
  Serial.print("setup A: hardware    version: "); Serial.println(ccs811A.hardware_version(),HEX);
  Serial.print("setup A: bootloader  version: "); Serial.println(ccs811A.bootloader_version(),HEX);
  Serial.print("setup A: application version: "); Serial.println(ccs811A.application_version(),HEX);
  
  Serial.print("setup B: hardware    version: "); Serial.println(ccs811B.hardware_version(),HEX);
  Serial.print("setup B: bootloader  version: "); Serial.println(ccs811B.bootloader_version(),HEX);
  Serial.print("setup B: application version: "); Serial.println(ccs811B.application_version(),HEX);

  // Start measuring
  ok = ccs811A.start(CCS811_MODE_1SEC);
  if (!ok) Serial.println("setup: CCS811 A start FAILED");
  ok = ccs811B.start(CCS811_MODE_1SEC);
  if (!ok) Serial.println("setup: CCS811 B start FAILED");

  Serial.print("CCS_811_CO2ppm_SENSOR_A\tCCS_811_S_A_etVOCppb\t");
  Serial.print("CCS_811_CO2ppm_SENSOR_B\tCCS_811_S_B_etVOCppb\t");
  Serial.println("");
}

void loop() {
  ccs811A.read(&eco2,&etvoc,&errstat,&raw); 
  printSerial(); 

  ccs811B.read(&eco2,&etvoc,&errstat,&raw);
  printSerial(); 
  
  Serial.println("CCS811");
  delay(1000); 
}
