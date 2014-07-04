
#ifndef Pirometr__h
#define Pirometr__h
// запрос температуры из перометра "Термоскоп-600-1С" (rs485)
void termoskop(void);
// приём температуры "Термоскоп-600-1С" (rs485)
void in_rs485(void);
//------------------------Переменные, флаги, константы для Пирометра---------------------------
extern unsigned int  Pirometr;


#endif

