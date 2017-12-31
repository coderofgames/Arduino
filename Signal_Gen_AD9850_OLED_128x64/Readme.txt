This is an improved version of the signal generator using the AD9850 module
here is the chip datasheet: http://www.analog.com/media/en/technical-documentation/data-sheets/AD9850.pdf
I considered using a clock signal however this is uneccessary because this library solves the problem
https://github.com/F4GOJ/AD9850
Note: the sample will not work without the library. The pin connections are as follows...
link the OLED as in the OLED sample. Link the AD9850 as in the library example. I have attached a buzzer too, but it isn't used yet.
Then simply start it up and connect it to a scope. Mine produces a 10 MHz sine wave. Frequency adjustment will be added soon as well
as a better wiring diagram.