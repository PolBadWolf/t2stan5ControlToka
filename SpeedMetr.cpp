
#include "SpeedMetr.h"
#include "main.h"
namespace nsSpeed
{
    //-------------------------------����������, ����� ��� ������������--------------------------------------------------------------------------------------
    unsigned int rezultat_skorosti = 0;     // ��������� �������� �������� ��/��� 
    unsigned int shethik_impulsow  = 0;     // C������ ���������
    unsigned char flag_old         = 0;     // ������ �������� ��������
    unsigned char flag_new         = 0;     // ����� �������� ��������
    unsigned char flag_reset_shet  = 0;     // ���� ��������� ���������-���� 1 ���� �������� 0-��������
    unsigned int Buf_impuls        = 0;     // ����� ��� ��������� ������ � �������� ���������
    unsigned long skorost_stana    = 0;     // �������� ����� ����� � ������
    unsigned int S                 = 0;     // ������ ����������
    unsigned char newData          = 0;     // ����� ��������
    unsigned long bufer_skorost    = 0;     // �������� �������� ����� ����� � ������ 
    //---------------���������� ��� ���������� �������� ������� � ������������ �������-------------------------------------------
    unsigned char D1        = 0;    // ���������� ��� �������� ������� � ������������ �������
    unsigned int Chethik_D1 = 0;    // ������� ��� ���������� ��������
    unsigned char D_1       = 0;    // ��������������� ��������� �������. ���� 0-������ ���. ���� 1-������ ����.
                                    // (��� ������� ���������� 2��� ����� ���������� 10 ���.���)
    //------------���� ������� ��������------------------------------------
    #define SPEED_SENSOR_RD PINE_PINE2

    // ��������� �������� �������� �����
    void Skorost()
    {
        flag_new=D_1;//��������� ����� �������� ������� 
        if (flag_reset_shet==1)
        {
            shethik_impulsow++;//��������� �������� 
        }
        if (shethik_impulsow>=3000)//�������� �������� �� ������������
        {
            flag_reset_shet=0;//���� ��������
            shethik_impulsow=0;//�������� ��������
            Buf_impuls=0;//��������� ������ ������, ���������� �� �������� ���������, ��� ���������
            rezultat_skorosti=0;//��������� 
        }
        if ((flag_old!=flag_new)&&(flag_new!=0))//���� ������ �������� ������� �� ����� ������ � ����� �� ����� ����
        {
            flag_reset_shet=1;//��������
            rezultat_skorosti=(120000/1)/(shethik_impulsow);//��������� �������� ��/��� (1-���.��������)            
            Buf_impuls=shethik_impulsow;//�������� ������ �� �������� � ����� ������ ��� ���������
            shethik_impulsow=0;//�������� �������� 
            newData = 1;
        }                                       
        flag_old=flag_new;//����� �������� ������� ��������� �������
    }

    void signal_D1()
    {
        //D1=PINE & 0x4;
        //2-�� ��� ���� E.  
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
        // ����� �������
        signal_D1();
        // ��������� �������� �������� �����
        Skorost();
    }
    // init
    void init()
    {
        // ����������� ������ ����������
        ((unsigned char *)&S)[0] = ReadEeprom(15);// - 1����
        ((unsigned char *)&S)[1] = ReadEeprom(16);// - 2����
    }
}

