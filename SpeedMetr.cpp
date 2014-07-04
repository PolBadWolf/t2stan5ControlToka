
#include "SpeedMetr.h"
#include "main.h"
namespace nsSpeed
{
    //-------------------------------Переменные, флаги для скоростимера--------------------------------------------------------------------------------------
    unsigned int rezultat_skorosti = 0;     // Результат скорости скорость об/мин 
    unsigned int shethik_impulsow  = 0;     // Cчётчик импульсов
    unsigned char flag_old         = 0;     // старое значение импульса
    unsigned char flag_new         = 0;     // новое значение импульса
    unsigned char flag_reset_shet  = 0;     // Флаг остановки измерения-Если 1 счёт разрешон 0-запрещён
    unsigned int Buf_impuls        = 0;     // Буфер для просмотра данных с счётчика импульсов
    unsigned long skorost_stana    = 0;     // скорость стана метры в минуту
    unsigned int S                 = 0;     // Длинна окружности
    unsigned char newData          = 0;     // новая скорость
    unsigned long bufer_skorost    = 0;     // хранение скорости стана метры в минуту 
    //---------------Переменные для фильтрации входного сигнала с индуктивного датчика-------------------------------------------
    unsigned char D1        = 0;    // Переменная для входного сигнала с индуктивного датчика
    unsigned int Chethik_D1 = 0;    // Счётчик для устранения дребезга
    unsigned char D_1       = 0;    // Отфильтрованное состояние датчика. Если 0-датчик вкл. Если 1-датчик выкл.
                                    // (при частоте прерывания 2КГц время фильтрации 10 мил.сек)
    //------------Порт датчика скорости------------------------------------
    #define SPEED_SENSOR_RD PINE_PINE2

    // Измерение линейной скорости стана
    void Skorost()
    {
        flag_new=D_1;//прочитать новое значение датчика 
        if (flag_reset_shet==1)
        {
            shethik_impulsow++;//инкремент счётчика 
        }
        if (shethik_impulsow>=3000)//проверка счётчика на переполнение
        {
            flag_reset_shet=0;//Счёт запрещён
            shethik_impulsow=0;//бнуление счётчика
            Buf_impuls=0;//обнуление буфера данных, полученных со счётчика импульсов, для просмотра
            rezultat_skorosti=0;//обнуление 
        }
        if ((flag_old!=flag_new)&&(flag_new!=0))//если старое значение датчика не равно новому и новое не равно нулю
        {
            flag_reset_shet=1;//разрешон
            rezultat_skorosti=(120000/1)/(shethik_impulsow);//Расчитать скорость об/мин (1-кол.секторов)            
            Buf_impuls=shethik_impulsow;//передача данных со счётчика в буфер данных для просмотра
            shethik_impulsow=0;//бнуление счётчика 
            newData = 1;
        }                                       
        flag_old=flag_new;//новое значение датчика присвоить старому
    }

    void signal_D1()
    {
        //D1=PINE & 0x4;
        //2-ой бит порт E.  
        D1 = SPEED_SENSOR_RD;
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
    // ================ USER ===============================
    // -----------Speed Stan-----------------------------
    unsigned long SpeedStana()
    {
        if (newData)
        {
            {
                CritSec csSpeed;
                bufer_skorost = rezultat_skorosti;
            }
            skorost_stana = S*bufer_skorost/10;
            newData = 0;
        }
        return skorost_stana;
    }
    
    
    // for timer
    void timer()
    {
        // Опрос датчика
        signal_D1();
        // Измерение линейной скорости стана
        Skorost();
    }
    // init
    void init()
    {
        // Коэффициент длинны окружности
        ((unsigned char *)&S)[0] = ReadEeprom(15);// - 1байт
        ((unsigned char *)&S)[1] = ReadEeprom(16);// - 2байт
    }
}

