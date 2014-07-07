
#include "TokMetr.h"
#include "main.h"

#define Massiv_ACP_MAX 100
namespace nsTok
{
    //---------------------------------����������, �����, ���������--------------------------------------------------------------
    unsigned long Massiv_ACP[Massiv_ACP_MAX];   // ������ ���
    unsigned char Index_M    = 0;   // ������ �������
    unsigned char curLenMas  = 0;   // ���� ����������� ���������� �������
    unsigned long summa      = 0;   // ���������� ��� �������� ������������ ����������� � ���
    unsigned int  acp_date   = 0;   // ����� ������ � ���
    unsigned char newData    = 0;   // ����� ������
    unsigned char flReady    = 1;   // ���� ���������� � ������ ������ ���
    unsigned int  usrTok     = 0;   // ����������� �������� ����
    unsigned int  usrTokA    = 0;   // ����������� �������� ���� � �������
    unsigned int  kAcpToTok  = 0;   // ���������� ��� ���
    __eeprom unsigned int  EEkAcpToTok = 1000;   // ���������� ��� ��� � EEPROM
    //---------------CORE--------------------
    void Usred(unsigned int zadc)
    {
        acp_date = zadc;
        // sub old data
        if (curLenMas<Massiv_ACP_MAX)
        {
            summa = summa - Massiv_ACP[Index_M];
        }
        // add new data
        summa = summa + zadc;
        Massiv_ACP[Index_M] = Massiv_ACP[Index_M];
        // index+1
        if (++Index_M)
        {
            Index_M = 0;
        }
        if (curLenMas<Massiv_ACP_MAX)
        {
            curLenMas++;
        }
        // ave
        usrTok = (summa/curLenMas)*kAcpToTok;
        // to Amper
        usrTokA = (usrTok+500)/1000;
        newData = 1;
        flReady = 1;
    }
    //---------------for timer---------------
    void timer()
    {
        if (flReady)
        {
            flReady = 0;
            nsAcp::adc_pusk(0, Usred);  // ������ ��� (� ������� ����� ������=0)
        }
    }
    //---------------init----------------------
    void init()
    {
        ReadKacpToTok();
    }
    void ReadKacpToTok()
    {
        kAcpToTok = EEkAcpToTok;
    }
    void WriteKacpToTok()
    {
        EEkAcpToTok = kAcpToTok;
    }
    void ClearBufferUsr()
    {
        CritSec csClear;
        curLenMas = 0;
    }
}

