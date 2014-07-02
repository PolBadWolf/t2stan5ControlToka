
#include "lcd.h"
#include "main.h"

//#include "comm_def.h" /* ���� � ������������� ������ */

#define DELAY_REPEAT       700 /* �������� � �� ����� ������������ */
#define TIME_REPEAT        10 /* ������ ����������� */
#define DELAY_BEEP         20 /* ������������ � �� ��������� ������� */


//#include "port_def.h" /* ���� � ������������� �������� � �������� */

/* ���� PB */
#define LCDRS      0x20 /* PORT_OUT, ����� �������� ������/���������� � �� */
#define LCDEN      0x10 /* PORT_OUT, ����� ��� �������� ������ � �� ���������� */
#define LCDEN_ZERO  PORTA &= ~LCDEN /* ������ "0" �� ���� ����������  */
#define LCDEN_ONE   PORTA |= LCDEN  /* ������ "1" �� ���� ���������� */
#define LCD_CONTROL PORTA &= ~LCDRS /* ������� ������� ��������� */
#define LCD_DATA    PORTA |= LCDRS  /* ������� ������� ������ */
#define LOAD_LCD    {LCDEN_ONE;_NOP();_NOP();_NOP();_NOP();LCDEN_ZERO;} /* ��������� ������ � LCD ��������� */
#define PORT_LCDKEY  PORTA   /* ���� ���������� LCD */

#define BELL       0x10 /* PORT_OUT, �������� ������ */
//#define LED        0x02 /* PORT_OUT, ���������  */
#define BELL_ON      PORTG &= ~BELL
#define BELL_OFF     PORTG |= BELL
#define BELL_TOGGLE  PORTG ^= BELL

         char display[32]; /* ���������� ����� �� 32 ����� */
unsigned char disp_count;  /* ������� ��������� ��������� */
unsigned char kbd_count;   /* ������� ��������� ����-���� */
unsigned char time_return;     /* ������� �������� ����������� � ������� ����� */
unsigned char kbd_shift = 0x01;/* ��������� �� ������� ������� ������ */
unsigned char beep_count;      /* ������� ������������ ��������� ������� */
unsigned int  repeat_count;    /* ������� �������� ����������� */
unsigned int  blink_count=500; /* ������� �������� ������� ������� */
unsigned int  sec_recount = 1000; /* ����������� ������� ������ */
unsigned char sys_flags;       /* ��������� ����� ��������� ������� */
unsigned char dispkbd_flag;    /* ����� ��� ������ � �������� � ����������� */
unsigned char scan_code;       /* ������� ����-���� ���������� */
unsigned char keyboard;        /* ������� ������ ��������� ���������� */
unsigned int blinc_line1;      /* ������� ������ ������� 1 ������ ���������� */
unsigned int blinc_line2;      /* ������� ������ ������� 2 ������ ���������� */

/* uchar sys_flags  ��������� ����� ��������� ������� */
#define F_START_BEEP     0x01 /* ����  */
#define F_SAVE           0x02 /* ���� ���������� ���������� ��������� */
#define F_RETURN_WORK    0x04 /* ���� "������� � ������� �����" */
#define F_SECONDS        0x08 /* ���� "��������� �������"  */
#define F_AVERAG         0x10 /* ���� "����������� �����������" */
#define F_REGIM          0x20 /* ���� "��������� ������" */
#define F_DISPLAY_CLEAR  0x40 /* ���� "������� ������" */
#define F_ALARM          0x80 /* ���� "�������" */

#define SET_START_BEEP    sys_flags |= F_START_BEEP  /* ���������� ���� ������ �������� ��������� ������� */
#define RESET_START_BEEP  sys_flags &= ~F_START_BEEP /* �������� ���� ������ �������� ��������� ������� */
#define START_BEEP       (sys_flags & F_START_BEEP) /* ��������� ���� ������ �������� ��������� ������� */

#define SET_SAVE          sys_flags |= F_SAVE  /* ���������� ���� "��������� ���������" */
#define RESET_SAVE        sys_flags &= ~F_SAVE /* �������� ���� "��������� ���������" */
#define SAVE             (sys_flags & F_SAVE) /* ��������� ���� "��������� ���������" */

