
#ifndef rs485__h
#define rs485__h

void InitUART2(void);
char ReadCom2Byte(unsigned char *Byte);
char WriteCom2Byte(unsigned char data);

#endif

