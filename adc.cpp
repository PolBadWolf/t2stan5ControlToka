
#include "adc.h"
#include "main.h"

namespace nsAcp
{
    // unsigned int  ACP_DANNIE=0;     // Полученные данные с АЦП
    unsigned long ACP_DANNIE_RS=0;  // Подготовленные  данные ACP для передачи в RS232
    unsigned long Smehenie_zero=0;  // Смещение нуля
    unsigned long Korecsia_zero=0;  // Кооэффициент коррекции нуля для АЦП 
    unsigned long ACP=0;            // Коэфициент для АЦП
    //unsigned int Rezult_ADC = 0;    // Переменная для вывода значения с АЦП на индикатор
    
    void (*CallBackFn)(unsigned int rez);
    
    unsigned int adc_read()
    {
      unsigned int temp;
      LOW(temp) = ADCL;
      HIGH(temp) = ADCH & 3;
      return temp;
    }
    
    // Инициализация АЦП
    void adc_pusk(unsigned char n)
    {
        CallBackFn = 0;
        ADMUX = n;    // Запустить измерение
        ADCSRA = 0xD7;
    }
    void adc_pusk(unsigned char n, void (*CallBackFunction)(unsigned int rez) )
    {
        CallBackFn = CallBackFunction;
        ADMUX = n;    // Запустить измерение
        ADCSRA = 0xD7;
    }
    
    // АЦП
    void adc_sys()
    {
        unsigned int rADC = 0;
        ((unsigned char *)&rADC)[0] = ADCL;
        ((unsigned char *)&rADC)[1] = ADCH & 3;
        if (CallBackFn!=0)
            CallBackFn(rADC);
    }
    
    
    
    // =========================================================
    // = обработка АЦП                                         =
    // =========================================================
    #pragma vector = ADC_vect
    __interrupt void adc_interrupt(void)
    {
      adc_sys();
    }
    void init()
    {
                 /*Коэффициент АЦП*/
((unsigned char *)&ACP)[0] = ReadEeprom(19);// - 1байт
((unsigned char *)&ACP)[1] = ReadEeprom(20);// - 2байт
((unsigned char *)&ACP)[2] = ReadEeprom(21);// - 3байт
((unsigned char *)&ACP)[3] = ReadEeprom(22);// - 4байт
    }

}