#define SET_RETURN_WORK   sys_flags |= F_RETURN_WORK  /* ���������� ���� "������� � ������� �����" */
#define RESET_RETURN_WORK sys_flags &= ~F_RETURN_WORK /* �������� ���� "������� � ������� �����" */
#define RETURN_WORK      (sys_flags & F_RETURN_WORK) /* ��������� ���� "������� � ������� �����" */

#define SET_SECONDS       sys_flags |= F_SECONDS  /* ���������� ���� "��������� �������" */
#define RESET_SECONDS     sys_flags &= ~F_SECONDS /* �������� ���� "��������� �������" */
#define SECONDS          (sys_flags & F_SECONDS) /* ��������� ���� "��������� �������" */

#define SET_AVERAG        sys_flags |= F_AVERAG  /* ���������� ���� "����������� �����������" */
#define RESET_AVERAG      sys_flags &= ~F_AVERAG /* �������� ���� "����������� �����������" */
#define AVERAG           (sys_flags & F_AVERAG) /* �������� ����� "����������� �����������" */

#define START_REGIM       sys_flags |= F_REGIM  /* ���������� ���� "��������� ������" */
#define STOP_REGIM        sys_flags &= ~F_REGIM /* �������� ���� "��������� ������" */
#define REGIM            (sys_flags & F_REGIM) /* ��������� ���� "��������� ������" */

#define SET_ALARM         sys_flags |= F_ALARM  /* ���������� ���� "�������" */
#define RESET_ALARM       sys_flags &= ~F_ALARM /* �������� ���� "�������" */
#define ALARM            (sys_flags & F_ALARM) /* ��������� ���� "�������" */

#define SET_DISPLAY_CLEAR sys_flags |= F_DISPLAY_CLEAR  /* ���������� ���� "������� ������" */
#define RESET_DISPLAY_CLEAR sys_flags &= ~F_DISPLAY_CLEAR /* �������� ���� "������� ������" */
#define DISPLAY_CLEAR    (sys_flags & F_DISPLAY_CLEAR) /* ��������� ���� ������� ������� */

#define SET_UPD_CLOCK     sys_flags |= F_UPD_CLOCK  /* ���������� ���� "�������� ��������� �������" */
#define RESET_UPD_CLOCK   sys_flags &= ~F_UPD_CLOCK /* �������� ���� "�������� ��������� �������" */
#define UPD_CLOCK        (sys_flags & F_UPD_CLOCK) /* �������� ����� "�������� ��������� �������" */

/* uchar dispkbd_flag  ��������� ����� ��������� ���������� � ��������� */
#define F_TWO_LINE       0x01 /* ���� �������� �� ������ ������ ���������� */
#define F_SCAN           0x02 /* ���� ���������� ����-���� */
#define F_SHOW_EN        0x04 /* ���� ���������� ��������� �� ������� */
#define F_BLINK          0x08 /* ���� ������� � �������� 2 �� */
#define F_AUTO_REPEAT    0x10 /* ���� ���������� ����������� ���� ������ */
#define F_START_DELAY    0x20 /* ���� ������ �������� ����������� */
#define F_STOP_DELAY     0x40 /* ���� ���������� �������� ����������� */
#define F_KEYB_REPEAT    0x80 /* ���� �������� �������� ����������� */

#define SET_TWO_LINE     dispkbd_flag |= F_TWO_LINE
#define RESET_TWO_LINE   dispkbd_flag &= ~F_TWO_LINE
#define TWO_LINE         (dispkbd_flag & F_TWO_LINE) /* ��������� ���� �������� �� ������ ������ ���������� */

#define SET_SCAN_READY   dispkbd_flag |= F_SCAN
#define RESET_SCAN_READY dispkbd_flag &= ~F_SCAN
#define SCAN_READY       (dispkbd_flag & F_SCAN) /* ��������� ���� ����-���� */

#define SET_SHOW_EN      dispkbd_flag |= F_SHOW_EN
#define RESET_SHOW_EN    dispkbd_flag &= ~F_SHOW_EN
#define SHOW_EN          (dispkbd_flag & F_SHOW_EN) /* ��������� ���� ���������� ����� ��������� */

#define TOGGLE_BLINK     dispkbd_flag ^= F_BLINK
#define BLINK            (dispkbd_flag & F_BLINK) /* ��������� ���� ������� */

