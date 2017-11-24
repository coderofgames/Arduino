This is a modification using an OLED instead of an LCD of the code and schematic found here

https://create.arduino.cc/projecthub/umar-sear/arduino-based-dds-signal-generator-using-ad9851-ed4d8e

the most important difference is the fact that the LCD CS pin is connected to the potentiometer, however Nano D11 is also connected 
to the CS pin, so I just use the Nano.

The project is successful in displaying the correct information on the OLED using the rotary encoder, 
however I have not been able to verify the output of the signal generator. 
