
#ifndef ks__h
#define ks__h

// подсчет кс в буфере
unsigned char crc8_buf( unsigned char *buf, int len );
// расщет одного байта
unsigned char crc8_byte( unsigned char old_crc, unsigned char data );

#endif

