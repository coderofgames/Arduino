This outputs a clock signal from pin 11 of an Arduino. The oscilloscope 
must be grounded to the Arduino grouund.

Setting the number of the ocr2aval sets the clock frequency.

Note: The output is something like +- 5 volts so probably
(according to my scope)


I do not fully understand the extent of the radiation from the wire via the electro magnetic fields 
(square waves have a sharp/small rise time, or rapid change in emf) the best policy is probably to scale 
the voltage down on the breadboard using a potential divider. 

Vo/Vi = R2/(R1+R2), 
so Vo*R1 = R2*(Vi-Vo), 

with Vi=5, Vo=0.05, then Vi-Vo = 4.95

therefore 0.05 * R1 = 4.95 * R2
R1 = 4.95/0.05 *R2 = 99 * R2

so R1 = 99 Ohm is a value that scales 5V to 50mV

(hifi DIY websites talk about radiation shielding from speakers).