
#ifndef adc__h
#define adc__h

void adc_pusk( unsigned char n );//������������� ���
unsigned int adc_read();

extern unsigned long ACP_DANNIE;     // ���������� ������ � ���
extern unsigned long ACP_DANNIE_RS;  // ��������������  ������ ACP ��� �������� � RS232
extern unsigned long Smehenie_zero;  // �������� ����
extern unsigned long Korecsia_zero;  // ������������ ��������� ���� ��� ��� 


#endif