#define SET_AUTO_REPEAT   dispkbd_flag |= F_AUTO_REPEAT  /* ���������� ���� ���������� ����������� */
#define RESET_AUTO_REPEAT dispkbd_flag &= ~F_AUTO_REPEAT /* �������� ���� ���������� ����������� */
#define AUTO_REPEAT       (dispkbd_flag & F_AUTO_REPEAT) /* ��������� ���� ���������� ����������� */

#define SET_START_DELAY   dispkbd_flag |= F_START_DELAY  /* ���������� ���� ������ �������� */
#define RESET_START_DELAY dispkbd_flag &= ~F_START_DELAY /* �������� ���� ������ �������� */
#define START_DELAY       (dispkbd_flag & F_START_DELAY) /* ��������� ���� ���������� ������ �������� */

#define SET_STOP_DELAY    dispkbd_flag |= F_STOP_DELAY  /* ���������� ���� ��������� �������� */
#define RESET_STOP_DELAY  dispkbd_flag &= ~F_STOP_DELAY /* �������� ���� ��������� �������� */
#define STOP_DELAY        (dispkbd_flag & F_STOP_DELAY) /* ��������� ���� ���������� ��������� �������� */

#define SET_KEYB_REPEAT   dispkbd_flag |= F_KEYB_REPEAT  /* ���������� ���� �������� �������� ����������� */
#define RESET_KEYB_REPEAT dispkbd_flag &= ~F_KEYB_REPEAT /* �������� ���� �������� �������� ����������� */
#define KEYB_REPEAT       (dispkbd_flag & F_KEYB_REPEAT) /* ��������� ���� �������� �������� ����������� */


/* ������� ������������� ��� ���������� */
flash unsigned char TableDecode[] = {
0xD9,0xDA,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
0xA2,0xB5,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
0x41,0xA0,0x42,0xA1,0xE0,0x45,0xA3,0xA4,0xA5,0xA6,0x4B,0xA7,0x4D,0x48,0x4F,0xA8,
0x50,0x43,0x54,0xA9,0xAA,0x58,0xE1,0xAB,0xAC,0xE2,0xAD,0xAE,0x62,0xAF,0xB0,0xB1,
0x61,0xB2,0xB3,0xB4,0xE3,0x65,0xB6,0xB7,0xB8,0xB9,0xBA,0xBB,0xBC,0xBD,0x6F,0xBE,
0x70,0x63,0xBF,0x79,0xE4,0x78,0xE5,0xC0,0xC1,0xE6,0xC2,0xC3,0xC4,0xC5,0xC6,0xC7};

void set_auto_repeat (void)//������� ��������� �����������
{
SET_AUTO_REPEAT;
}
void reset_auto_repeat (void)//������� ���������� �����������
{
 RESET_AUTO_REPEAT; 
}
void ClearDisplay(void) /* �������� ��������� */
{
 //if(!DISPLAY_CLEAR) /* ���� ������� �� ��� ������, �������� */
 {
  char *ptr_1 = &display[0];
  for(;ptr_1 <= &display[31];)  *ptr_1++ = 0x20;
  //SET_DISPLAY_CLEAR; /* ���������� ������� ������� ������� */
 }
}

/* ������� ������ �� ��������� � �������� ������� */
void Show(unsigned char pos, char *t)
{
    char *s;
// if(pos < 32)
// {
  s = (char *)(display + pos);
  while(*t)  *s++ = *t++;
// }
}

/* ������� ������ �� ��������� � �������� ������� */
void ShowChar(unsigned char pos, char ch)
{
//if(pos < 32)
 display[pos] = ch;
}

/* �������� �������� ����������� � ���������� ���� 00.0 � ���������� ������� */
void ShowTemperature(unsigned char pos, unsigned int digit)
{
    char *s;
 //unsigned int i;
 if((pos > 31) || ((pos + 4) > 32))
  return;
 if(digit == 0)
  {
   s = display + pos; /* ��������� �� ���������� � ������ */
   *s++ = ' '; /*  */
   *s++ = '0'; /*  */
   *s++ = '.'; /*  */
   *s = '0';
   return;
  }
 s = display + pos + 4; /* ��������� �� ���������� �������� ������� */
 *--s = digit % 10 + '0';
 *--s = '.';
 if(digit/10 > 0) *--s = (digit/10)%10 + '0';
 else *--s = '0'; /* ������� ������� */
 if(digit/100 > 0) *--s = (digit/100)%10 + '0';
 else *--s = ' '; /* ������� ������� */
}

