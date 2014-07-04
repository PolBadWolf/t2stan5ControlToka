
#include "TokMetr.h"
#include "main.h"

namespace nsTok
{
    //---------------------------------Переменные, флаги, константы--------------------------------------------------------------
    unsigned long Massiv_ACP[100];  // Массив АЦП
    unsigned char Index_M    = 0;   // Индекс массива
    unsigned char Flag_end   = 0;   // Флаг регистрации заполнения массива
    unsigned long summa      = 0;   // Переменная для хранения суммированых результатов с АЦП
    unsigned int  acp_date   = 0;   // Голые данные с ацп
    unsigned char newDate    = 0;   // новые данные
    unsigned int  usrTok     = 0;   // усредненное значение тока
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
        nsAcp::adc_pusk(0, Usred);            // Запуск АЦП (в скобках номер канала=0)
        //usred ();               // Чтение с АЦП  усреднённых данных
        acp_date=nsAcp::adc_read();    // Голые данные с АЦП
        newDate = 1;
    }
}

