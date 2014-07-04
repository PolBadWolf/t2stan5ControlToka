
#ifndef Peredaha_rs232__h
#define Peredaha_rs232__h
void Peredaha_rs232(void);
//-------------------------Переменные и флаги для Задержки передачи данных в RS-----------------------------------------------
extern unsigned int Z1,Z2;
extern unsigned char Start_Data_RS485;  // Если 1-передача в RS-485 разрешена.Если 0-запрещена.
extern unsigned char Start_Data_RS;     // Если 1-передача в RS-232 разрешена.Если 0-запрещена.


#endif

