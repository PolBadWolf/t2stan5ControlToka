
#include "lcd.h"
#include "main.h"

//#include "comm_def.h" /* Файл с определениями портов */

#define DELAY_REPEAT       700 /* Задержка в мс перед автоповтором */
#define TIME_REPEAT        10 /* Период автоповтора */
#define DELAY_BEEP         20 /* Длительности в мс звукового сигнала */


//#include "port_def.h" /* Файл с определениями структур и констант */

/* Порт PB */
#define LCDRS      0x20 /* PORT_OUT, выбор регистра данных/управления у ЖК */
#define LCDEN      0x10 /* PORT_OUT, строб для фиксации данных в ЖК индикаторе */
#define LCDEN_ZERO  PORTA &= ~LCDEN /* Подать "0" на вход разрешения  */
#define LCDEN_ONE   PORTA |= LCDEN  /* Подать "1" на вход разрешения */
#define LCD_CONTROL PORTA &= ~LCDRS /* Выбрать регистр упавления */
#define LCD_DATA    PORTA |= LCDRS  /* Выбрать регистр данных */
#define LOAD_LCD    {LCDEN_ONE;_NOP();_NOP();_NOP();_NOP();LCDEN_ZERO;} /* Загрузить данные в LCD индикатор */
#define PORT_LCDKEY  PORTA   /* Порт индикатора LCD */

#define BELL       0x10 /* PORT_OUT, звуковой сигнал */
//#define LED        0x02 /* PORT_OUT, светодиод  */
#define BELL_ON      PORTG &= ~BELL
#define BELL_OFF     PORTG |= BELL
#define BELL_TOGGLE  PORTG ^= BELL

         char display[32]; /* Дисплейный буфер на 32 байта */
unsigned char disp_count;  /* Счетчик указатель индикации */
unsigned char kbd_count;   /* Счетчик указатель скан-кода */
unsigned char time_return;     /* Счетчик таймаута возвращения в рабочий режим */
unsigned char kbd_shift = 0x01;/* Указатель на битовую позицию кнопки */
unsigned char beep_count;      /* Счетчик длительности звукового сигнала */
unsigned int  repeat_count;    /* Счетчик задержек автоповтора */
unsigned int  blink_count=500; /* Регистр счетчика периода мигания */
unsigned int  sec_recount = 1000; /* Пересчетный регистр секунд */
unsigned char sys_flags;       /* Системные флаги состояния системы */
unsigned char dispkbd_flag;    /* Флаги для работы с дисплеем и клавиатурой */
unsigned char scan_code;       /* Регистр скан-кода клавиатуры */
unsigned char keyboard;        /* Регистр флагов признаков клавиатуры */
unsigned int blinc_line1;      /* Регистр флагов мигания 1 строки индикатора */
unsigned int blinc_line2;      /* Регистр флагов мигания 2 строки индикатора */

/* uchar sys_flags  Системные флаги состояния системы */
#define F_START_BEEP     0x01 /* Флаг  */
#define F_SAVE           0x02 /* Флаг требование сохранения изменений */
#define F_RETURN_WORK    0x04 /* Флаг "Возврат в рабочий режим" */
#define F_SECONDS        0x08 /* Флаг "Очередная секунда"  */
#define F_AVERAG         0x10 /* Флаг "Усредненная температура" */
#define F_REGIM          0x20 /* Флаг "Состояние режима" */
#define F_DISPLAY_CLEAR  0x40 /* Флаг "Дисплей очищен" */
#define F_ALARM          0x80 /* Флаг "Тревога" */

#define SET_START_BEEP    sys_flags |= F_START_BEEP  /* Установить флаг начала выдержки звукового сигнала */
#define RESET_START_BEEP  sys_flags &= ~F_START_BEEP /* Сбросить флаг начала выдержки звукового сигнала */
#define START_BEEP       (sys_flags & F_START_BEEP) /* Проверить флаг начала выдержки звукового сигнала */

#define SET_SAVE          sys_flags |= F_SAVE  /* Установить флаг "Сохранить изменения" */
#define RESET_SAVE        sys_flags &= ~F_SAVE /* Сбросить флаг "Сохранить изменения" */
#define SAVE             (sys_flags & F_SAVE) /* Проверить флаг "Сохранить изменения" */

