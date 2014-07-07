
#include "TokMetr.h"
#include "main.h"

#define Massiv_ACP_MAX 100
namespace nsTok
{
    //---------------------------------Переменные, флаги, константы--------------------------------------------------------------
    unsigned long Massiv_ACP[Massiv_ACP_MAX];   // Массив АЦП
    unsigned char Index_M    = 0;   // Индекс массива
    unsigned char curLenMas  = 0;   // Флаг регистрации заполнения массива
    unsigned long summa      = 0;   // Переменная для хранения суммированых результатов с АЦП
    unsigned int  acp_date   = 0;   // Голые данные с ацп
    unsigned char newData    = 0;   // новые данные
    unsigned char flReady    = 1;   // флаг готовности к новому чтению АЦП
    unsigned int  usrTok     = 0;   // усредненное значение тока
    unsigned int  usrTokA    = 0;   // усредненное значение тока в Амперах
    unsigned int  kAcpToTok  = 0;   // Коэфициент для АЦП
    __eeprom unsigned int  EEkAcpToTok = 1000;   // Коэфициент для АЦП в EEPROM
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
            nsAcp::adc_pusk(0, Usred);  // Запуск АЦП (в скобках номер канала=0)
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