/* �������� ����� � �������� ��������� � ���������� ���� � ���������� ������� � ������������ ����������� ���� */
void ShowDigit(unsigned char pos, unsigned char numdigit, unsigned int digit)
{
    char *s;
 unsigned int i;
if(digit == 0)
 {
  s = display + pos; /* ��������� �� ���������� � ������ */
  for(;numdigit >1; numdigit--)
   *s++ = ' '; /* ������� ������� */
  *s = '0';
  return;
 }
s = display + pos + numdigit; /* ��������� �� ���������� �������� ������� */
*--s = digit % 10 + '0';
for(i = 10;numdigit >1;i*=10, numdigit--)
 if(digit/i > 0) *--s = (digit/i)%10 + '0';
 else *--s = ' '; /* ������� ������� */
}

/* �������� ����� � �������� ������ � ���������� ���� � ���������� ������� � ������������ ����������� ���� */
void ShowDigitZ(unsigned char pos, unsigned char numdigit, unsigned int digit)
{
    char *s;
 unsigned int i;
if(digit == 0)
 {
  s = display + pos; /* ��������� �� ���������� � ������ */
  for(;numdigit >1; numdigit--)
   *s++ = '0'; /* ������� ���� */
  *s = '0';
  return;
 }
s = display + pos + numdigit; /* ��������� �� ���������� �������� ������� */
*--s = digit % 10 + '0';
for(i = 10;numdigit >1;i*=10, numdigit--)
 if(digit/i > 0) *--s = (digit/i)%10 + '0';
 else *--s = '0'; /* ������� ���� */
}

// ��������� ������ � LCD ���������
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