#define SET_RETURN_WORK   sys_flags |= F_RETURN_WORK  /* Установить флаг "Возврат в рабочий режим" */
#define RESET_RETURN_WORK sys_flags &= ~F_RETURN_WORK /* Сбросить флаг "Возврат в рабочий режим" */
#define RETURN_WORK      (sys_flags & F_RETURN_WORK) /* Проверить флаг "Возврат в рабочий режим" */

#define SET_SECONDS       sys_flags |= F_SECONDS  /* Установить флаг "Очередная секунда" */
#define RESET_SECONDS     sys_flags &= ~F_SECONDS /* Сбросить флаг "Очередная секунда" */
#define SECONDS          (sys_flags & F_SECONDS) /* Проверить флаг "Очередная секунда" */

#define SET_AVERAG        sys_flags |= F_AVERAG  /* Установить флаг "Усредненная температура" */
#define RESET_AVERAG      sys_flags &= ~F_AVERAG /* Сбросить флаг "Усредненная температура" */
#define AVERAG           (sys_flags & F_AVERAG) /* Проверка флага "Усредненная температура" */

#define START_REGIM       sys_flags |= F_REGIM  /* Установить флаг "Состояние режима" */
#define STOP_REGIM        sys_flags &= ~F_REGIM /* Сбросить флаг "Состояние режима" */
#define REGIM            (sys_flags & F_REGIM) /* Проверить флаг "Состояние режима" */

#define SET_ALARM         sys_flags |= F_ALARM  /* Установить флаг "Тревога" */
#define RESET_ALARM       sys_flags &= ~F_ALARM /* Сбросить флаг "Тревога" */
#define ALARM            (sys_flags & F_ALARM) /* Проверить флаг "Тревога" */

#define SET_DISPLAY_CLEAR sys_flags |= F_DISPLAY_CLEAR  /* Установить флаг "Дисплей очищен" */
#define RESET_DISPLAY_CLEAR sys_flags &= ~F_DISPLAY_CLEAR /* Сбросить флаг "Дисплей очищен" */
#define DISPLAY_CLEAR    (sys_flags & F_DISPLAY_CLEAR) /* Проверить флаг очистки дисплея */

#define SET_UPD_CLOCK     sys_flags |= F_UPD_CLOCK  /* Установить флаг "Обновить индикацию времени" */
#define RESET_UPD_CLOCK   sys_flags &= ~F_UPD_CLOCK /* Сбросить флаг "Обновить индикацию времени" */
#define UPD_CLOCK        (sys_flags & F_UPD_CLOCK) /* Проверка флага "Обновить индикацию времени" */

/* uchar dispkbd_flag  Системные флаги состояния клавиатуры и индикации */
#define F_TWO_LINE       0x01 /* Флаг перехода на вторую строку индикатора */
#define F_SCAN           0x02 /* Флаг готовности скан-кода */
#define F_SHOW_EN        0x04 /* Флаг разрешения изменений на дисплее */
#define F_BLINK          0x08 /* Флаг мигания с частотой 2 Гц */
#define F_AUTO_REPEAT    0x10 /* Флаг разрешения автоповтора кода кнопки */
#define F_START_DELAY    0x20 /* Флаг старта выдержек автоповтора */
#define F_STOP_DELAY     0x40 /* Флаг завершения выдержек автоповтора */
#define F_KEYB_REPEAT    0x80 /* Флаг признака действия автоповтора */

#define SET_TWO_LINE     dispkbd_flag |= F_TWO_LINE
#define RESET_TWO_LINE   dispkbd_flag &= ~F_TWO_LINE
#define TWO_LINE         (dispkbd_flag & F_TWO_LINE) /* Проверить флаг перехода на вторую строку индикатора */

#define SET_SCAN_READY   dispkbd_flag |= F_SCAN
#define RESET_SCAN_READY dispkbd_flag &= ~F_SCAN
#define SCAN_READY       (dispkbd_flag & F_SCAN) /* Проверить флаг скан-кода */

#define SET_SHOW_EN      dispkbd_flag |= F_SHOW_EN
#define RESET_SHOW_EN    dispkbd_flag &= ~F_SHOW_EN
#define SHOW_EN          (dispkbd_flag & F_SHOW_EN) /* Проверить флаг разрешения смены индикации */

