
#ifndef TokMetr__h
#define TokMetr__h

namespace nsTok
{
    //---------------------------------����������, �����, ���������--------------------------------------------------------------
    extern unsigned int acp_date;   // ����� ������ � ���
    extern unsigned int usrTok;     // ����������� �������� ����
    extern unsigned int usrTokA;    // ����������� �������� ���� � �������
    extern unsigned int kAcpToTok;  // ���������� ��� ���
    //---------------CORE--------------------
    void timer();
    void init();
    void ReadKacpToTok();
    void WriteKacpToTok();
}

#endif

