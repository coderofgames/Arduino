// A couple of tests for arduino clock
// grabbed from various forums on the internet.
//
//

void setup() {
  int test = 0;
  if( test == 0 )
  {
      // put your setup code here, to run once:
    pinMode(3, OUTPUT);
    TCCR2A = 0x23;
    TCCR2B = 0x09;
    OCR2A = 3;
    OCR2B = 1;
  }
  else
  {
     pinMode(3,OUTPUT);
    
    TCCR2A = ((1 << WGM21) | (1 << COM2A0));
    TCCR2B = (4 << CS20); // prescaler
    TIMSK2 = 0;
    OCR2A = 3; 
  }
}

void loop() {
  // put your main code here, to run repeatedly:

}


