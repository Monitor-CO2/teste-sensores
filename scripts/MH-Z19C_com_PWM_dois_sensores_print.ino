/*************************************************************************
* 2 MH-Z19C connected to a NodeMCU ESP32 (UART and PWM output from sensors)
* Output to Serial
* 
* MH-Z19C needs external power supply (5V). Do not connect directly the 
* sensor VCC pin to USB (Vin at NodeMCU) because it demands too much 
* power and makes serial communication fail. 
* IMPORTANT: Remember to connect GND of the power source to NodeMCU GND
*
* Serial communication to the sensor works intermitently.
* PWM is always working, but you lose the temperature reading.
* Here, both (UART and PWM) are included.
* 
* Fernando S. Pacheco
* Nov-2021
**************************************************************************/

#include <MHZ.h>

#undef DEBUG

// SENSOR A
// pin for pwm reading
#define SENSOR_A_CO2_IN_PWM 23
// pin for uart reading
#define SENSOR_A_MH_Z19_RX 16
#define SENSOR_A_MH_Z19_TX 17

// SENSOR B
// pin for pwm reading
#define SENSOR_B_CO2_IN_PWM 39
// pin for uart reading
#define SENSOR_B_MH_Z19_RX 34
#define SENSOR_B_MH_Z19_TX 25

// assim usa soft serial. Funciona por um tempo e depois parece que perde comunicacao.
// Mas com hard serial tambem nao funcionou direito
MHZ co2_A(SENSOR_A_MH_Z19_RX, SENSOR_A_MH_Z19_TX, SENSOR_A_CO2_IN_PWM, MHZ19C);
MHZ co2_B(SENSOR_B_MH_Z19_RX, SENSOR_B_MH_Z19_TX, SENSOR_B_CO2_IN_PWM, MHZ19C);

int ppm_uart;
int ppm_pwm;
int temperature;

void printSerial(int value) {
  if (value > 0) {
      Serial.print(value);
    } else {
      Serial.print("-1");
  }
  Serial.print("\t");
}

void setup() {
  Serial.begin(115200);
  pinMode(SENSOR_A_CO2_IN_PWM, INPUT);
  pinMode(SENSOR_B_CO2_IN_PWM, INPUT);
  delay(100);
  Serial.println("MHZ 19C");

  // enable debug to get addition information
  #ifdef DEBUG
    co2_A.setDebug(true);
    co2_B.setDebug(true);
  #endif

  if (co2_B.isPreHeating()) {
    Serial.print("Preheating MHZ 19C");
    while (co2_B.isPreHeating()) {
      Serial.print(".");
      delay(5000);
    }
    Serial.println();
    Serial.print("MHZ_19C_CO2ppm_UART_SENSOR_A\tMHZ_19C_CO2ppm_PWM_S_A\tMHZ_19C_TEMPCelsius_S_A\t");
    Serial.println("MHZ_19C_CO2ppm_UART_SENSOR_B\tMHZ_19C_CO2ppm_PWM_S_B\tMHZ_19C_TEMPCelsius_S_B\t");
  }
}

void loop() {
  // SENSOR A
  ppm_uart = co2_A.readCO2UART();
  printSerial(ppm_uart);

  ppm_pwm = co2_A.readCO2PWM();
  printSerial(ppm_pwm);

  temperature = co2_A.getLastTemperature();
  printSerial(temperature);

  // SENSOR B
  ppm_uart = co2_B.readCO2UART();
  printSerial(ppm_uart);

  ppm_pwm = co2_B.readCO2PWM();
  printSerial(ppm_pwm);

  temperature = co2_B.getLastTemperature();
  printSerial(temperature);
  
  Serial.println("MHZ19C");
  delay(5000);
}
