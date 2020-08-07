#include "Timer1.h"

void setup() 
{
 Serial.begin(9600);
 Serial.println(F("Setup Done"));
 timer1.initialize(1); //1KHz timer
 timer1.attachIntCompB(timerIsr);
 timer1.start();
 adc.begin();
 adc.attachInterrupt(adcIsr);
 adc.setAutoTriggerSource(TIMER1_CMP_B);
 adc.startConversion(0);
 DDRB |= 1<<PB5;
}

void loop() 
{
  
}
void timerIsr()
{
//  PORTB ^= 1<<PB5;
}

void adcIsr()
{
  PORTB ^= 1<<PB5;
}

