
#include "main.h"

//---------------------------------Переменные, флаги, константы--------------------------------------------------------------
unsigned long Massiv_ACP [100];//Массив АЦП
unsigned char Index_M=0;//Индекс массива
unsigned char Flag_end=0;//Флаг регистрации заполнения массива
unsigned long summa=0;//Переменная для хранения суммированых результатов с АЦП
unsigned long Rezult_ADC=0;//Переменная для вывода значения с АЦП на индикатор
unsigned long acp_date=0;//Голые данные с ацп
//---------------Переменные для фильтрации входного сигнала с индуктивного датчика-------------------------------------------
unsigned char D1=0;//Переменная для входного сигнала с индуктивного датчика
unsigned int Chethik_D1=0;//Счётчик для устранения дребезга
unsigned char D_1=0;//Отфильтрованное состояние датчика. Если 0-датчик вкл. Если 1-датчик выкл.
                /*(при частоте прерывания 2КГц время фильтрации 10 мил.сек)*/
//-------------------------------Переменные, флаги для скоростимера--------------------------------------------------------------------------------------
unsigned int Buf_impuls=0;//Буфер для просмотра данных с счётчика импульсов
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
void signal_D1 (void)
{
D1=PINE & 0x4;//2-ой бит порт E.  
if (D1==0)
{
if (Chethik_D1==0)
 {
  D_1=0;
 }
else 
 {
  Chethik_D1--;
 }
}
else
{
 if (Chethik_D1>=20) 
 {
  D_1=1;
 }
 else
 {
  Chethik_D1++;
 }
}
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
void Skorost (void)//Измерение линейной скорости стана
{
flag_new=D_1;//прочитать новое значение датчика 
if (flag_reset_shet==1)
{
 shethik_impulsow++;//инкремент счётчика 
}
if (shethik_impulsow>=3000)//проверка счётчика на переполнение
{
 flag_reset_shet=0;//Счёт запрещён
 shethik_impulsow=0;//бнуление счётчика
 Buf_impuls=0;//обнуление буфера данных, полученных со счётчика импульсов, для просмотра
 rezultat_skorosti=0;//обнуление 
}
if ((flag_old!=flag_new)&&(flag_new!=0))//если старое значение датчика не равно новому и новое не равно нулю
{
flag_reset_shet=1;//разрешон
rezultat_skorosti=(120000/1)/(shethik_impulsow);//Расчитать скорость об/мин (1-кол.секторов)            
Buf_impuls=shethik_impulsow;//передача данных со счётчика в буфер данных для просмотра
shethik_impulsow=0;//бнуление счётчика 
}                                       
flag_old=flag_new;//новое значение датчика присвоить старому
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
signal_D1();//Опрос датчика
Skorost ();//Измерение скорости
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


