
#include "main.h"

//extern unsigned long Korecsia_zero;////������������ ��������� ���� ��� ��� 
//extern unsigned long Smehenie_zero;//�������� ����
//extern unsigned int S,Z2,Z1;//������ ����������
//extern unsigned char Start_Data_RS485;//���� 1-�������� � RS-485 ���������.���� 0-���������.
void iniz_port (void)
{
DDRB=0x00;//���� B �� ������
PORTB=0xFF;//�������� � ����

DDRD = 0xF2; //���� D ��������� ��� ����  
PORTD= 0xF0; //�������� � ���� D  

DDRE=0x00;//���� E �� ������
PORTE=0xFF;//�������� � ����

DDRF=0x00;//���� F �� ������// ���
PORTF=0x00;//�������� � ����

DDRG=0x00;//���� G �� ������
PORTG=0xFF;//�������� � ����
}
//-----------------------------------------------------------------------------------------------------------
void main( void )
{  
iniz_port(); // ������������� ������
timer_sys_pusk(); // ������������� �������
InitLcd(); // ������������� ���
InitUART();//������������� RS232
InitUART2();//������������� RS485
LoadSymbol();// �������� �������������� �������� � ���
__enable_interrupt(); // ��������� ���������� ����������      
ClearDisplay();// ������� ���   
__delay_cycles (3200000);//�������� 0,2 ������� ��� ���������� �������� �������� �������� ������� � ������������ �������
//------------------������������� ������ �� ����� ������------------------------------------------------------- 
// ����������� ������ ����������
nsSpeed::init();
/*
((unsigned char *)&S)[0] = ReadEeprom(15);// - 1����
((unsigned char *)&S)[1] = ReadEeprom(16);// - 2����
*/
                 //����������� ���
nsTok::init();
/*
((unsigned char *)&ACP)[0] = ReadEeprom(19);// - 1����
((unsigned char *)&ACP)[1] = ReadEeprom(20);// - 2����
((unsigned char *)&ACP)[2] = ReadEeprom(21);// - 3����
((unsigned char *)&ACP)[3] = ReadEeprom(22);// - 4����
*/
          /*����������� ���������� ����*/
//((unsigned char *)&Korecsia_zero)[0] = ReadEeprom(23);// - 1����
//((unsigned char *)&Korecsia_zero)[1] = ReadEeprom(24);// - 2����
//((unsigned char *)&Korecsia_zero)[2] = ReadEeprom(25);// - 3����
//((unsigned char *)&Korecsia_zero)[3] = ReadEeprom(26);// - 4����
            /*�������� ����*/
//((unsigned char *)&Smehenie_zero)[0] = ReadEeprom(27);// - 1����
//((unsigned char *)&Smehenie_zero)[1] = ReadEeprom(28);// - 2����
//((unsigned char *)&Smehenie_zero)[2] = ReadEeprom(29);// - 3����
//((unsigned char *)&Smehenie_zero)[3] = ReadEeprom(30);// - 4����

//---------------����� �������������----------------------------
for (;;)
{    
menu ();
Peredaha_rs232();//�������� ������ � RS232
in_rs485 ();//���� ����������� c ���������  �� RS485 
if (Start_Data_RS485==1)//�������� ��������� �������
  {
    Start_Data_RS485=0;//����� �����
    termoskop();//������ ����������� � ��������� �� RS485
  }

}
}

  
 