#define TOGGLE_BLINK     dispkbd_flag ^= F_BLINK
#define BLINK            (dispkbd_flag & F_BLINK) /* Проверить флаг мигания */

#define SET_AUTO_REPEAT   dispkbd_flag |= F_AUTO_REPEAT  /* Установить флаг разрешения автоповтора */
#define RESET_AUTO_REPEAT dispkbd_flag &= ~F_AUTO_REPEAT /* Сбросить флаг разрешения автоповтора */
#define AUTO_REPEAT       (dispkbd_flag & F_AUTO_REPEAT) /* Проверить флаг разрешения автоповтора */

#define SET_START_DELAY   dispkbd_flag |= F_START_DELAY  /* Установить флаг начала выдержки */
#define RESET_START_DELAY dispkbd_flag &= ~F_START_DELAY /* Сбросить флаг начала выдержки */
#define START_DELAY       (dispkbd_flag & F_START_DELAY) /* Проверить флаг разрешения начала выдержки */

#define SET_STOP_DELAY    dispkbd_flag |= F_STOP_DELAY  /* Установить флаг окончания выдержки */
#define RESET_STOP_DELAY  dispkbd_flag &= ~F_STOP_DELAY /* Сбросить флаг окончания выдержки */
#define STOP_DELAY        (dispkbd_flag & F_STOP_DELAY) /* Проверить флаг разрешения окончания выдержки */

#define SET_KEYB_REPEAT   dispkbd_flag |= F_KEYB_REPEAT  /* Установить флаг признака действия автоповтора */
#define RESET_KEYB_REPEAT dispkbd_flag &= ~F_KEYB_REPEAT /* Сбросить флаг признака действия автоповтора */
#define KEYB_REPEAT       (dispkbd_flag & F_KEYB_REPEAT) /* Проверить флаг признака действия автоповтора */


/* Таблица перекодировки для индикатора */
flash unsigned char TableDecode[] = {
0xD9,0xDA,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
0xA2,0xB5,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
0x41,0xA0,0x42,0xA1,0xE0,0x45,0xA3,0xA4,0xA5,0xA6,0x4B,0xA7,0x4D,0x48,0x4F,0xA8,
0x50,0x43,0x54,0xA9,0xAA,0x58,0xE1,0xAB,0xAC,0xE2,0xAD,0xAE,0x62,0xAF,0xB0,0xB1,
0x61,0xB2,0xB3,0xB4,0xE3,0x65,0xB6,0xB7,0xB8,0xB9,0xBA,0xBB,0xBC,0xBD,0x6F,0xBE,
0x70,0x63,0xBF,0x79,0xE4,0x78,0xE5,0xC0,0xC1,0xE6,0xC2,0xC3,0xC4,0xC5,0xC6,0xC7};

void set_auto_repeat (void)//Функция включения автоповтора
{
SET_AUTO_REPEAT;
}
void reset_auto_repeat (void)//функция выключения автоповтора
{
 RESET_AUTO_REPEAT; 
}
void ClearDisplay(void) /* Очистить индикатор */
{
 //if(!DISPLAY_CLEAR) /* Если дисплей не был очищен, очистить */
 {
  char *ptr_1 = &display[0];
  for(;ptr_1 <= &display[31];)  *ptr_1++ = 0x20;
  //SET_DISPLAY_CLEAR; /* Установить признак чистого дисплея */
 }
}

/* Вывести строку на индикатор с заданной позиции */
void Show(unsigned char pos, char *t)
{
    char *s;
// if(pos < 32)
// {
  s = (char *)(display + pos);
  while(*t)  *s++ = *t++;
// }
}

/* Вывести символ на индикатор с заданной позиции */
void ShowChar(unsigned char pos, char ch)
{
//if(pos < 32)
 display[pos] = ch;
}

/* Показать значение температуры в десятичном виде 00.0 в конкретной позиции */
void ShowTemperature(unsigned char pos, unsigned int digit)
{
    char *s;
 //unsigned int i;
 if((pos > 31) || ((pos + 4) > 32))
  return;
 if(digit == 0)
  {
   s = display + pos; /* Указатель на знакоместо в буфере */
   *s++ = ' '; /*  */
   *s++ = '0'; /*  */
   *s++ = '.'; /*  */
   *s = '0';
   return;
  }
 s = display + pos + 4; /* Указатель на знакоместо младшего разряда */
 *--s = digit % 10 + '0';
 *--s = '.';
 if(digit/10 > 0) *--s = (digit/10)%10 + '0';
 else *--s = '0'; /* Ведущие пробелы */
 if(digit/100 > 0) *--s = (digit/100)%10 + '0';
 else *--s = ' '; /* Ведущие пробелы */
}

