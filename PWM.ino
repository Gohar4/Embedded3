#include <avr/io.h>
#include "PWM.h"

void PWM_init(){

  DDRB = DDRB | (1 << PB1); //PB1 as output

  TCCR1A = TCCR1A | (1 << WGM11) | (1 << WGM10); // fast PWM
  TCCR1B = TCCR1B & ~ (1 << WGM12);

  TCCR1B = TCCR1B & ~ (1 << CS10); // select pre-scaler
  TCCR1B = TCCR1B & ~ (1 << CS11);
  TCCR1B = TCCR1B | (1 << CS12);

  TCCR1A = TCCR1A | (1 << COM1A1); //select PB1
  TCCR1A = TCCR1A & ~ (1 << COM1A0);

}

void Set_DutyCycle(int duty){
  int value = (255*duty/100); 
  OCR1A = value;

}