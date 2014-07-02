
#include "adc.h"
#include "main.h"

unsigned long ACP_DANNIE=0;     // ���������� ������ � ���
unsigned long ACP_DANNIE_RS=0;  // ��������������  ������ ACP ��� �������� � RS232
unsigned long Smehenie_zero=0;  // �������� ����
unsigned long Korecsia_zero=0;  // ������������ ��������� ���� ��� ��� 

unsigned int adc_read()
{
  unsigned int temp;
  LOW(temp) = ADCL; /* ?????? ???????? ??? */
  HIGH(temp) = ADCH & 3;
  return temp;
}

void adc_pusk( unsigned char n )//������������� ���
{
  ADMUX = n;    // ��������� ���������
  ADCSRA = 0xD7;

}



// ���
void adc_sys(void)
{
Rezult_ADC=adc_read();
}



// =========================================================
// = ��������� ���                                         =
// =========================================================
#pragma vector = ADC_vect
__interrupt void adc_interrupt(void)
{
  adc_sys();
}








