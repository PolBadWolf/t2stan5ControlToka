
#ifndef adc__h
#define adc__h

void adc_pusk( unsigned char n );//Инициализация АЦП
unsigned int adc_read();

extern unsigned long ACP_DANNIE;     // Полученные данные с АЦП
extern unsigned long ACP_DANNIE_RS;  // Подготовленные  данные ACP для передачи в RS232
extern unsigned long Smehenie_zero;  // Смещение нуля
extern unsigned long Korecsia_zero;  // Кооэффициент коррекции нуля для АЦП 


#endif

