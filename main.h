
#ifndef main__h
#define main__h
// base cpu
#include <ioavr.h>
#include <ina90.h>
#include <inavr.h>
#define C_Fosc 16000000

#include "lcd.h"
#include "timer.h"
#include "rs232.h"
#include "menu.h"
#include "ks.h"
#include "rs485.h" 
#include "adc.h" 
#include "EEPROM.h"
#include "Peredaha_rs232.h"
// data in
#include "Pirometr.h"
#include "SpeedMetr.h"

#define LOW(x) ((char*)&x)[0]
#define HIGH(x) ((char*)&x)[1]
#define checkbit(var,bit)  (var & (0x01 << (bit)))
#define setbit(var,bit)    (var |= (0x01 << (bit)))
#define clrbit(var,bit)    (var &= (~(0x01 << (bit))))
#define invbit(var,bit)    (var ^= (0x01 << (bit)))

#define delay_mks(us)     __delay_cycles((C_Fosc / 1000000) * (us));

#endif