/* Показать число с ведущими пробелами в десятичном виде в конкретной позиции с определенным количеством цифр */
void ShowDigit(unsigned char pos, unsigned char numdigit, unsigned int digit)
{
    char *s;
 unsigned int i;
if(digit == 0)
 {
  s = display + pos; /* Указатель на знакоместо в буфере */
  for(;numdigit >1; numdigit--)
   *s++ = ' '; /* Ведущие пробелы */
  *s = '0';
  return;
 }
s = display + pos + numdigit; /* Указатель на знакоместо младшего разряда */
*--s = digit % 10 + '0';
for(i = 10;numdigit >1;i*=10, numdigit--)
 if(digit/i > 0) *--s = (digit/i)%10 + '0';
 else *--s = ' '; /* Ведущие пробелы */
}

/* Показать число с ведущими нулями в десятичном виде в конкретной позиции с определенным количеством цифр */
void ShowDigitZ(unsigned char pos, unsigned char numdigit, unsigned int digit)
{
    char *s;
 unsigned int i;
if(digit == 0)
 {
  s = display + pos; /* Указатель на знакоместо в буфере */
  for(;numdigit >1; numdigit--)
   *s++ = '0'; /* Ведущие нули */
  *s = '0';
  return;
 }
s = display + pos + numdigit; /* Указатель на знакоместо младшего разряда */
*--s = digit % 10 + '0';
for(i = 10;numdigit >1;i*=10, numdigit--)
 if(digit/i > 0) *--s = (digit/i)%10 + '0';
 else *--s = '0'; /* Ведущие нули */
}

// Загрузить данные в LCD индикатор
void LoadLcd(void)
{
 LCDEN_ONE;
 _NOP();
 _NOP();
 _NOP();
 _NOP();
 LCDEN_ZERO;
}

void Delay4mks(void)
{
 unsigned char i;
 for(i = 17;i;i--);
}


