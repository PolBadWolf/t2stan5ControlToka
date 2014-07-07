
#include "main.h"

//extern unsigned long Korecsia_zero;////Кооэффициент коррекции нуля для АЦП 
//extern unsigned long Smehenie_zero;//Смещение нуля
//extern unsigned int S,Z2,Z1;//длинна окружности
//extern unsigned char Start_Data_RS485;//Если 1-передача в RS-485 разрешена.Если 0-запрещена.
void iniz_port (void)
{
DDRB=0x00;//Порт B на чтение
PORTB=0xFF;//Записать в порт

DDRD = 0xF2; //Порт D настроить как вход  
PORTD= 0xF0; //Записать в порт D  

DDRE=0x00;//Порт E на чтение
PORTE=0xFF;//Записать в порт

DDRF=0x00;//Порт F на чтение// АЦП
PORTF=0x00;//Записать в порт

DDRG=0x00;//Порт G на чтение
PORTG=0xFF;//Записать в порт
}
//-----------------------------------------------------------------------------------------------------------
void main( void )
{  
iniz_port(); // Инициализация портов
timer_sys_pusk(); // Инициализация таймера
InitLcd(); // Инициализация ЖКИ
InitUART();//Инициализация RS232
InitUART2();//Инициализация RS485
LoadSymbol();// Загрузка дополнительных символов в ЖКИ
__enable_interrupt(); // Разрешить глобальное прерывание      
ClearDisplay();// Очистка ЖКИ   
__delay_cycles (3200000);//Задержка 0,2 секунды для накопления фильтром значение входного сигнала с индуктивного датчика
//------------------Востановление данных из флешь памяти------------------------------------------------------- 
// Коэффициент длинны окружности
nsSpeed::init();
/*
((unsigned char *)&S)[0] = ReadEeprom(15);// - 1байт
((unsigned char *)&S)[1] = ReadEeprom(16);// - 2байт
*/
                 //Коэффициент АЦП
nsTok::init();
/*
((unsigned char *)&ACP)[0] = ReadEeprom(19);// - 1байт
((unsigned char *)&ACP)[1] = ReadEeprom(20);// - 2байт
((unsigned char *)&ACP)[2] = ReadEeprom(21);// - 3байт
((unsigned char *)&ACP)[3] = ReadEeprom(22);// - 4байт
*/
          /*Коэффициент калибровки нуля*/
//((unsigned char *)&Korecsia_zero)[0] = ReadEeprom(23);// - 1байт
//((unsigned char *)&Korecsia_zero)[1] = ReadEeprom(24);// - 2байт
//((unsigned char *)&Korecsia_zero)[2] = ReadEeprom(25);// - 3байт
//((unsigned char *)&Korecsia_zero)[3] = ReadEeprom(26);// - 4байт
            /*Смещение нуля*/
//((unsigned char *)&Smehenie_zero)[0] = ReadEeprom(27);// - 1байт
//((unsigned char *)&Smehenie_zero)[1] = ReadEeprom(28);// - 2байт
//((unsigned char *)&Smehenie_zero)[2] = ReadEeprom(29);// - 3байт
//((unsigned char *)&Smehenie_zero)[3] = ReadEeprom(30);// - 4байт

//---------------Конец востановления----------------------------
for (;;)
{    
menu ();
Peredaha_rs232();//Передача данных в RS232
in_rs485 ();//приём температуры c перометра  по RS485 
if (Start_Data_RS485==1)//проверка очередной секунды
  {
    Start_Data_RS485=0;//сброс флага
    termoskop();//Запрос температуры с перометра по RS485
  }

}
}

  
 


