
#include "TokMetr.h"
#include "main.h"

namespace nsTok
{
    //---------------------------------����������, �����, ���������--------------------------------------------------------------
    unsigned long Massiv_ACP[100];  // ������ ���
    unsigned char Index_M    = 0;   // ������ �������
    unsigned char Flag_end   = 0;   // ���� ����������� ���������� �������
    unsigned long summa      = 0;   // ���������� ��� �������� ������������ ����������� � ���
    unsigned int  acp_date   = 0;   // ����� ������ � ���
    unsigned char newDate    = 0;   // ����� ������
    unsigned int  usrTok     = 0;   // ����������� �������� ����
    //---------------CORE--------------------
    void Usred(unsigned int zadc)
    { 
        if (Flag_end==1)
        {
            summa=summa-Massiv_ACP[Index_M];
            Massiv_ACP[Index_M]=nsAcp::adc_read();
            summa=summa+Massiv_ACP[Index_M];
            usrTok = summa/100;
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
                Massiv_ACP[Index_M]=nsAcp::adc_read();
                summa=summa+Massiv_ACP[Index_M];
                usrTok = summa/Index_M;
                Index_M++;
            }
        }
    }
    //---------------for timer---------------
    void timer()
    {
        nsAcp::adc_pusk(0, Usred);            // ������ ��� (� ������� ����� ������=0)
        //usred ();               // ������ � ���  ���������� ������
        acp_date=nsAcp::adc_read();    // ����� ������ � ���
        newDate = 1;
    }
}

