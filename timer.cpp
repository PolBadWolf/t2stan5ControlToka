
#include "main.h"

//---------------------------------����������, �����, ���������--------------------------------------------------------------
unsigned long Massiv_ACP [100];//������ ���
unsigned char Index_M=0;//������ �������
unsigned char Flag_end=0;//���� ����������� ���������� �������
unsigned long summa=0;//���������� ��� �������� ������������ ����������� � ���
unsigned long Rezult_ADC=0;//���������� ��� ������ �������� � ��� �� ���������
unsigned long acp_date=0;//����� ������ � ���
//---------------���������� ��� ���������� �������� ������� � ������������ �������-------------------------------------------
unsigned char D1=0;//���������� ��� �������� ������� � ������������ �������
unsigned int Chethik_D1=0;//������� ��� ���������� ��������
unsigned char D_1=0;//��������������� ��������� �������. ���� 0-������ ���. ���� 1-������ ����.
                /*(��� ������� ���������� 2��� ����� ���������� 10 ���.���)*/
//-------------------------------����������, ����� ��� ������������--------------------------------------------------------------------------------------
unsigned int Buf_impuls=0;//����� ��� ��������� ������ � �������� ���������
//----------------------------------------------------------------------------------------------------------------------------
//-------------------------���������� � ����� ��� �������� �������� ������ � RS-----------------------------------------------
unsigned int Z1,Z2;
unsigned char Start_Data_RS485=0;//���� 1-�������� � RS-485 ���������.���� 0-���������.
unsigned char Start_Data_RS=0;//���� 1-�������� � RS-232 ���������.���� 0-���������.
//----------------------------------------------------------------------------------------------------------------------------
void Time_out_pirometr (void)
{  
if (Z2!=0)
   {Z2--;}
else {Z2=2000;Start_Data_RS485=1;}
}
void Zaderga_peredahi_RS (void)
{
 if (Z1!=0)
    {Z1--;}
 else {Z1=1000;Start_Data_RS=1;}
}
void signal_D1 (void)
{
D1=PINE & 0x4;//2-�� ��� ���� E.  
if (D1==0)
{
if (Chethik_D1==0)
 {
  D_1=0;
 }
else 
 {
  Chethik_D1--;
 }
}
else
{
 if (Chethik_D1>=20) 
 {
  D_1=1;
 }
 else
 {
  Chethik_D1++;
 }
}
}
/*
unsigned int adc_read(void)//������� ������ � ���
{
 unsigned int temp;
 LOW(temp) = ADCL; 
 HIGH(temp) = ADCH & 3;
 return temp;
}
void adc_pusk( unsigned char n )//������ ���
{
 ADMUX = n;    // ����� ������
 ADCSRA = 0xD7;
}
*/
void timer_sys_pusk(void)//������������� �������/�������� 0 (2 ���)
{
 TCCR0 = 0x0c;  // ����������� ������� 64 , ������� ��� ����������
 TCNT0 = 0;     // Fx=Fs/( TCCR0 * ( 1+ OCR0 ) )
 OCR0 = 125;//2��� //249=1���;
 TIMSK |= 1<<OCIE0; // ��������� ���������� �� Timer/Counter 0
}
void Skorost (void)//��������� �������� �������� �����
{
flag_new=D_1;//��������� ����� �������� ������� 
if (flag_reset_shet==1)
{
 shethik_impulsow++;//��������� �������� 
}
if (shethik_impulsow>=3000)//�������� �������� �� ������������
{
 flag_reset_shet=0;//���� ��������
 shethik_impulsow=0;//�������� ��������
 Buf_impuls=0;//��������� ������ ������, ���������� �� �������� ���������, ��� ���������
 rezultat_skorosti=0;//��������� 
}
if ((flag_old!=flag_new)&&(flag_new!=0))//���� ������ �������� ������� �� ����� ������ � ����� �� ����� ����
{
flag_reset_shet=1;//��������
rezultat_skorosti=(120000/1)/(shethik_impulsow);//��������� �������� ��/��� (1-���.��������)            
Buf_impuls=shethik_impulsow;//�������� ������ �� �������� � ����� ������ ��� ���������
shethik_impulsow=0;//�������� �������� 
}                                       
flag_old=flag_new;//����� �������� ������� ��������� �������
}

void usred (void)
{ 
  if (Flag_end==1)
   {
     summa=summa-Massiv_ACP[Index_M];
     Massiv_ACP[Index_M]=adc_read();
     summa=summa+Massiv_ACP[Index_M];
     Rezult_ADC=summa/100;
     Index_M++;
     if (Index_M==100)
        {Index_M=0;}
   } 
   else
   {
   if (Index_M==100)
      {
       Flag_end=1;
       Index_M=0;
      }
   else
      {
      Massiv_ACP[Index_M]=adc_read();
      summa=summa+Massiv_ACP[Index_M];
      Rezult_ADC=summa/Index_M;
      Index_M++;
      }
   }
}










void timer_sys(void)
{
lcd_int();//������������� ���
signal_D1();//����� �������
Skorost ();//��������� ��������
//adc_pusk(0);// ������ ��� (� ������� ����� ������=0)
usred ();//������ � ���  ���������� ������
//acp_date=adc_read();//����� ������ � ���
Time_out_pirometr ();//�������� ����� �������� ���������
Zaderga_peredahi_RS();//�������� ��� �������� � RS232 ������
}














// =========================================================
// =     ��������� ���������� Timer 0  �������  2���       =
// =========================================================
#pragma vector = TIMER0_COMP_vect
__interrupt void tim0_interrupt(void)
{
  timer_sys();
}


