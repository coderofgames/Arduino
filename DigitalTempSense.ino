/* DS18B20 1-Wire digital temperature sensor with Arduino example code. More info: https://www.makerguides.com */

// Include the required Arduino libraries:
#include <OneWire.h>
#include <DallasTemperature.h>

// Round side facing, Ground, Vcc, Data
// Define to which pin of the Arduino the 1-Wire bus is connected:
#define ONE_WIRE_BUS 2

// Create a new instance of the oneWire class to communicate with any OneWire device:
OneWire oneWire(ONE_WIRE_BUS);

// Pass the oneWire reference to DallasTemperature library:
DallasTemperature sensors(&oneWire);

bool relayState = 1;
int BLUE = 4;
int GREEN = 5;
int RED = 6;

void setup() {
  // Begin serial communication at a baud rate of 9600:
  Serial.begin(9600);
  // Start up the library:
  sensors.begin();
  pinMode(BLUE, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(RED, OUTPUT);
}

void loop() {
  // Send the command for all devices on the bus to perform a temperature conversion:
  sensors.requestTemperatures();

  // Fetch the temperature in degrees Celsius for device index:
  float tempC = sensors.getTempCByIndex(0); // the index 0 refers to the first device
  // Fetch the temperature in degrees Fahrenheit for device index:
  float tempF = sensors.getTempFByIndex(0);

#if 1
  // Print the temperature in Celsius in the Serial Monitor:
  Serial.print("Temperature: ");
  Serial.print(tempC);
  Serial.print(" \xC2\xB0"); // shows degree symbol
  Serial.print("C  |  ");

  // Print the temperature in Fahrenheit
  Serial.print(tempF);
  Serial.print(" \xC2\xB0"); // shows degree symbol
  Serial.println("F");
#endif

  digitalWrite(BLUE, LOW);
  digitalWrite(GREEN, LOW);
  digitalWrite(RED, LOW);

  if( tempC < 20 ){
    digitalWrite(BLUE, HIGH);
  }
  else if( tempC >= 20.0 && tempC <= 21.5 ){
    digitalWrite(BLUE, HIGH);
    digitalWrite(GREEN, HIGH);
  }
  else if( tempC > 21.5 && tempC <= 23.5 ){
    digitalWrite(GREEN, HIGH);    
  }
  else if( tempC > 23.5 && tempC <= 25 ){
    digitalWrite(GREEN, HIGH);
    digitalWrite(RED, HIGH);
  }
  else if( tempC > 25 ){
    digitalWrite(RED, HIGH);
  }

  // Wait 30 second:
  delay(1000);
}
