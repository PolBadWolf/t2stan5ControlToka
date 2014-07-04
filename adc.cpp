
#include "adc.h"
#include "main.h"

namespace nsAcp
{
    // unsigned int  ACP_DANNIE=0;     // ���������� ������ � ���
    unsigned long ACP_DANNIE_RS=0;  // ��������������  ������ ACP ��� �������� � RS232
    unsigned long Smehenie_zero=0;  // �������� ����
    unsigned long Korecsia_zero=0;  // ������������ ��������� ���� ��� ��� 
    unsigned long ACP=0;            // ���������� ��� ���
    //unsigned int Rezult_ADC = 0;    // ���������� ��� ������ �������� � ��� �� ���������
    
    void (*CallBackFn)(unsigned int rez);
    
    unsigned int adc_read()
    {
      unsigned int temp;
      LOW(temp) = ADCL;
      HIGH(temp) = ADCH & 3;
      return temp;
    }
    
    // ������������� ���
    void adc_pusk(unsigned char n)
    {
        CallBackFn = 0;
        ADMUX = n;    // ��������� ���������
        ADCSRA = 0xD7;
    }
    void adc_pusk(unsigned char n, void (*CallBackFunction)(unsigned int rez) )
    {
        CallBackFn = CallBackFunction;
        ADMUX = n;    // ��������� ���������
        ADCSRA = 0xD7;
    }
    
    // ���
    void adc_sys()
    {
        unsigned int rADC = 0;
        ((unsigned char *)&rADC)[0] = ADCL;
        ((unsigned char *)&rADC)[1] = ADCH & 3;
        if (CallBackFn!=0)
            CallBackFn(rADC);
    }
    
    
    
    // =========================================================
    // = ��������� ���                                         =
    // =========================================================
    #pragma vector = ADC_vect
    __interrupt void adc_interrupt(void)
    {
      adc_sys();
    }
    void init()
    {
                 /*����������� ���*/
((unsigned char *)&ACP)[0] = ReadEeprom(19);// - 1����
((unsigned char *)&ACP)[1] = ReadEeprom(20);// - 2����
((unsigned char *)&ACP)[2] = ReadEeprom(21);// - 3����
((unsigned char *)&ACP)[3] = ReadEeprom(22);// - 4����
    }

}

