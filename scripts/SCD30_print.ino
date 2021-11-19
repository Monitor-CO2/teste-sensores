/*************************************************************************
* Sensirium SCD30 connected to a NodeMCU ESP32 (I2C protocol)
* Output to Serial
* 
* Connected to external power supply (5V). I do not recommend 
* connecting directly the sensor VCC pin to USB (Vin at NodeMCU) 
* because it demands too much power and can make serial communication
* fail. In my tests, it did not work with 3.3 V supply from NodeMCU pin.
* IMPORTANT: Remember to connect GND of the power source to NodeMCU GND.
*
* I2C communication to the sensor works almost perfectly. Sometimes,
* it returns 0 or -2147483648 as results. Simply discard these values when 
* processing data.
* 
* Fernando S. Pacheco
* Nov-2021
**************************************************************************/

#include <Wire.h> // for I2C communication
#include "SCD30.h"

void setup() {
    Wire.begin(); //enable I2C
    Serial.begin(115200);

    // SCD30
    Serial.println("Initializing SCD30");
    scd30.initialize();
    Serial.println("DONE. Initialized SCD30");
        
    Serial.println("SCD30_CO2ppm TempCelsius Umid%");
}

void loop() {
    // SCD30
    float measureSCD30[3] = {0};
    if (scd30.isAvailable()) {
        scd30.getCarbonDioxideConcentration(measureSCD30);
    }
    
    Serial.print((int)measureSCD30[0]);
    Serial.print("\t");
    Serial.print((int)measureSCD30[1]);
    Serial.print("\t");
    Serial.print((int)measureSCD30[2]);
    Serial.print("\t");
   
    Serial.println("SCD30");
    delay(2000);
}
