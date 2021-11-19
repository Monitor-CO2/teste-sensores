/*************************************************************************
* DS18B20 connected to an Arduino Nano (OneWire protocol)
* Output to Serial in Celsius
* 
* It also works perfectly without changes on Arduino Uno and NodeMCU ESP32
* 
* Remember to add a resistor from data to Vcc.
* Check schematics (normal mode) at 
* https://randomnerdtutorials.com/esp32-ds18b20-temperature-arduino-ide/
* 
* Fernando S. Pacheco
* Nov-2021
**************************************************************************/

#include <OneWire.h>
#include <DallasTemperature.h>

// GPIO where the DS18B20 is connected to
const int oneWireBus = 4;     

// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(oneWireBus);

// Pass our oneWire reference to Dallas Temperature sensor 
DallasTemperature sensors(&oneWire);

void setup() {
  // Start the Serial Monitor
  Serial.begin(115200);
  // Start the DS18B20 sensor
  sensors.begin();
  Serial.println("Sensor de Temperatura Dallas DS18B20");
}

void loop() {
  sensors.requestTemperatures(); 
  float temperatureC = sensors.getTempCByIndex(0);
  // Check if reading was successful
  if (temperatureC != DEVICE_DISCONNECTED_C) {
      Serial.print(temperatureC);
  }
  else {
    Serial.print(-1);
  }
  Serial.println("\tDS18B20-Celsius");

  delay(5000);
}
