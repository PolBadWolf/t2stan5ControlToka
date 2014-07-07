
#ifndef adc__h
#define adc__h

namespace nsAcp
{
    // Инициализация АЦП
    void adc_pusk(unsigned char n);
    void adc_pusk(unsigned char n, void (*CallBackFunction)(unsigned int rez) );
    unsigned int adc_read();
    void init();
    
    //extern unsigned long ACP_DANNIE;     // Полученные данные с АЦП
    extern unsigned long ACP_DANNIE_RS;  // Подготовленные  данные ACP для передачи в RS232
    extern unsigned long Smehenie_zero;  // Смещение нуля
    extern unsigned long Korecsia_zero;  // Кооэффициент коррекции нуля для АЦП 
//    extern unsigned long ACP;//Коэфициент для АЦП
//    extern unsigned int Rezult_ADC;      // Переменная для вывода значения с АЦП на индикатор
}
#endif

