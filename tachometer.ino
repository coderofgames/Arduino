int sensorvalue;
int state1 = HIGH;
int state2;
float rps;
float rpm;
long prevMillis = 0;
long interval = 200;
long currentTime;
long prevTime = 1;
long diffTime;
int sensorthreshold = 450;  
// this value indicates the limit reading between dark and light,
// it has to be tested as it may change acording to the 
// distance the leds are placed.
// to see what number is good, check the sensorvalue variable value
// as printed out in the serial monitor

#include <LiquidCrystal.h>

// LCD pins <--> Arduino pins
const int RS = 11, EN = 12, D4 = 7, D5 = 8, D6 = 9, D7 = 10     ;
LiquidCrystal lcd(RS, EN, D4, D5, D6, D7);


void setup()
{
  Serial.begin(9600);
  pinMode(13,OUTPUT);      // assign pin 13 led as indicator because we cannot se the IR light
        //   pinMode(12,OUTPUT);
        //   digitalWrite(12,HIGH);          
        
          lcd.begin(16, 2); // set up number of columns and rows
                                                                   
}
void loop()
{


  sensorvalue = analogRead(A1); // read from pin 0
  //Serial.println(sensorvalue);
  if(sensorvalue < sensorthreshold)
  { 
    state1 = HIGH;
  }
   else {
    state1 = LOW;
   }
   
   digitalWrite(13,state1);   // as iR light is invisible for us, the led on pin 13 
                              // indicate the state of the circuit.

   if(state2!=state1){      //counts when the state change, thats from (dark to light) or 
                            //from (light to dark), remember that IR light is invisible to us.
     if (state2>state1){
       currentTime = micros();   // Get the arduino time in microseconds
       diffTime = currentTime - prevTime;  // calculate the time diff from the last meet-up
       rps = 1000000/(float)diffTime;   // calculate how many rev per second, good to know
       rpm = 60000000/(float)diffTime;  // calculate how many rev per minute
       
       unsigned long currentMillis = millis();
       
       // print to serial at every interval - defined at the variables declaration
       if(currentMillis - prevMillis > interval){ // see if now already an interval long
       prevMillis = currentMillis;       
      // Serial.print(rps, DEC); Serial.print(" rps  "); Serial.print(rpm,DEC); Serial.println(" rpm");
                 // move cursor to   (0, 0)
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Rad/s: ");        
        lcd.print((rps*2*3.14159));        // print message at (0, 0)
        lcd.setCursor(0, 1);         // move cursor to   (2, 1)
        lcd.print("RPM: ");
        lcd.print(rpm); // print message at (2, 1)  
       }
       
       prevTime = currentTime;
   }
     state2 = state1;
   }


 //only for testing to determine the sensorthreshold value
 //delay(500);
// Serial.println(sensorvalue);
  /**/
}
