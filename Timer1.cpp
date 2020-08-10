#include "Timer1.h"

Timer1 timer1; //preinitiate timer1

ISR(TIMER1_COMPB_vect)          // interrupt service routine that wraps a user defined function supplied by attachInterrupt
{
  timer1.isrCallback();
}

void Timer1::initialize(float freq)
{
  TCCR1A = 0;
  TCCR1B = 0;

  TCCR1B |= (1 << WGM13) | (1 << WGM12);//Fast PWM mod
  TCCR1A |= (1 << WGM11) ;              //Fast PWM mod
  setTopValue(freq);
}

void Timer1::setTopValue(float freq)
{
  //  int sec = ms/1000;
  //  int freq = 1000/ms
//  uint32_t top = (F_CPU / 1000) * ms; //for fast pwms
  uint32_t top = (uint32_t)(F_CPU/freq);

  if (top < RESOLUTION)                   _prescalerBits = (1 << CS10);                  //No prescaler
  else if ((top = top >> 3) < RESOLUTION) _prescalerBits = (1 << CS11);                  //prescale by 1/8
  else if ((top = top >> 3) < RESOLUTION) _prescalerBits = (1 << CS11) | (1 << CS10);  //prescaler by 1/64
  else if ((top = top >> 2) < RESOLUTION) _prescalerBits = (1 << CS12);                //prescale by 1/256
  else if ((top = top >> 2) < RESOLUTION) _prescalerBits = (1 << CS12) | (1 << CS10);  //prescale by 1/1024
  else    top = RESOLUTION,               _prescalerBits = (1 << CS12) | (1 << CS10);  //out of bound. set as maximum
  Serial.print(F("Top :")); Serial.println(top);
  Serial.print(F("Prescalebits : "));Serial.println(_prescalerBits, BIN);
  byte oldReg = SREG;
  cli();
  ICR1 = top - 1;
  SREG = oldReg;

  //  TCCR1B &= ~((1<<CS12)|1<<CS11)|(1<<CS10)); //clearing prescaler select bits
  //  TCCR1B |= prescalerBits;
}

void Timer1::start()
{
  stop();
  TCCR1B |= _prescalerBits;
}
void Timer1::stop()
{
  TCCR1B &= ~((1 << CS12) | (1 << CS11) | (1 << CS10)); //Deselecting clock stops timer1
}

void Timer1::attachIntCompB(void (*isr)())
{
  if(isr !=NULL)
  {
  	isrCallback = isr;
  }
  sei();                    //Enable Global Interrupt
  TIMSK1 |= (1 << OCIE1B);  //Channel B update interrupt enable
//  start();                  //start timer. 
}
/*****************************ADC********************************/

Adc adc; //preinitiate adc

ISR(ADC_vect)
{
  adc.isrCallback();
}

void Adc::begin()
{
  
  ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); //Prescaler 128.So F_adc = 16MHz/128 = 125KHz
  ADMUX  |= (1 << REFS0);                               //Reference AVCC
  ADCSRA |= (1 << ADEN);                                //Enable ADC
  
}

void Adc::setAutoTriggerSource(byte source)
{
  if(source)
  {
    ADCSRA |= (1 << ADATE); //ADC Auto trigger enable
  }
  ADCSRB &= 0b11111000;   //Clearing trigger source bits
  ADCSRB |= source;
}

void Adc::attachInterrupt(void (*isr)())
{
  if(isr !=NULL)
  {
  	isrCallback = isr;
  }
  sei();                  //Enable Global Interrupt
  ADCSRA |= (1 << ADIE);  //ADC Conversion complete interrupt enable
  
}
void Adc::detachInterrupt()
{
  
}


void Adc::startConversion(byte channel)
{
  setChannel(channel);
  ADCSRA |= (1 << ADSC);  //start conversion
}
void Adc::setChannel(byte channel)
{
  ADMUX = (ADMUX & 0b11110000) | (channel);
}

