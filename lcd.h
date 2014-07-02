
#ifndef lcd__h
#define lcd__h

// CORE
void lcd_int(void);
void InitLcd(void);     /* Инициализация индикатора */
void LoadSymbol(void); /* Загрузить в индикатор дополнительные символы */
// Загрузить данные в LCD индикатор
void LoadLcd(void);
extern unsigned int blinc_line1;      /* Регистр флагов мигания 1 строки индикатора */
extern unsigned int blinc_line2;      /* Регистр флагов мигания 2 строки индикатора */
// User
void Show(unsigned char pos, unsigned char *t);
void ClearDisplay(void); /* Очистить индикатор */
void ShowChar(unsigned char pos, unsigned char ch);
unsigned char scan_ready(void);
unsigned char read_key(void);
// Показать число с ведущими нулями в десятичном виде в конкретной позиции с определенным количеством цифр
void ShowDigitZ(unsigned char pos, unsigned char numdigit, unsigned int digit);
void ShowDigitZLONG(unsigned char pos, unsigned char numdigit, unsigned long digit);
void set_auto_repeat (void);//Функция включения автоповтора
void reset_auto_repeat (void);//функция выключения автоповтора

/*
unsigned char ReadEeprom(unsigned int addr); // Прочитать байт из EEPROM
void WriteEeprom(unsigned int addr,  unsigned char data); // Записать байт в EEPROM
void String2Rs( unsigned char *string);// Вывести строку на rs с заданной позиции
void InitUART(void);//Инициализация UART
void InitUART2(void);//инициализация RS485
char WriteCom2Byte(unsigned char data);//запись данных в RS485
char ReadCom2Byte(unsigned char *Byte);//чтение данных с RS485
char WriteComByte(unsigned char data);//Передача байта в rs
//void Data2BufferRs( unsigned char data);// выдача в буфер rs с ожиданием
void Peredaha_rs232 (void);//Выдача данных в RS232
void in_rs485(void);//приём температуры с перометра по RS485
*/
#endif

