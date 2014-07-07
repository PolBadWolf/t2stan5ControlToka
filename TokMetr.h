
#ifndef TokMetr__h
#define TokMetr__h

namespace nsTok
{
    //---------------------------------Переменные, флаги, константы--------------------------------------------------------------
    extern unsigned int acp_date;   // Голые данные с ацп
    extern unsigned int usrTok;     // усредненное значение тока
    extern unsigned int usrTokA;    // усредненное значение тока в Амперах
    extern unsigned int kAcpToTok;  // Коэфициент для АЦП
    //---------------CORE--------------------
    void timer();
    void init();
    void ReadKacpToTok();
    void WriteKacpToTok();
}

#endif

