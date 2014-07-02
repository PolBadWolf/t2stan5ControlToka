
#ifndef rs232__h
#define rs232__h

void InitUART(void);
char ReadComByte(unsigned char *Byte);
char WriteComByte(unsigned char data);
//----------------------------------------------
// выдача в буфер rs с ожиданием
void Data2BufferRs( unsigned char data);
// Вывести строку на rs с заданной позиции
void String2Rs( unsigned char *string);


#endif

