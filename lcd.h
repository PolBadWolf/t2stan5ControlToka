
#ifndef lcd__h
#define lcd__h

// CORE
void lcd_int(void);
void InitLcd(void);     /* ������������� ���������� */
void LoadSymbol(void); /* ��������� � ��������� �������������� ������� */
// ��������� ������ � LCD ���������
void LoadLcd(void);
extern unsigned int blinc_line1;      /* ������� ������ ������� 1 ������ ���������� */
extern unsigned int blinc_line2;      /* ������� ������ ������� 2 ������ ���������� */
// User
void Show(unsigned char pos, unsigned char *t);
void ClearDisplay(void); /* �������� ��������� */
void ShowChar(unsigned char pos, unsigned char ch);
unsigned char scan_ready(void);
unsigned char read_key(void);
// �������� ����� � �������� ������ � ���������� ���� � ���������� ������� � ������������ ����������� ����
void ShowDigitZ(unsigned char pos, unsigned char numdigit, unsigned int digit);
void ShowDigitZLONG(unsigned char pos, unsigned char numdigit, unsigned long digit);
void set_auto_repeat (void);//������� ��������� �����������
void reset_auto_repeat (void);//������� ���������� �����������

/*
unsigned char ReadEeprom(unsigned int addr); // ��������� ���� �� EEPROM
void WriteEeprom(unsigned int addr,  unsigned char data); // �������� ���� � EEPROM
void String2Rs( unsigned char *string);// ������� ������ �� rs � �������� �������
void InitUART(void);//������������� UART
void InitUART2(void);//������������� RS485
char WriteCom2Byte(unsigned char data);//������ ������ � RS485
char ReadCom2Byte(unsigned char *Byte);//������ ������ � RS485
char WriteComByte(unsigned char data);//�������� ����� � rs
//void Data2BufferRs( unsigned char data);// ������ � ����� rs � ���������
void Peredaha_rs232 (void);//������ ������ � RS232
void in_rs485(void);//���� ����������� � ��������� �� RS485
*/
#endif

