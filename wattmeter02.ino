/*********************************************************************

This is an example for our Monochrome OLEDs based on SSD1306 drivers



  Pick one up today in the adafruit shop!

  ------> http://www.adafruit.com/category/63_98



This example is for a 128x64 size display using I2C to communicate

3 pins are required to interface (2 I2C and one reset)



Adafruit invests time and resources providing this open source code, 

please support Adafruit and open-source hardware by purchasing 

products from Adafruit!



Written by Limor Fried/Ladyada  for Adafruit Industries.  

BSD license, check license.txt for more information

All text above, and the splash screen must be included in any redistribution

*********************************************************************/



//#include <SPI.h>

//#include <Wire.h>

#include <Adafruit_GFX.h>

#include <Adafruit_SSD1306.h>



//#define OLED_RESET 4

Adafruit_SSD1306 display(4);


#define LOGO16_GLCD_HEIGHT 16 

#define LOGO16_GLCD_WIDTH  16 

#if (SSD1306_LCDHEIGHT != 64)

#error("Height incorrect, please fix Adafruit_SSD1306.h!");

#endif
/*
OLED pins
yellow a4  SDA
black a5   SCL
brown 5V   VCC
blue GND   GND

current sense pins
(right to left upside down)
5v
A1
gnd

potential divider pins
A0
*/

int sensorPin = A0;    // select the input pin for the potentiometer
int sensorPin2 = A1;   


int state = 0;

float recipDividerVal = 15.662; // 1/0.064;, or R1 = 810k, R2 = 55.6, R2/(R1+R2) = 0.064
#define mVperAmp 100

// number of analog samples to take per reading
#define NUM_SAMPLES 20

int sum = 0;                    // sum of samples taken
unsigned char sample_count = 0; // current sample number

double prev_V = 0.0;


double WATT_SECONDS = 0.0;
double watts = 0.0;
double prev_watts = 0.0;

long time_inc_secs = 0;
long last_time = 0;
long time = 0;

double vcc;
double voltage;
double current;


long readVcc() {
  long result;
  // Read 1.1V reference against AVcc
  ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
  delay(2); // Wait for Vref to settle
  ADCSRA |= _BV(ADSC); // Convert
  while (bit_is_set(ADCSRA,ADSC));
  result = ADCL;
  result |= ADCH<<8;
  result = 1125300L / result; // Back-calculate AVcc in mV
  return result;
}


void setup()   
{       //38400         
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3D (for the 
  display.display();
  delay(2000);
  last_time = millis(); 

  delay(1);
  
}


void loop() {

  time = millis();
  time_inc_secs = (time-last_time)/1000;
  last_time = time;

      // take a number of analog samples and add them up
  while (sample_count < NUM_SAMPLES) {
      sum += analogRead(sensorPin);// * 0.0049;
      sample_count++;
      delay(10);
  }
  sample_count = 0;
   voltage = (sum / (NUM_SAMPLES))  * 0.0049;

  sum=0;
  
  double vA = (analogRead(sensorPin2) / 1024.0) * 5000; // Gets you mV
  current = ((vA - 2500) / mVperAmp);
  
  
  double tV = voltage + (0.5)*(voltage - prev_V);
  prev_V = tV;


  watts = current * voltage;
  WATT_SECONDS += current * tV * time_inc_secs;

  

  if( watts > prev_watts )
  {
      WATT_SECONDS += (watts - prev_watts)*time_inc_secs*0.5 + prev_watts *time_inc_secs;
  }
  else
  {
      WATT_SECONDS += (prev_watts - watts)*time_inc_secs*0.5 + watts *time_inc_secs;
  }
  
  prev_watts = watts;

      display.clearDisplay(); 
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0,0); 
    display.print("VOLTAGE:");
    display.setCursor(60,0);
    display.print(prev_V*15.662);
  //  display.print(prev_V*14.9584);
    //display.print(prev_voltage[ circ_array_elem % CIRC_ARRAY_COUNT ]);
    display.setCursor(0,20);
    display.print("CURRENT(A):");
    display.setCursor(80,20);
    display.print(current);

    display.setCursor(0,35);
        display.print("WATTS:");
    display.setCursor(80,35);
    display.print(watts);

        display.setCursor(0,45);
        display.print("WATT HOURS:");
    display.setCursor(80,45);
    display.print(WATT_SECONDS/3600); // should print watt hours
    display.display();


}
