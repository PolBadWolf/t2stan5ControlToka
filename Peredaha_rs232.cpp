
#include "Peredaha_rs232.h"
#include "main.h"
//----------------------���������� ��� �������� ������ � RS232------------//
extern unsigned long skorost_stana;//�������� �������� ����� �/���
extern unsigned long ACP_DANNIE_RS;//��������������  ������ ACP ��� �������� � RS232
extern unsigned int  Pirometr;//������ � ���������
extern unsigned char Start_Data_RS;//���� ���������� �������� ������ (������������� 2 ���� � 1���.)0-���������.1-���������.
//-------���������� ��� ��������������� � ��������� ��� ������------------//
unsigned char str_skorost_stana[10];
unsigned char str_ACP_DANNIE_RS[10];
unsigned char str_PIROMETR[10];
//-----------------------������ � ���������� ����������� �����-------------------------//
unsigned char Kontr_summa_dannix [22];//�������� � ���������� ������� ����������� ������
unsigned int hislo_crc;//�������� ����������� ����� � ���� �����
unsigned char  crc [10];//�������� ����������� ����� � ���� ������
//-------------------------------------------------------------------------------------//
// ������, ���-�� ��������, ����������, �����
//-----------------------------------------------------------------------
void Long2String( unsigned char *pos, unsigned char n, unsigned long digit, unsigned char tchk )
{
    unsigned char len=0;
    unsigned long temp;
    unsigned long temp1;
    unsigned char temp2;
    unsigned char *s;
    s = pos + n+1;
    *--s=0;
    temp = digit;
    for(temp2=0;temp2<n;temp2++)
    {
        if (++len==tchk+1) *--s='.';
        else
        {
            temp1 = temp /10;
            if ( temp>0 ) *--s = temp-temp1*10+'0';
            else  *--s = '0';
            temp = temp1;
        }
    }
}
void Peredaha_rs232 (void)
{
if (Start_Data_RS==1)
{
Start_Data_RS=0;//����� ����� � ����
//-------------------------------------------------------------------------------------------------------------------------------
Long2String(str_skorost_stana,6,skorost_stana,2);//�������������� ��������� �������� �������� �������� � ���������  
Long2String(str_ACP_DANNIE_RS,5,ACP_DANNIE_RS,0 );//�������������� ��������� ����������� �������� � ��� � ���������  
Long2String(str_PIROMETR,5,Pirometr,0);//�������������� ��������� �������� � ��������� � ���������
Kontr_summa_dannix[0] =str_skorost_stana[0];
Kontr_summa_dannix[1] =str_skorost_stana[1];
Kontr_summa_dannix[2] =str_skorost_stana[2];
Kontr_summa_dannix[3] =str_skorost_stana[3];
Kontr_summa_dannix[4] =str_skorost_stana[4];
Kontr_summa_dannix[5] =str_skorost_stana[5];//' ';
Kontr_summa_dannix[6]=' ';
Kontr_summa_dannix[7] =str_ACP_DANNIE_RS[0];
Kontr_summa_dannix[8] =str_ACP_DANNIE_RS[1];
Kontr_summa_dannix[9] =str_ACP_DANNIE_RS[2];
Kontr_summa_dannix[10] =str_ACP_DANNIE_RS[3];
Kontr_summa_dannix[11]=' ';
Kontr_summa_dannix[12]=str_PIROMETR[0];
Kontr_summa_dannix[13]=str_PIROMETR[1];
Kontr_summa_dannix[14]=str_PIROMETR[2];
Kontr_summa_dannix[15]=str_PIROMETR[3];
Kontr_summa_dannix[16]='#';
hislo_crc=crc8_buf(Kontr_summa_dannix,16);//������� ����������� �����  
Long2String (crc,4,hislo_crc,0);//�������������� � ��������� ���
Kontr_summa_dannix[17]=crc[0];
Kontr_summa_dannix[18]=crc[1];
Kontr_summa_dannix[19]=crc[2];
Kontr_summa_dannix[20]=0x0d;
Kontr_summa_dannix[21]=0x0a;
for(unsigned char x=0;x<22;x++) Data2BufferRs(Kontr_summa_dannix[x]);//������ ������ � RS232
//-----------------
}
}