void InitLcd(void)     /* ������������� ���������� */
{
DDRA |= 0X3f;
delay_mks(30000); //16000
LCD_CONTROL;           /* ������� ������� ���������� */
PORT_LCDKEY = 0x03;    /* �������� ������ � ������� ����������/���������� */
LoadLcd();             /* ��������� ������ � LCD ��������� */
delay_mks(4600);//4100
PORT_LCDKEY = 0x03;    /* �������� ������ � ������� ����������/���������� */
LoadLcd();             /* ��������� ������ � LCD ��������� */
delay_mks(4600);//100
PORT_LCDKEY = 0x03;    /* �������� ������ � ������� ����������/���������� */
LoadLcd();             /* ��������� ������ � LCD ��������� */
delay_mks(4600);//4100
PORT_LCDKEY = 0x02;    /* �������� ������ � ������� ����������/���������� */
LoadLcd();             /* ��������� ������ � LCD ��������� */
delay_mks(4600);//60
                       /* 4-bit operation, 1/16 duty cycle, font 5x8 dot matrix */
PORT_LCDKEY = 0x02;    /* �������� ������ � ������� ����������/���������� */
LoadLcd();             /* ��������� ������ � LCD ��������� */
PORT_LCDKEY = 0x08;    /* �������� ������ � ������� ����������/���������� */
LoadLcd();             /* ��������� ������ � LCD ��������� */
delay_mks(4600);//60
                       /* Display off, cursor off, blink off */
PORT_LCDKEY = 0x00;    /* �������� ������ � ������� ����������/���������� */
LoadLcd();             /* ��������� ������ � LCD ��������� */
PORT_LCDKEY = 0x08;    /* �������� ������ � ������� ����������/���������� */
LoadLcd();             /* ��������� ������ � LCD ��������� */
delay_mks(4600);//60
                       /* Display on */
PORT_LCDKEY = 0x00;    /* �������� ������ � ������� ����������/���������� */
LoadLcd();             /* ��������� ������ � LCD ��������� */
PORT_LCDKEY = 0x0C;    /* �������� ������ � ������� ����������/���������� */
LoadLcd();             /* ��������� ������ � LCD ��������� */
delay_mks(240);//60

PORT_LCDKEY = 0x00;    /* �������� ������ � ������� ����������/���������� */
LoadLcd();             /* ��������� ������ � LCD ��������� */
PORT_LCDKEY = 0x01;    /* �������� ������ � ������� ����������/���������� */
LoadLcd();             /* ��������� ������ � LCD ��������� */
delay_mks(240);//60

                       /* Set entry mode, cursor moving right */
PORT_LCDKEY = 0x00;    /* �������� ������ � ������� ����������/���������� */
LoadLcd();             /* ��������� ������ � LCD ��������� */
PORT_LCDKEY = 0x06;    /* �������� ������ � ������� ����������/���������� */
LoadLcd();             /* ��������� ������ � LCD ��������� */
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
//#pragma vector = TIMER0_COMP_vect /* ���������� ������ ������������ */
//__interrupt void RefreshLcdKbd(void) /* ����� ���������� � ����������� ������� */
void lcd_int(void)
{
 unsigned char disp_temp; /* ��������� ������� ������� */
 unsigned char kbd;
// TCNT0 = 0xAF; /* ������������� ������ */
  f_ButtonFiltr();
 if(!--sec_recount)
 {
  SET_SECONDS; /* ���������� ���� "��������� �������" */
  sec_recount = 1000;
//  sec_recount = 200;
 }
 if(!--blink_count)
 {
  TOGGLE_BLINK;
  blink_count = 500;
//  if(ALARM) /* ��������� ���� "�������" */
//   BELL_TOGGLE;
 }
 if(START_DELAY) /* ��������� ���� ���������� ������ �������� */
 {
  if(!--repeat_count) /* ���� �������� ������ �������� */
  {
   SET_STOP_DELAY; /* ���������� ���� ��������� �������� */
   RESET_START_DELAY; /* �������� ���� ������ �������� */
  }
 }
 if(START_BEEP) /* ��������� ���� ������ �������� ��������� ������� */
 {
  BELL_TOGGLE;
  if(!--beep_count) /* ���� �������� ������ �������� */
  {
   BELL_OFF; /* ��������� �������� ������ */
   RESET_START_BEEP; /* �������� ���� ������ �������� ��������� ������� */
  }
 }
//KKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKK
 // kbd = (PING & 0x0F) ; // 
// kbd >>= 4;
 kbd = v_key;

if(!(kbd & kbd_shift)) /* ������� ��������� ���������� �� �������� ���� ����� PA */
{                       /* ������� ������ ������ */
 if(!(keyboard & kbd_shift)) /* ���� ������ �� ���� �������� */
  {
   if(!SCAN_READY) /* ���� ���������� ����-��� ��������� */
   {
    scan_code = kbd_count; /* ����� ����-��� ������ */
    SET_SCAN_READY; /* ���������� ���� ���������� ����-���� */
   }
   BELL_ON; /* �������� ������ ������� ������ */
   SET_START_BEEP; /* ���������� ���� ������ �������� ��������� ������� */
   beep_count = DELAY_BEEP;  /* ������������ � �� ��������� ������� */
   SET_START_DELAY; /* ���������� ���� ������ �������� ����� ������������ */
   repeat_count = DELAY_REPEAT; /* �������� � �� ����� ������������ */
  }
 else /* ���� ������ ���� �������� */
  {
   if(STOP_DELAY) /* ��������� ��������� �������� ����� ������������ */
   {
    SET_KEYB_REPEAT; /* ���������� ���� �������� �������� ����������� */
    if(!SCAN_READY && AUTO_REPEAT) /* ���� ���������� ����-��� ��������� � �������� ���������� */
    {
     scan_code = kbd_count; /* ����� ����-��� ������ */
     SET_SCAN_READY; /* ���������� ���� ���������� ����-���� */
    }
    RESET_STOP_DELAY; /* �������� ���� ��������� �������� */
    SET_START_DELAY; /* ���������� ���� ������ �������� ������� ����������� */
    repeat_count = TIME_REPEAT; /* ������ ����������� */
   }
  }
 keyboard |= kbd_shift; /* ���������� ��������������� ��� ������� ������ */
}
else /* ������� ������ ������ */
{
 if(keyboard & kbd_shift) /* ���� ������ ���� �������� � �������� */
 {
  RESET_START_DELAY; /* �������� ���� ������ �������� */
  RESET_STOP_DELAY; /* �������� ���� ��������� �������� */
  RESET_KEYB_REPEAT; /* �������� ���� �������� �������� ����������� */
 }
 keyboard &= ~kbd_shift; /* �������� ��������������� ��� ������� ������ */
}

 kbd_count++;
 kbd_shift <<= 1;    /* �������� �� ��������� ������� */

 if(kbd_count == 4) /* ��������� ������� ������ �� ��������� ������ */
 {
  kbd_count = 0;    /* ������� ��������� ����-���� */
  kbd_shift = 0x01; /* ��������� �� ������� ������� ������ */
 }


if(disp_count == 32) /* ��������� ������� �� ��������� ������ */
 {
  disp_count = 0;       /* �������� ������� */
  LCD_CONTROL;          /* ������� ������� ���������� */
  PORT_LCDKEY &= 0xF0;  /* �������� ������� ������� */
  PORT_LCDKEY |= 0x08;  /* ���������� ����� ������� ������� ������ ������ */
  LOAD_LCD;             /* ��������� ������� ������� � ��������� */
  PORT_LCDKEY &= 0xF0;  /* �������� ������� ������� */
  LOAD_LCD;             /* ��������� ������� ������� � ��������� */
  RESET_TWO_LINE;       /* �������� ���� ������ ������ */
  return;
 }
if(disp_count == 16 && !TWO_LINE)
 {
  LCD_CONTROL;          /* ������� ������� ���������� */
  PORT_LCDKEY &= 0xF0;  /* �������� ������� ������� */
  PORT_LCDKEY |= 0x0C;  /* ���������� ����� ������� ������� ������ ������ */
  LOAD_LCD;             /* ��������� ������� ������� � ��������� */
  PORT_LCDKEY &= 0xF0;  /* �������� ������� ������� */
  LOAD_LCD;             /* ��������� ������� ������� � ��������� */
  SET_TWO_LINE;         /* ���������� ���� �������� �� ������ ������ */
  return;
 }
LCD_DATA; /* ������� ������� ������ */
disp_temp = display[disp_count];
if(disp_temp>0x7f)
  disp_temp = TableDecode[display[disp_count] - 0x80];

if(TWO_LINE)   /* ���������� ��������� ������ */
 {             /* ������� �� 2 ������ */
 if(checkbit(blinc_line2,disp_count-16) && BLINK) /* ��������� ������ �� ������� */
  disp_temp = 0x20; /* ��� ������� */
 }
else
 {             /* ������� �� 1 ������ */
 if(checkbit(blinc_line1,disp_count) && BLINK) /* ��������� ������ �� ������� */
  disp_temp = 0x20; /* ��� ������� */
 }
PORT_LCDKEY &= 0xF0; /* �������� ������� ������� */
PORT_LCDKEY |= disp_temp >> 4; /* �������� ������� ������� � ������� ����������/���������� */
LOAD_LCD;              /* ��������� ������ � LCD ��������� */
PORT_LCDKEY &= 0xF0; /* �������� ������� ������� */
PORT_LCDKEY |= (disp_temp & 0x0F); /* �������� ������� ������� � ������� ����������/���������� */
LOAD_LCD;              /* ��������� ������ � LCD ��������� */
disp_count ++;
}

/* ��������� �� ���������� ������, ������ �������, ����������� ��������, ������������ ��������, ������� �������� ����� ������������ �������� */
void ParametrUp(void *ptr,unsigned int size,unsigned int min_param,unsigned int max_param,unsigned char over_max)
{
if(size == sizeof(unsigned int)) /* ���� �������� ���� unsigned int */
 {
  unsigned int *param = (unsigned int *)ptr;
  if(*param < max_param)
  {  // ���� ���� �������� �������� ����������� ����������, �� ��������� �������� �� 10 ����� �� 1
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
else /* ����� �������� ���� unsigned char */
 {
  unsigned char *param = (unsigned char *)ptr;
  if(*param < max_param)
  {  // ���� ���� �������� �������� ����������� ����������, �� ��������� �������� �� 10 ����� �� 1
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
SET_SAVE; /* ���������� ���� "��������� ���������" */
}

/* ��������� �� ���������� ������, ������ �������, ����������� ��������, ������������ ��������, ������� �������� ����� ����������� �������� */
void ParametrDown(void *ptr,unsigned int size,unsigned int min_param,unsigned int max_param,unsigned char over_min)
{
if(size == sizeof(unsigned int)) /* ���� �������� ���� unsigned int */
 {
  unsigned int *param = (unsigned int *)ptr;
  if(*param > min_param)
  { // ���� ���� �������� �������� ����������� ����������, �� ��������� �������� �� 10 ����� �� 1
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
else /* ����� �������� ���� unsigned char */
 {
  unsigned char *param = (unsigned char *)ptr;
  if(*param > min_param)
  { // ���� ���� �������� �������� ����������� ����������, �� ��������� �������� �� 10 ����� �� 1
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
SET_SAVE; /* ���������� ���� "��������� ���������" */
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
