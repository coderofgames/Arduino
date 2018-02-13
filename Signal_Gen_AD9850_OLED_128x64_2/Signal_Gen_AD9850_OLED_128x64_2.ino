


#include <AD9850.h>


//======================================================================
const unsigned long max_frequency_step = 1000000; //Max Frequency step
const unsigned long max_frequency = 50000000; //Max Frequency
const int min_frequency=25; // Minimum Frequency


unsigned long last_frequency = 5000;
unsigned long frequency_step = 1;

const int EncoderPinCLK = 2; 
const int EncoderPinDT = 3;  
const int EncoderPinSW = 4;  
//=====================================================================


const int W_CLK_PIN = 7;
const int FQ_UD_PIN = 8;
const int DATA_PIN = 9;
const int RESET_PIN = 10;

double freq = 10000000; // not used
double trimFreq = 124999500; // used for calibration

int phase = 0; // not currently used

// Updated by the ISR (Interrupt Service Routine)
unsigned volatile long frequency = 5000;

// interrupt service request
void isr ()  
{
    static unsigned long lastInterruptTime = 0;
    unsigned long interruptTime = millis();

    if (interruptTime - lastInterruptTime > 5) 
    {
        if (digitalRead(EncoderPinDT) == LOW)
        {
            frequency=frequency-frequency_step ; // Could be -5 or -10
        }
        else 
        {
            frequency=frequency+frequency_step ; // Could be +5 or +10
        }


        frequency = min(max_frequency, max(min_frequency, frequency));

        lastInterruptTime = interruptTime;
    }
}

void setup_DDS()
{
    DDS.begin(W_CLK_PIN, FQ_UD_PIN, DATA_PIN, RESET_PIN);
    DDS.calibrate(trimFreq);
}



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


#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

// signal pin
int SIGNAL = 11;

const int SWEEP = 13;
bool sweep = false;

void show_frequency()
{
    // text display tests

    display.clearDisplay(); 
    // lcd.clear();
    float display_frequency=frequency;
    
    // low on memory so setting this here 
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
    if( sweep )
    {
        display.print("SWEEP :");
    }
    else display.print("STEP :"); //display.println(0xDEADBEEF, HEX);
    display.setCursor(0,55); 
    display.println(frequency_step); //display.println(0xDEADBEEF, HEX);


    display.display();

}




void setup()   
{                
    Serial.begin(9600);

    setup_DDS();
  
    // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
     display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3D (for the 128x64)
    // init done
  
    // Show image buffer on the display hardware.
    // Since the buffer is intialized with an Adafruit splashscreen
    // internally, this will display the splashscreen.
    display.display();
    delay(2000);

    // Rotary pulses are INPUTs
    pinMode(EncoderPinCLK, INPUT);
    pinMode(EncoderPinDT, INPUT);

    // Switch is floating so use the in-built PULLUP so we don't need a resistor
    pinMode(EncoderPinSW, INPUT_PULLUP);  

    // Attach the routine to service the interrupts
    attachInterrupt(digitalPinToInterrupt(EncoderPinCLK), isr, LOW);

    pinMode(SIGNAL, INPUT);
    pinMode(SWEEP, INPUT);
}

bool sweep_complete = false;

void loop() {
  // process the rotary encoder 
    if( digitalRead(SWEEP)== HIGH && !sweep_complete)
    {
        sweep_freq(20, 22000, 220);
        sweep = true;
    }
    else sweep = false;
    
    if ((!digitalRead(EncoderPinSW))) 
    {
        while (!digitalRead(EncoderPinSW))
          delay(10);
        // change the frequency step when the encoder is clicked
        if (frequency_step>max_frequency_step)
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
 
    if (frequency != last_frequency) 
    {
        Serial.print(frequency > last_frequency ? "Up  :" : "Down:");
        Serial.println(frequency);
    
        DDS.setfreq(frequency, phase);
        last_frequency = frequency ;
    }

    show_frequency();
}

// need to disable all interupts for sweep.
void sweep_freq(unsigned long start_freq, unsigned long end_freq, int num_steps)
{
    frequency = start_freq;
    DDS.setfreq(frequency, phase);

    unsigned long sweep_step = (unsigned long)(float(end_freq - start_freq) / num_steps);
    for( int i = 0; i < num_steps; i++ )
    {
        frequency=frequency+sweep_step; // Could be +5 or +10
        DDS.setfreq(frequency, phase);
        show_frequency();
        delay(10);
    }
    sweep_complete = true;

}