void InitLcd(void)     /* Инициализация индикатора */
{
DDRA |= 0X3f;
delay_mks(30000); //16000
LCD_CONTROL;           /* Выбрать регистр управления */
PORT_LCDKEY = 0x03;    /* Записать данные в регистр индикатора/клавиатуры */
LoadLcd();             /* Загрузить данные в LCD индикатор */
delay_mks(4600);//4100
PORT_LCDKEY = 0x03;    /* Записать данные в регистр индикатора/клавиатуры */
LoadLcd();             /* Загрузить данные в LCD индикатор */
delay_mks(4600);//100
PORT_LCDKEY = 0x03;    /* Записать данные в регистр индикатора/клавиатуры */
LoadLcd();             /* Загрузить данные в LCD индикатор */
delay_mks(4600);//4100
PORT_LCDKEY = 0x02;    /* Записать данные в регистр индикатора/клавиатуры */
LoadLcd();             /* Загрузить данные в LCD индикатор */
delay_mks(4600);//60
                       /* 4-bit operation, 1/16 duty cycle, font 5x8 dot matrix */
PORT_LCDKEY = 0x02;    /* Записать данные в регистр индикатора/клавиатуры */
LoadLcd();             /* Загрузить данные в LCD индикатор */
PORT_LCDKEY = 0x08;    /* Записать данные в регистр индикатора/клавиатуры */
LoadLcd();             /* Загрузить данные в LCD индикатор */
delay_mks(4600);//60
                       /* Display off, cursor off, blink off */
PORT_LCDKEY = 0x00;    /* Записать данные в регистр индикатора/клавиатуры */
LoadLcd();             /* Загрузить данные в LCD индикатор */
PORT_LCDKEY = 0x08;    /* Записать данные в регистр индикатора/клавиатуры */
LoadLcd();             /* Загрузить данные в LCD индикатор */
delay_mks(4600);//60
                       /* Display on */
PORT_LCDKEY = 0x00;    /* Записать данные в регистр индикатора/клавиатуры */
LoadLcd();             /* Загрузить данные в LCD индикатор */
PORT_LCDKEY = 0x0C;    /* Записать данные в регистр индикатора/клавиатуры */
LoadLcd();             /* Загрузить данные в LCD индикатор */
delay_mks(240);//60

PORT_LCDKEY = 0x00;    /* Записать данные в регистр индикатора/клавиатуры */
LoadLcd();             /* Загрузить данные в LCD индикатор */
PORT_LCDKEY = 0x01;    /* Записать данные в регистр индикатора/клавиатуры */
LoadLcd();             /* Загрузить данные в LCD индикатор */
delay_mks(240);//60

                       /* Set entry mode, cursor moving right */
PORT_LCDKEY = 0x00;    /* Записать данные в регистр индикатора/клавиатуры */
LoadLcd();             /* Загрузить данные в LCD индикатор */
PORT_LCDKEY = 0x06;    /* Записать данные в регистр индикатора/клавиатуры */
LoadLcd();             /* Загрузить данные в LCD индикатор */
delay_mks(4600);//60
RESET_TWO_LINE;
RESET_SCAN_READY;
}
#define ButtonGis 100
unsigned char v_ButtonMask[4]={1,2,4,8};
unsigned char v_ButtonCount[4]={ButtonGis,ButtonGis,ButtonGis,ButtonGis};
unsigned char v_key = 0x0f;
void f_ButtonFiltr(void)
{
  unsigned char pp;
  pp = (PING & 0x0F);
  for(unsigned char i=0;i<4;i++)
  {
    if ( pp & v_ButtonMask[i] )
    {
      if (v_ButtonCount[i]<ButtonGis) v_ButtonCount[i]++;
      if ( (v_ButtonCount[i]>=ButtonGis) && ((v_key&v_ButtonMask[i])==0) ) v_key |= v_ButtonMask[i];
    }
    else
    {
      if ( v_ButtonCount[i]>0 ) v_ButtonCount[i]--;
      if ( (v_ButtonCount[i]==0) && (v_key&v_ButtonMask[i]) ) v_key &= ~v_ButtonMask[i];
    }
  }
}
//TIMER0_COMP_vect,TIMER0_OVF_vect
//#pragma vector = TIMER0_COMP_vect /* Прерывание каждую миллисекунду */
//__interrupt void RefreshLcdKbd(void) /* Опрос клавиатуры и регенерация дисплея */
void lcd_int(void)
{
 unsigned char disp_temp; /* Временный рабочий регистр */
 unsigned char kbd;
// TCNT0 = 0xAF; /* Перезагрузить таймер */
  f_ButtonFiltr();
 if(!--sec_recount)
 {
  SET_SECONDS; /* Установить флаг "Очередная секунда" */
  sec_recount = 1000;
//  sec_recount = 200;
 }
 if(!--blink_count)
 {
  TOGGLE_BLINK;
  blink_count = 500;
//  if(ALARM) /* Проверить флаг "Тревога" */
//   BELL_TOGGLE;
 }
 if(START_DELAY) /* Проверить флаг разрешения начала выдержки */
 {
  if(!--repeat_count) /* Если обратный отсчет закончен */
  {
   SET_STOP_DELAY; /* Установить флаг окончания выдержки */
   RESET_START_DELAY; /* Сбросить флаг начала выдержки */
  }
 }
 if(START_BEEP) /* Проверить флаг начала выдержки звукового сигнала */
 {
  BELL_TOGGLE;
  if(!--beep_count) /* Если обратный отсчет закончен */
  {
   BELL_OFF; /* Выключить звуковой сигнал */
   RESET_START_BEEP; /* Сбросить флаг начала выдержки звукового сигнала */
  }
 }
//KKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKK
 // kbd = (PING & 0x0F) ; // 
// kbd >>= 4;
 kbd = v_key;

if(!(kbd & kbd_shift)) /* Считать состояние клавиатуры по текущему биту порта PA */
{                       /* Текущая кнопка нажата */
 if(!(keyboard & kbd_shift)) /* Если кнопка не была удержана */
  {
   if(!SCAN_READY) /* Если предыдущий скан-код обработан */
   {
    scan_code = kbd_count; /* Взять скан-код кнопки */
    SET_SCAN_READY; /* Установить флаг готовности скан-кода */
   }
   BELL_ON; /* Звуковой сигнал нажатия кнопки */
   SET_START_BEEP; /* Установить флаг начала выдержки звукового сигнала */
   beep_count = DELAY_BEEP;  /* Длительности в мс звукового сигнала */
   SET_START_DELAY; /* Установить флаг начала выдержки перед автоповтором */
   repeat_count = DELAY_REPEAT; /* Выдержка в мс перед автоповтором */
  }
 else /* Если кнопка была удержана */
  {
   if(STOP_DELAY) /* Проверить окончание выдержки перед автоповтором */
   {
    SET_KEYB_REPEAT; /* Установить флаг признака действия автоповтора */
    if(!SCAN_READY && AUTO_REPEAT) /* Если предыдущий скан-код обработан и разрешен автоповтор */
    {
     scan_code = kbd_count; /* Взять скан-код кнопки */
     SET_SCAN_READY; /* Установить флаг готовности скан-кода */
    }
    RESET_STOP_DELAY; /* Сбросить флаг окончания выдержки */
    SET_START_DELAY; /* Установить флаг начала выдержки периода автоповтора */
    repeat_count = TIME_REPEAT; /* Период автоповтора */
   }
  }
 keyboard |= kbd_shift; /* Установить соответствующий бит нажатой кнопки */
}
else /* Текущая кнопка отжата */
{
 if(keyboard & kbd_shift) /* Если кнопка была удержана и отпущена */
 {
  RESET_START_DELAY; /* Сбросить флаг начала выдержки */
  RESET_STOP_DELAY; /* Сбросить флаг окончания выдержки */
  RESET_KEYB_REPEAT; /* Сбросить флаг признака действия автоповтора */
 }
 keyboard &= ~kbd_shift; /* Сбросить соответствующий бит отжатой кнопки */
}

 kbd_count++;
 kbd_shift <<= 1;    /* Сдвинуть на следующую позицию */

 if(kbd_count == 4) /* Проверить счетчик кнопок на последний разряд */
 {
  kbd_count = 0;    /* Счетчик указатель скан-кода */
  kbd_shift = 0x01; /* Указатель на битовую позицию кнопки */
 }


if(disp_count == 32) /* Проверить счетчик на последний разряд */
 {
  disp_count = 0;       /* Сбросить счетчик */
  LCD_CONTROL;          /* Выбрать регистр управления */
  PORT_LCDKEY &= 0xF0;  /* Обнулить младшую тетраду */
  PORT_LCDKEY |= 0x08;  /* Установить адрес первого символа первой строки */
  LOAD_LCD;             /* Загрузить старшую тетраду в индикатор */
  PORT_LCDKEY &= 0xF0;  /* Обнулить младшую тетраду */
  LOAD_LCD;             /* Загрузить младшую тетраду в индикатор */
  RESET_TWO_LINE;       /* Сбросить флаг второй строки */
  return;
 }
if(disp_count == 16 && !TWO_LINE)
 {
  LCD_CONTROL;          /* Выбрать регистр управления */
  PORT_LCDKEY &= 0xF0;  /* Обнулить младшую тетраду */
  PORT_LCDKEY |= 0x0C;  /* Установить адрес первого символа второй строки */
  LOAD_LCD;             /* Загрузить старшую тетраду в индикатор */
  PORT_LCDKEY &= 0xF0;  /* Обнулить младшую тетраду */
  LOAD_LCD;             /* Загрузить младшую тетраду в индикатор */
  SET_TWO_LINE;         /* Установить флаг перехода на вторую строку */
  return;
 }
LCD_DATA; /* Выбрать регистр данных */
disp_temp = display[disp_count];
if(disp_temp>0x7f)
  disp_temp = TableDecode[display[disp_count] - 0x80];

if(TWO_LINE)   /* Определить выводимую строку */
 {             /* Мигание на 2 строке */
 if(checkbit(blinc_line2,disp_count-16) && BLINK) /* Проверить разряд на мигание */
  disp_temp = 0x20; /* Код пробела */
 }
else
 {             /* Мигание на 1 строке */
 if(checkbit(blinc_line1,disp_count) && BLINK) /* Проверить разряд на мигание */
  disp_temp = 0x20; /* Код пробела */
 }
PORT_LCDKEY &= 0xF0; /* Обнулить младшую тетраду */
PORT_LCDKEY |= disp_temp >> 4; /* Записать старшую тетраду в регистр индикатора/клавиатуры */
LOAD_LCD;              /* Загрузить данные в LCD индикатор */
PORT_LCDKEY &= 0xF0; /* Обнулить младшую тетраду */
PORT_LCDKEY |= (disp_temp & 0x0F); /* Записать младшую тетраду в регистр индикатора/клавиатуры */
LOAD_LCD;              /* Загрузить данные в LCD индикатор */
disp_count ++;
}

