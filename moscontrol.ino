/*
Adafruit Arduino - Lesson 3. RGB LED
*/
int motorpin = 9;
int mspeed = 0;
int mstep = 15;
int potPin = 2;
int ledPin=13;
int val=0;

void setup()
{
  pinMode(motorpin, OUTPUT);
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);
  
}

void loop()
{
  val = analogRead(A0);
  //digitalWrite(ledPin, HIGH);
  //delay(val);
  //digitalWrite(ledPin, LOW);
  //delay(val);
  //Serial.print("Val: ");
  //  Serial.println(val);// print mSpeed value on Serial monitor (click on Tools->Serial Monitor)
analogWrite(motorpin, mspeed);// send peed value to motor
  //  Serial.print("Speed: ");
  //  Serial.println(mspeed);// print mSpeed value on Serial monitor (click on Tools->Serial Monitor)
  mspeed = (int)(float)(val)/4.0;
  // See video you for details.
  if (mspeed < 0 ) {
    mstep = 0;
  }  
  else if(mspeed > 255)
  {
    mspeed = 255;
  }
  
//delay(20);
}
