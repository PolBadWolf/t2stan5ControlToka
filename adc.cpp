
#include "adc.h"
#include "main.h"

unsigned long ACP_DANNIE=0;     // Полученные данные с АЦП
unsigned long ACP_DANNIE_RS=0;  // Подготовленные  данные ACP для передачи в RS232
unsigned long Smehenie_zero=0;  // Смещение нуля
unsigned long Korecsia_zero=0;  // Кооэффициент коррекции нуля для АЦП 

unsigned int adc_read()
{
  unsigned int temp;
  LOW(temp) = ADCL; /* ?????? ???????? ??? */
  HIGH(temp) = ADCH & 3;
  return temp;
}

void adc_pusk( unsigned char n )//Инициализация АЦП
{
  ADMUX = n;    // Запустить измерение
  ADCSRA = 0xD7;

}



// АЦП
void adc_sys(void)
{
Rezult_ADC=adc_read();
}



// =========================================================
// = обработка АЦП                                         =
// =========================================================
#pragma vector = ADC_vect
__interrupt void adc_interrupt(void)
{
  adc_sys();
}








