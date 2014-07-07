
#include "menu.h"
#include "main.h"

namespace nsMenu
{
    //-------------------------------------------------------------------------
    int P1,P2,P3,P4,P5;//���������� ��� ����� ������
    unsigned char modes=0;
    //-------------------------------------------------------------------------
    //---------------------------����������, �����, ��������� ��� ��������-----
    //-------------------------------------------------------------------------
void Dummy (void)
{}
    //-------------------------------------------------------------------------
    // �������� ���������
    void Indicasia (void)
    {
        unsigned long ACP_DANNIE    = 0;    // ���������� ������ � ���
        //---------������ �������� �����-----------------------------------
        unsigned long speed = nsSpeed::SpeedStana();
        //-----------------����� �����������-------------------------------------
        Show (23,"T="); 
        ShowDigitZ (25,4,Pirometr);
        Show (31,"C");
        ShowChar (30,7);
        //-----------------------------------------------------------------------
        //��������� ������ ������������ �������
        Show (13,"�1"); 
        if (nsSpeed::D_1==0)
            ShowChar (15,4);
        if (nsSpeed::D_1==1) 
            ShowChar (15,3);
        // ����� �������� ����� M/���
        ShowDigitZ (0, 3, speed/100);
        Show (3,",");
        // ======= ��������� ��������� 2 ����� =======
        ShowDigitZ (4, 2, speed);
        Show (7,"�/���");
        //------------------------------------------------------------------------------
        //---------------------------------���------------------------------------------
        // ����������� ������
        {
            CritSec csm;
            ACP_DANNIE = nsTok::usrTokA;
        }
        //--------------------------����� ������ � ���----------------------------------
        /*
        ACP_DANNIE=(ACP_DANNIE-0)*nsAcp::ACP/1000;
        nsAcp::ACP_DANNIE_RS=ACP_DANNIE+0;
        ShowDigitZ (16,3,(ACP_DANNIE+0));//����� ������ � ���
        */
        ShowDigitZ (16, 3, ACP_DANNIE);//����� ������ � ��� � �������
        ShowChar(20,'A');
        //----------------------------------------------------------------------------------
    }
void Perexod_w_indicasia (void)//������� �� ���������� ���� "��������� ������������"
{
P1=0;//��������� ������ ����� ������
P2=0;//��������� ������ ����� ������
P3=0;//��������� ������ ����� ������ 
P4=0;//��������� �������� ����� ������ 
P5=0;//��������� ����� ����� ������
ClearDisplay();
modes=0;
}
void Perexod_w_menu (void)//���� � ���� "��������� ������������"
{
P1=0;//��������� ������ ����� ������
P2=0;//��������� ������ ����� ������
P3=0;//��������� ������ ����� ������ 
P4=0;//��������� �������� ����� ������ 
P5=0;//��������� ����� ����� ������
modes=1;
ClearDisplay();
}
void Menu_0 (void)//���� ������� �������
{
Show (0,"���������� ����!");
Show (16,"�����(�)");
ShowChar (30,0x7E);
Show (25,"����(");
Show (31,")");
}
void vvod_pasword (void)//���� �� ���� ������
{
modes=2;
ClearDisplay();
}
//------------------------------���� ������-------------------------------------
//------------------------------------------------------------------------------
void password_1 (void)//���� ������ �����
{
Show(1,"������� ������:"); 
Show(22,"----"); 
ShowDigitZ(21,1,P1);
}
void P_1_Plus (void)
{
   if (P1==9)
      {P1=0;}
       else
      {P1++;}
}
void P_1_Minus (void)
{
   if (P1==0)
      {P1=9;}
       else
      {P1--;}
}
void wwod_P1 (void)//������� �� ���� ������ ����� 
{
ClearDisplay();// ������� ���
modes=3;
}
void password_2 (void)//���� ������ �����
{
Show(1,"������� ������:"); 
Show(21,"*"); 
Show(23,"---"); 
ShowDigitZ(22,1,P2);
}
void P_2_Plus (void)
{
   if (P2==9)
      {P2=0;}
       else
      {P2++;}
}
void P_2_Minus (void)
{
   if (P2==0)
      {P2=9;}
       else
      {P2--;}
}
void wwod_P2 (void)
{
ClearDisplay();//������� �� ���� ������ ����� 
modes=4;
}
void password_3 (void)
{
Show(1,"������� ������:");//���� ������ �����
Show(21,"**"); 
Show(24,"--"); 
ShowDigitZ(23,1,P3);
}
void P_3_Plus (void)
{
   if (P3==9)
      {P3=0;}
       else
      {P3++;}
}
void P_3_Minus (void)
{
   if (P3==0)
      {P3=9;}
       else
      {P3--;}
}
void wwod_P3 (void)
{
ClearDisplay();//������� �� ���� �������� ����� 
modes=5;
}
void password_4 (void)
{
Show(1,"������� ������:");//���� �������� ����� 
Show(21,"***"); 
Show(25,"-"); 
ShowDigitZ(24,1,P4);
}
void P_4_Plus (void)
{
   if (P4==9)
      {P4=0;}
       else
      {P4++;}
}
void P_4_Minus (void)
{
   if (P4==0)
      {P4=9;}
       else
      {P4--;}
}
void wwod_P4 (void)//������� �� ���� ����� ����� 
{
ClearDisplay();
modes=6;
}
void password_5 (void)//���� ����� ����� 
{
Show(1,"������� ������:"); 
Show(21,"****");  
ShowDigitZ(25,1,P5);
}
void P_5_Plus (void)
{
   if (P5==9)
      {P5=0;}
       else
      {P5++;}
}
void P_5_Minus (void)
{
   if (P5==0)
      {P5=9;}
       else
      {P5--;}
}
void wwod_P5 (void)
{
ClearDisplay();//������� �� �������� ������
modes=7;
}
void Proverka_password (void)
{
ClearDisplay();
if ((P1==3)&&(P2==2)&&(P3==4)&&(P4==6)&&(P5==3))
    {
      P1=0;
      P2=0;
      P3=0;
      P4=0;
      P5=0;
      ClearDisplay();
      modes=9;
    }
else
    {
      P1=0;
      P2=0;
      P3=0;
      P4=0;
      P5=0;
      ClearDisplay();
      modes=8;
    }
} 
void error_password (void)
{
  Show(4,"������!"); 
  Show(16,"�������� ������!"); 
}
//------------------------------------------------------------------------------
//--------------------------------���� � �������� ������------------------------
void Wxod_dannie (void)
{
ClearDisplay();
modes=17;  
}
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void Menu_1  (void)//������ ���������� ���? 
{
 Show(1,"�����.");
 Show(8,"�����."); 
 Show(24,"���.?"); 
 Show(29,"("); 
 Show(31,")"); 
 ShowChar(30,0x7E);
 ShowDigitZ(16,4,nsSpeed::S);
 Show(21,"mm"); 
 reset_auto_repeat();//���� ����������
}
void Wxod_dlina (void)// ���� � ��������� ����������
{
ClearDisplay();
modes=16;  
}

void Perexod_menu2 (void)//������� �� ���� ������������  ���
{
ClearDisplay();
modes=10;
}
void Menu_2 (void)//����������� ��� ���?
{ reset_auto_repeat();//���� ����������
  Show(0,"����������� ���."); 
  Show(24,"���.?"); 
  Show(29,"("); 
  Show(31,")"); 
  ShowChar(30,0x7E);
  ShowDigitZ(16,1,nsTok::kAcpToTok/1000); 
  Show(17,",");
  ShowDigitZ(18,3,nsTok::kAcpToTok);
}
void Perexod_exit_menu3 (void)
{
ClearDisplay();
modes=13;// ������� �� �����, ����� ��������� �������������
}
void Perexod_exit_menu4 (void)
{
ClearDisplay();
modes=12;  
}
void Menu_3 (void)//��������� ���� ��� ���
{reset_auto_repeat();//���� ����������
Show(0,"���������� ����."); 
Show(24,"���.?"); 
Show(29,"("); 
Show(31,")"); 
ShowChar(30,0x7E);
ShowDigitZ(16,3,nsAcp::Korecsia_zero); 
}
void Menu_4 (void)//��������� �������� ����
{reset_auto_repeat();//���� ����������
Show(1,"�������� ����."); 
Show(24,"���.?"); 
Show(29,"("); 
Show(31,")"); 
ShowChar(30,0x7E);
ShowDigitZ(16,4,nsAcp::Smehenie_zero); 
}
void Wxod_Smehenia (void)//���� �������� ����
{
ClearDisplay();
modes=18; 
}
void Perexod_exit_menu5 (void)
{
ClearDisplay();
modes=13;

}
void Menu_5 (void)//����� �� ���� ������ ������ � EPROM
{
Show(2,"����� �� ����"); 
Show(19,"�������!");
Show(27,"(");
ShowChar(28,0x7E);
Show (29,")");
}
void Perexod_menu_1 (void)//������ �� ������ ���������� ���? 
{
ClearDisplay();
modes=9;  
}

void Save_dannie_Exit_Indicasia (void)
{
//��������� ������ � ���
ClearDisplay();
modes=0;  
}
//----------------------------���� ������---------------------------------------
void Wxod_instal_zero (void)//���� �� ���� ���� ��� ���
{
ClearDisplay();
modes=15;  
}
//-----------------------���� ������ ����������---------------------------------
void Wwod_dlina (void)
{
Show(1,"�����.");
Show(8,"�����."); 
Show(24,"���.?"); 
Show(29,"("); 
Show(31,")"); 
ShowChar(30,0x7E);
ShowDigitZ(16,4,nsSpeed::S); 
Show(21,"mm"); 
}
void Wwod_dlina_min (void)
{
set_auto_repeat();//��� ����������
if ( nsSpeed::S<=0 ) nsSpeed::S=0;
else nsSpeed::S--;
}
void Wwod_dlina_plu (void)
{
set_auto_repeat();//��� ����������
if ( nsSpeed::S>=2000 ) nsSpeed::S=2000;
else nsSpeed::S++;
}
void Wwod_dlina_exit (void)
{      /*������ �� ����� ������ ����������*/
    WriteEeprom (15,((unsigned char *)&nsSpeed::S)[0]);// 1 ����
    WriteEeprom (16,((unsigned char *)&nsSpeed::S)[1]);// 2 ����
ClearDisplay();
modes=9;  
}
//----------------------------------����� ����� ������ �����������----------------------------------------
//---------------------------------------------------------------------------------------------------
void Wxod_ACP (void)//���� �� ���� ������������ ���
{
ClearDisplay();
modes=14;  
}
//--------------------------------------���� ������������ ��� ���----------------------------
void Wwod_ACP (void)
{
Show(0,"����������� ���."); 
  Show(24,"���.?"); 
  Show(29,"("); 
  Show(31,")"); 
  ShowChar(30,0x7E);
  ShowDigitZ(16, 1, nsTok::kAcpToTok/1000); 
  ShowChar(17, ',');
  ShowDigitZ(18, 3, nsTok::kAcpToTok);
}
void Wwod_ACP_min (void)
{
    set_auto_repeat();//��� ����������
    if (nsTok::kAcpToTok>1)
        nsTok::kAcpToTok--;
}
void Wwod_ACP_plu(void)
{
    set_auto_repeat();//��� ����������
    if (nsTok::kAcpToTok<2000)
        nsTok::kAcpToTok++;
    else
        nsTok::kAcpToTok = 2000;
}
void Wwod_ACP_exit (void)
{       // ������ �� ����� ������������ ���
    nsTok::WriteKacpToTok();
    /*
    WriteEeprom (19,((unsigned char *)&nsAcp::ACP)[0]);// 1 ����
    WriteEeprom (20,((unsigned char *)&nsAcp::ACP)[1]);// 2 ����
    WriteEeprom (21,((unsigned char *)&nsAcp::ACP)[2]);// 3 ����
    WriteEeprom (22,((unsigned char *)&nsAcp::ACP)[3]);// 4 ����
    */
    ClearDisplay();
    modes=10;  
}
//----------------------------����� ����� ������ ���--------------------------------
//-------------------------���� ��������� ���� ��� ���----------------------------
void Wwod_zero (void)
{
Show(0,"���������� ����."); 
Show(24,"���.?"); 
Show(29,"("); 
Show(31,")"); 
ShowChar(30,0x7E);
ShowDigitZ(16,3,nsAcp::Korecsia_zero); 
}
void Wwod_zero_min (void)
{
set_auto_repeat();//��� ����������
if ( nsAcp::Korecsia_zero<=0 ) nsAcp::Korecsia_zero=0;
else nsAcp::Korecsia_zero--;
}
void Wwod_zero_plu (void)
{
set_auto_repeat();//��� ����������
if ( nsAcp::Korecsia_zero>=512 ) nsAcp::Korecsia_zero=512;
else nsAcp::Korecsia_zero++;
}
void Wwod_zero_exit (void)
{
      /*������ �� ����� ������������ ��������� ����*/
    WriteEeprom (23,((unsigned char *)&nsAcp::Korecsia_zero)[0]);// 1 ����
    WriteEeprom (24,((unsigned char *)&nsAcp::Korecsia_zero)[1]);// 2 ����
    WriteEeprom (25,((unsigned char *)&nsAcp::Korecsia_zero)[2]);// 3 ����
    WriteEeprom (26,((unsigned char *)&nsAcp::Korecsia_zero)[3]);// 4 ����
ClearDisplay();
modes=11;  
}
//-----------------------------------����� ����� ��������� ����---------------------
//-----------------------------------���� �������� ����----------------------------
void Wwod_Smehenie (void)
{
Show(1,"�������� ����."); 
Show(24,"COX.?"); 
Show(29,"("); 
Show(31,")"); 
ShowChar(30,0x7E);
ShowDigitZ(16,4,nsAcp::Smehenie_zero); 
}

void Wwod_Smehenie_plu (void)
{
set_auto_repeat();//��� ����������
if ( nsAcp::Smehenie_zero>=1000 ) nsAcp::Smehenie_zero=1000;
else nsAcp::Smehenie_zero++;
}

void Wwod_Smehenie_min (void)
{
set_auto_repeat();//��� ����������
if ( nsAcp::Smehenie_zero<=0 ) nsAcp::Smehenie_zero=0;
else nsAcp::Smehenie_zero--;
}
void Wwod_Smehenie_exit (void)
{
            /*������ �� ����� �������� ����*/
    WriteEeprom (27,((unsigned char *)&nsAcp::Smehenie_zero)[0]);// 1 ����
    WriteEeprom (28,((unsigned char *)&nsAcp::Smehenie_zero)[1]);// 2 ����
    WriteEeprom (29,((unsigned char *)&nsAcp::Smehenie_zero)[2]);// 3 ����
    WriteEeprom (30,((unsigned char *)&nsAcp::Smehenie_zero)[3]);// 4 ����
ClearDisplay();
modes=12;  
}
//-----------------------------------����� ����� �������� ����--------------------
void Wwod_dannie (void)
{
Show(0,"S:"); 
__disable_interrupt();//��������� ���������� ����������
ShowDigitZ(2,5,nsSpeed::Buf_impuls); 
__enable_interrupt(); // ��������� ���������� ���������� 
Show(19,"��/���"); 
__disable_interrupt();//��������� ���������� ����������
ShowDigitZ(16,3,nsSpeed::rezultat_skorosti);//��/���
__enable_interrupt(); // ��������� ���������� ���������� 
Show(8,"���:");
__disable_interrupt();//��������� ���������� ����������
ShowDigitZ(12,4,nsTok::acp_date);//������ � ���
__enable_interrupt(); // ��������� ���������� ���������� 
//---------------------����� �����������------------------
  ShowDigitZ (26,4,Pirometr);
//--------------------------------------------------------
ShowChar (30,7);
Show (31,"C");
/*-------------------------------------------------------*/


}


void Exit_dannie (void)
{
ClearDisplay();
modes=0;  
}







//----------------------------����� ����� ��������� ���� �� ���--------------------------------
void (* const __flash Function[][6])(void) = {
{Indicasia,Perexod_w_menu ,Dummy,Wxod_dannie ,Dummy,Dummy },                                                                
{Menu_0,Perexod_w_indicasia,Dummy,Dummy,vvod_pasword,Dummy},
{password_1,Perexod_w_indicasia,P_1_Minus,P_1_Plus,wwod_P1,Dummy },
{password_2,Perexod_w_indicasia,P_2_Minus,P_2_Plus,wwod_P2,Dummy },
{password_3,Perexod_w_indicasia,P_3_Minus,P_3_Plus,wwod_P3,Dummy },
{password_4,Perexod_w_indicasia,P_4_Minus,P_4_Plus,wwod_P4,Dummy },
{password_5,Perexod_w_indicasia,P_5_Minus,P_5_Plus,wwod_P5,Dummy },
{Proverka_password, Dummy,Dummy, Dummy ,Dummy,Dummy },
{error_password,Perexod_w_indicasia,Perexod_w_indicasia ,Perexod_w_indicasia,Perexod_w_indicasia,Dummy},
{Menu_1,Perexod_menu2,Dummy,Dummy ,Wxod_dlina,Dummy },//������ ���������� ���?
{Menu_2,Perexod_exit_menu3,Dummy,Dummy ,Wxod_ACP,Dummy },//�����������  ��� ���?
{Menu_3,Perexod_exit_menu4,Dummy,Dummy ,Wxod_instal_zero,Dummy },//��������� ���� ��� ���
{Menu_4,Perexod_exit_menu5,Dummy,Dummy,Wxod_Smehenia,Dummy},//�������� ����
{Menu_5,Perexod_menu_1,Dummy,Dummy ,Save_dannie_Exit_Indicasia,Dummy },//����� �� ���� ������ ������ � ���
//---------------------------------------------------------------------------------------------------------
{Wwod_ACP,Dummy,Wwod_ACP_min,Wwod_ACP_plu ,Wwod_ACP_exit,Dummy },//���� ������������ ���

{Wwod_zero,Dummy,Wwod_zero_min,Wwod_zero_plu,Wwod_zero_exit,Dummy},//���� ��������� ���� ��� ���

{Wwod_dlina,Dummy,Wwod_dlina_min, Wwod_dlina_plu,Wwod_dlina_exit,Dummy},//���� ����� ����������

{Wwod_dannie,Dummy,Exit_dannie,Dummy,Dummy,Dummy},//�������� ������

{Wwod_Smehenie,Dummy,Wwod_Smehenie_min ,Wwod_Smehenie_plu,Wwod_Smehenie_exit,Dummy},//���� �������� ����

};
}

void menu()
{
    unsigned char key;
    if (scan_ready())
    {
        key=read_key();
        nsMenu::Function[nsMenu::modes][key]();
    }
    else
    { 
        nsMenu::Function[nsMenu::modes][0]();
    }
}