/* Указатель на изменяемый объект, размер объекта, минимальное значение, максимальное значение, признак перехода через максимальное значение */
void ParametrUp(void *ptr,unsigned int size,unsigned int min_param,unsigned int max_param,unsigned char over_max)
{
if(size == sizeof(unsigned int)) /* Если параметр типа unsigned int */
 {
  unsigned int *param = (unsigned int *)ptr;
  if(*param < max_param)
  {  // Если флаг признака действия автоповтора установлен, то увеличить параметр на 10 иначе на 1
   if(KEYB_REPEAT)
   {
    if(*param > 0xFFF5)
    {
     if(over_max) *param = min_param;
     else         *param = max_param;
    }
    else
    {
     *param+=10;
     if(*param > max_param)
     {
      if(over_max) *param = min_param;
      else         *param = max_param;
     }
    }
   }
   else (*param)++;
  }
  else
  {
   if(over_max) *param = min_param;
   else         *param = max_param;
  }
 }
else /* Иначе параметр типа unsigned char */
 {
  unsigned char *param = (unsigned char *)ptr;
  if(*param < max_param)
  {  // Если флаг признака действия автоповтора установлен, то увеличить параметр на 10 иначе на 1
   if(KEYB_REPEAT)
   {
    if(*param > 0xF5)
    {
     if(over_max) *param = min_param;
     else         *param = max_param;
    }
    else
    {
     *param+=10;
     if(*param > max_param)
     {
      if(over_max) *param = min_param;
      else         *param = max_param;
     }
    }
   }
   else (*param)++;
  }
  else
  {
   if(over_max) *param = min_param;
   else         *param = max_param;
  }
 }
SET_SAVE; /* Установить флаг "Сохранить изменения" */
}

