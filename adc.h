
#ifndef adc__h
#define adc__h

namespace nsAcp
{
    // ������������� ���
    void adc_pusk(unsigned char n);
    void adc_pusk(unsigned char n, void (*CallBackFunction)(unsigned int rez) );
    unsigned int adc_read();
    void init();
    
    //extern unsigned long ACP_DANNIE;     // ���������� ������ � ���
    extern unsigned long ACP_DANNIE_RS;  // ��������������  ������ ACP ��� �������� � RS232
    extern unsigned long Smehenie_zero;  // �������� ����
    extern unsigned long Korecsia_zero;  // ������������ ��������� ���� ��� ��� 
//    extern unsigned long ACP;//���������� ��� ���
//    extern unsigned int Rezult_ADC;      // ���������� ��� ������ �������� � ��� �� ���������
}
#endif

