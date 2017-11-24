
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

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2


#define LOGO16_GLCD_HEIGHT 16 
#define LOGO16_GLCD_WIDTH  16 
static const unsigned char PROGMEM logo16_glcd_bmp[] =
{ B00000000, B11000000,
  B00000001, B11000000,
  B00000001, B11000000,
  B00000011, B11100000,
  B11110011, B11100000,
  B11111110, B11111000,
  B01111110, B11111111,
  B00110011, B10011111,
  B00011111, B11111100,
  B00001101, B01110000,
  B00011011, B10100000,
  B00111111, B11100000,
  B00111111, B11110000,
  B01111100, B11110000,
  B01110000, B01110000,
  B00000000, B00110000 };

#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

#include <EEPROM.h>

const unsigned long max_frequency_step = 1000000; //Max Frequency step
const unsigned long max_frequency = 50000000; //Max Frequency
const int min_frequency=25; // Minimum Frequency

unsigned long last_frequency = 5000;
unsigned long frequency_step = 1;

// Rotary encoder

const int EncoderPinCLK = 2; 
const int EncoderPinDT = 3;  
const int EncoderPinSW = 4;  

byte dds_RESET = 5;
byte dds_DATA  = 6;
byte dds_LOAD  = 7;
byte dds_CLOCK = 8;


// Updated by the ISR (Interrupt Service Routine)
unsigned volatile long frequency = 5000;

void isr ()  {
  static unsigned long lastInterruptTime = 0;
  unsigned long interruptTime = millis();

  if (interruptTime - lastInterruptTime > 5) {
    if (digitalRead(EncoderPinDT) == LOW)
    {
      frequency=frequency-frequency_step ; // Could be -5 or -10
    }
    else {
      frequency=frequency+frequency_step ; // Could be +5 or +10
    }

    frequency = min(max_frequency, max(min_frequency, frequency));

    lastInterruptTime = interruptTime;
  }
}

void show_frequency()
{
  // text display tests

  display.clearDisplay(); 
 // lcd.clear();
  float display_frequency=frequency;

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0); 
  
  if (frequency<1000)
  {
    display.print("freq (Hz):");
    display.setCursor(0,20);  
    display.print(frequency);
  }
  
  if (frequency>=1000)
  {  
    display.print("freq (kHz):");
    display.setCursor(0,20);  
    display.println(display_frequency/1000);      
  }

  display.setCursor(0,40);  
  display.print("STEP (Hz):"); //display.println(0xDEADBEEF, HEX);
  display.setCursor(0,55); 
  display.println(frequency_step); //display.println(0xDEADBEEF, HEX);
  display.display();

}

void setup() {
  Serial.begin(9600);

  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3D (for the 128x64)
  // init done
  
  // Show image buffer on the display hardware.
  // Since the buffer is intialized with an Adafruit splashscreen
  // internally, this will display the splashscreen.
  display.display();
  
  delay(2000);

  // Clear the buffer.
  display.clearDisplay(); 
  
  // Rotary pulses are INPUTs
  pinMode(EncoderPinCLK, INPUT);
  pinMode(EncoderPinDT, INPUT);

  // Switch is floating so use the in-built PULLUP so we don't need a resistor
  pinMode(EncoderPinSW, INPUT_PULLUP);

  // Attach the routine to service the interrupts
  attachInterrupt(digitalPinToInterrupt(EncoderPinCLK), isr, LOW);
  //lcd.begin();
  setup_dds();

  show_frequency();
  dds(frequency);
  Serial.println("Start");
}

void loop() {
  // Is someone pressing the rotary switch?
  if ((!digitalRead(EncoderPinSW))) {
    while (!digitalRead(EncoderPinSW))
      delay(10);
    Serial.println("Reset");
    if (frequency_step==max_frequency_step)
    {
      frequency_step=1;
    }
    else
    {
      frequency_step=frequency_step*10;  
    }
    Serial.print("multiplier:");
    Serial.println(frequency_step);

  }
 
  if (frequency != last_frequency) {
    Serial.print(frequency > last_frequency ? "Up  :" : "Down:");
    Serial.println(frequency);
    
    dds(frequency);
    last_frequency = frequency ;
  }
  show_frequency();
}
