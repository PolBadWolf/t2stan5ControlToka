
#include "timer.h"
#include "main.h"

//---------------------------------Переменные, флаги, константы--------------------------------------------------------------
unsigned long Massiv_ACP [100];//Массив АЦП
unsigned char Index_M=0;//Индекс массива
unsigned char Flag_end=0;//Флаг регистрации заполнения массива
unsigned long summa=0;//Переменная для хранения суммированых результатов с АЦП
unsigned long Rezult_ADC=0;//Переменная для вывода значения с АЦП на индикатор
unsigned long acp_date=0;//Голые данные с ацп
//----------------------------------------------------------------------------------------------------------------------------
//-------------------------Переменные и флаги для Задержки передачи данных в RS-----------------------------------------------
unsigned int Z1,Z2;
unsigned char Start_Data_RS485=0;//Если 1-передача в RS-485 разрешена.Если 0-запрещена.
unsigned char Start_Data_RS=0;//Если 1-передача в RS-232 разрешена.Если 0-запрещена.
//----------------------------------------------------------------------------------------------------------------------------
void Time_out_pirometr (void)
{  
if (Z2!=0)
   {Z2--;}
else {Z2=2000;Start_Data_RS485=1;}
}
void Zaderga_peredahi_RS (void)
{
 if (Z1!=0)
    {Z1--;}
 else {Z1=1000;Start_Data_RS=1;}
}
/*
unsigned int adc_read(void)//Считать данные с АЦП
{
 unsigned int temp;
 LOW(temp) = ADCL; 
 HIGH(temp) = ADCH & 3;
 return temp;
}
void adc_pusk( unsigned char n )//Запуск АЦП
{
 ADMUX = n;    // Номер канала
 ADCSRA = 0xD7;
}
*/
void timer_sys_pusk(void)//Инициализауия таймера/счётчика 0 (2 КГц)
{
 TCCR0 = 0x0c;  // Коэффициент деления 64 , очистка при совпадении
 TCNT0 = 0;     // Fx=Fs/( TCCR0 * ( 1+ OCR0 ) )
 OCR0 = 125;//2КГц //249=1КГц;
 TIMSK |= 1<<OCIE0; // Разрешить прерывания от Timer/Counter 0
}

void usred (void)
{ 
  if (Flag_end==1)
   {
     summa=summa-Massiv_ACP[Index_M];
     Massiv_ACP[Index_M]=adc_read();
     summa=summa+Massiv_ACP[Index_M];
     Rezult_ADC=summa/100;
     Index_M++;
     if (Index_M==100)
        {Index_M=0;}
   } 
   else
   {
   if (Index_M==100)
      {
       Flag_end=1;
       Index_M=0;
      }
   else
      {
      Massiv_ACP[Index_M]=adc_read();
      summa=summa+Massiv_ACP[Index_M];
      Rezult_ADC=summa/Index_M;
      Index_M++;
      }
   }
}










void timer_sys(void)
{
    lcd_int();//Инициализация ЖКИ
    nsSpeed::timer();
    /*
    signal_D1();//Опрос датчика
    // Измерение скорости
    nsSpeed::Skorost();
    */
//adc_pusk(0);// Запуск АЦП (в скобках номер канала=0)
usred ();//Чтение с АЦП  усреднённых данных
//acp_date=adc_read();//Голые данные с АЦП
Time_out_pirometr ();//Задержка перед запросом перометра
Zaderga_peredahi_RS();//Задержка для передачи в RS232 данных
}














// =========================================================
// =     обработка системного Timer 0  частота  2КГц       =
// =========================================================
#pragma vector = TIMER0_COMP_vect
__interrupt void tim0_interrupt(void)
{
  timer_sys();
}


