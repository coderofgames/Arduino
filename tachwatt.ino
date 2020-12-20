// Tachometer_For_Lathe_IR
// Display RPM rate of lathe spindle

// Uses:
// Arduino Nano V 3.0
// OSOYOO IR Infrared Sensor (available on Amazon)
// IM162 LCD Display (similar ones available on Amazon)

// Include the LCD and Timer library code:

#include <TimerOne.h>

// RPM values to track
const int IRSensorPin = 2;  // the number of the IR sensor input pin
const int ledPin = 13;          // the number of the LED pin on Nano
int ledState = HIGH;           // the current state of the output pin

// Note: This program was used at one time with a reed relay, for a bicycle cadence counter,
//       but it works all right with the input from the IR sensor. 
//       If I need to reuse it, all variables are still in place.
int inputState;                          // the current state from the input pin
int lastInputState = LOW;        // the previous InputState from the input pin
long lastDebounceTime = 0;   // the last time the output pin was toggled
long debounceDelay = 5;        // the debounce time; increase if the output flickers
long time;
long endTime;
long startTime;
int RPM = 0;



float lnTime = 0;

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
double LAST_WATT_SECONDS = 0.0;
double watts = 0.0;
double prev_watts = 0.0;
double time_inc_secs = 0.1;
double vcc=0;
double voltage=0;
double current=0;
long wattStartTime = 0;
long wattEndTime = 0;
long wattDelay = 5;

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
// ---------------------------------------------------------------
void setup(void) {
  pinMode(IRSensorPin, INPUT);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, ledState);
  
  Serial.begin(9600);


  // Startup text on display



  endTime = 0;
  Timer1.initialize(1000000);  // Set the timer to 60 rpm, 1,000,000 microseconds (1 second)
  Timer1.attachInterrupt(timerIsr);  // Attach the service routine here

}

// ---------------------------------------------------------------
void loop(void) {
  time = millis();
  int currentSwitchState = digitalRead(IRSensorPin);

  if (currentSwitchState != lastInputState) {
    lastDebounceTime = millis();
  }

  if ((time - lastDebounceTime) > debounceDelay) {
    if (currentSwitchState != inputState) {
      inputState = currentSwitchState;
      if (inputState == LOW) {
        digitalWrite(ledPin, LOW);
        calculateRPM(); // Real RPM from sensor
        
        ledState = !ledState;
        
      }
      else {
        digitalWrite(ledPin, HIGH);
      }
    }
    ComputeWatts();
  }
  
  lastInputState = currentSwitchState;
}


void ComputeWatts(){
      // take a number of analog samples and add them up
  while (sample_count < NUM_SAMPLES) {
    sum += analogRead(sensorPin);// * 0.0049;
    sample_count++;
    delay(10);
  }
  sample_count = 0;
//  vcc = readVcc() / 1000;
  //unsigned int adcvalue alogRead(sensorPin);
   //voltage = analogRead(sensorPin)* 0.0049;
   voltage = (sum / (NUM_SAMPLES))  * 0.0049;
  //double current = ((analogRead(sensorPin2)-2.5)/10)*1000;

sum=0;
  
  double vA = (analogRead(sensorPin2) / 1024.0) * 5000; // Gets you mV
  current = ((vA - 2500) / mVperAmp);
  
  
  double tV = voltage + (0.5)*(voltage - prev_V);
  prev_V = tV;


  watts = current * voltage;

  time_inc_secs = (time - wattEndTime)/1000;
  
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

  wattEndTime = time;
}



// ---------------------------------------------------------------
void calculateRPM() {
  startTime = lastDebounceTime;
  lnTime = startTime - endTime;
  RPM = 60000 / (startTime - endTime);
  endTime = startTime;
}

// --------------------------
// Custom ISR Timer Routine
// Timer set to rpm, see above
// --------------------------
void timerIsr()
{
  // Print RPM every second
  // RPM based on timer
  Serial.println("---------------");
  time = millis() / 1000;
  Serial.print(time);
  Serial.print(" RPM: ");
  Serial.print(RPM);
  Serial.print("| Voltage: ");
  Serial.print(prev_V*15.662);
   Serial.print("| Current(A): ");
  Serial.print(current);
  Serial.print("| Watts(W): ");
  Serial.print(watts); 
  Serial.print("| Watt / Cycle: ");
  Serial.print(WATT_SECONDS-LAST_WATT_SECONDS);
  LAST_WATT_SECONDS = WATT_SECONDS;
 /* lcd.clear( );
  lcd.setCursor(0, 0);
  lcd.print("RPM");
  lcd.setCursor(6, 0);
  lcd.print(RPM);
  lcd.setCursor(0,1);
  lcd.print("Rad/s");
  lcd.setCursor(6,1);
  lcd.print(RPM*3.14159/30);
  delay(500);*/
  RPM = 0;
}

// ---------------------------------------------------------------
