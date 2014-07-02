
#ifndef EEPROM__h
#define EEPROM__h

// Прочитать байт из EEPROM
unsigned char ReadEeprom(unsigned int addr);
void WriteEeprom(unsigned  int addr,  unsigned char data);


#endif

