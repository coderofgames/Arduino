#######################################################
NOTE: YOU HAVE TO TURN THE ROTARY ENCODER TO START IT 
#######################################################

This is an improved version of the signal generator using the AD9850 module
here is the chip datasheet: http://www.analog.com/media/en/technical-documentation/data-sheets/AD9850.pdf
I considered using a clock signal however this is uneccessary because this library solves the problem
https://github.com/F4GOJ/AD9850
Note: the sample will not work without the library. The pin connections are as follows...

link the OLED as in the OLED sample.
SDA - A4 (analog 4)
SCL - A5
VCC - +5V on Arduino  
GND - GND on Arduino
 
Link the AD9850 as in the library example. 
VCC - +5V on Arduino
W_CLK - D7
FU_UD - D8
DATA  - D9
RESET - D10
GND - GND on Arduino
ZOUT2 - Oscilloscope probe input
GND - Oscilloscope probe ground

Rotary Encoder Module
CLK - D2
DT - D3
SW - D4
+  - +5V on Arduino
GND - GND on Arduino

Then upload the code, make sure you have an arduino with enough memory. There is also pin 13 (D13) on the Arduino that I 
have used for a switch to start the frequency sweep, so use whatever button you like there. At this stage the device must be
reset after a frequency sweep, and since the memory is low (on my Arduino Nano) I have not set up variable frequency sweeps, so I just
sweep over the audio frequency range in steps of 100Hz, this step size may change.

#####################################
Frequency Sweep is temporarily Broken
#####################################