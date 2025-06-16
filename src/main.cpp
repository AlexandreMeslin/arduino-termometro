#include <Arduino.h>
// Include the libraries we need
#include <OneWire.h>
#include <DallasTemperature.h>
#include <MultiFuncShield.h>

// Data wire is plugged into port 2 on the Arduino
#define ONE_WIRE_BUS A4

// Adiciona um tempo de espera para estabilização (em ms)
#define TEMPERATURE_WAIT 750

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);

void setup() {
  // start serial port
  Serial.begin(9600);
  Serial.println("Dallas Temperature IC Control Library Demo");

  // Start up the library
  sensors.begin();
  sensors.setResolution(12); // Define resolução de 12 bits (padrão)
  Serial.print("Found ");
  Serial.print(sensors.getDeviceCount());
  Serial.println(" devices.");
  DeviceAddress sensorAddress;
  if (sensors.getAddress(sensorAddress, 0)) {
    Serial.print("Sensor Address: ");
    for (uint8_t i = 0; i < 8; i++) {
      Serial.print("0x"); Serial.print(sensorAddress[i], HEX); Serial.print(" ");
    }
    Serial.println();
  }
  Timer1.initialize();
  MFS.initialize(&Timer1);
}

void loop() {
  // call sensors.requestTemperatures() to issue a global temperature 
  // request to all devices on the bus
  Serial.print("Requesting temperatures...");
  sensors.requestTemperatures(); // Send the command to get temperatures
  // Adicione um delay após requestTemperatures() para conversão
  delay(TEMPERATURE_WAIT);
  Serial.println("DONE");
  // After we got the temperatures, we can print them here.
  // We use the function ByIndex, and as an example get the temperature from the first sensor only.
  float tempC = sensors.getTempCByIndex(0);
  // Check if reading was successful
  if(tempC != DEVICE_DISCONNECTED_C) {
    Serial.print("Temperature for the device 1 (index 0) is: ");
    Serial.print(tempC);
    Serial.println("°C");
    MFS.write(tempC);
  } 
  else {
    Serial.println("Error: Could not read temperature data");
  }
  delay(5000);
}
