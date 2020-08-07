#ifndef _TIMER1_H
#define _TIMER1_H
#include <Arduino.h>


#define RESOLUTION 65536    // Timer1 is 16 bit

class Timer1
{
  public:
    void initialize(float freq);
    void setTopValue(float freq);
    //    void attachInterrupt(void (*isr)());
    void attachIntCompB(void (*isr)());
    //    void detachInterrupt();
    void start();
    void stop();
    void (*isrCallback)(); //Function pointer for interrupt routine
  private:
    unsigned char _prescalerBits;
};

/**************Macro definition for ADC Auto Trigger source**********/
#define FREE_RUNNING 0b000
#define TIMER1_CMP_B 0b101


class Adc
{
  public :
    void begin();
    void attachInterrupt(void (*isr)()); 
    void detachInterrupt();
    void setChannel(byte channel);
    void (*isrCallback)(); //Function pointer for interrupt routine
    void setAutoTriggerSource(byte source);
    void startConversion(byte channel = 0);
};

extern Timer1 timer1;
extern Adc adc;
#endif