/* Указатель на изменяемый объект, размер объекта, минимальное значение, максимальное значение, признак перехода через минимальное значение */
void ParametrDown(void *ptr,unsigned int size,unsigned int min_param,unsigned int max_param,unsigned char over_min)
{
if(size == sizeof(unsigned int)) /* Если параметр типа unsigned int */
 {
  unsigned int *param = (unsigned int *)ptr;
  if(*param > min_param)
  { // Если флаг признака действия автоповтора установлен, то уменьшить параметр на 10 иначе на 1
   if(KEYB_REPEAT)
   {
    if(*param < 10)
    {
     if(over_min) *param = max_param;
     else         *param = min_param;
    }
    else
    {
     *param-=10;
     if(*param < min_param)
     {
      if(over_min) *param = max_param;
      else         *param = min_param;
     }
    }
   }
   else (*param)--;
  }
  else
  {
   if(over_min) *param = max_param;
   else         *param = min_param;
  }
 }
else /* Иначе параметр типа unsigned char */
 {
  unsigned char *param = (unsigned char *)ptr;
  if(*param > min_param)
  { // Если флаг признака действия автоповтора установлен, то уменьшить параметр на 10 иначе на 1
   if(KEYB_REPEAT)
   {
    if(*param < 10)
    {
     if(over_min) *param = max_param;
     else         *param = min_param;
    }
    else
    {
     *param-=10;
     if(*param < min_param)
     {
      if(over_min) *param = max_param;
      else         *param = min_param;
     }
    }
   }
   else (*param)--;
  }
  else
  {
   if(over_min) *param = max_param;
   else         *param = min_param;
  }
 }
SET_SAVE; /* Установить флаг "Сохранить изменения" */
}

unsigned char scan_ready(void)
{
  return SCAN_READY;
}

unsigned char read_key(void)
{
  RESET_SCAN_READY;
  return 4-scan_code;
}
