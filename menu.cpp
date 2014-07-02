
#include "menu.h"
#include "main.h"

//--------------------------------------------------------------------------------------------------------------------------------
extern unsigned char D_1;//Отфильтрованное состояние датчика. Если 0-датчик вкл. Если 1-датчик выкл.
int P1,P2,P3,P4,P5;//Переменные для ввода пароля
unsigned char modes=0;
//---------------------------------------------------------------------------------------------------------------------------------
//---------------------------Переменные, флаги, константы для скорости-------------------------------------------
extern unsigned int shethik_impulsow;//Cчётчик импульсов
extern unsigned int Buf_impuls;//Буфер импульсов для просмотра
unsigned int S=0;//Длинна окружности
unsigned long skorost_stana=0;//скорость стана метры в минуту
unsigned long bufer_skorost=0;//хранение скорости стана метры в минуту 
extern unsigned char flag_reset_shet;//Флаг остановки измерения-Если 1 счёт разрешон 0-запрещён
//----------------------------Переменные, флаги, константы для АЦП---------------------------------------------
extern unsigned long ACP;//Коэфициент для АЦП (Расчёт нового коэффициента 1).(1204-Korecsia_zero)=rez 2).1000/rez=Новый коэффициент.Расчёт смещения нуля:
extern unsigned long acp_date;//Голые данные с ацп
extern unsigned long Rezult_ADC;//Переменная для вывода значения с АЦП на индикатор
//---------------------------------------------------------------------------------------------
//------------------------Переменные, флаги, константы для Пирометра---------------------------
unsigned int  Pirometr;
//---------------------------------------------------------------------------------------------
void Dummy (void)
{}
void Indicasia (void)//Основная индикация
{
//-----------------вывод температуры-------------------------------------
Show (23,"T="); 
ShowDigitZ (25,4,Pirometr);
Show (31,"C");
ShowChar (30,7);
//-----------------------------------------------------------------------
//индикация работы индуктивного датчика
Show (13,"Д1"); 
if (D_1==0)
{ShowChar (15,4);}
if (D_1==1) 
{ShowChar (15,3);} 
//-----------------------------Скорость стана-----------------------------------
__disable_interrupt();//Запретить глобальное прерывание
bufer_skorost=nsSpeed::rezultat_skorosti;//об.мин 
__enable_interrupt(); // Разрешить глобальное прерывание 
skorost_stana=(S*bufer_skorost/10);//об.мин * длинну окружности (метры в минуту)
ShowDigitZ (4,2,skorost_stana);//Вывод скорости стана M/МИН
Show (3,",");
ShowDigitZ (0,3,skorost_stana/100);
Show (7,"М/МИН");
//------------------------------------------------------------------------------
//---------------------------------АЦП------------------------------------------
__disable_interrupt();//Запретить глобальное прерывание
ACP_DANNIE=Rezult_ADC;
__enable_interrupt(); // Разрешить глобальное прерывание 
Show (20,"A");
//--------------------------Вывод данных с АЦП----------------------------------
ACP_DANNIE=(ACP_DANNIE-0)*ACP/1000;
ACP_DANNIE_RS=ACP_DANNIE+0;
ShowDigitZ (16,3,(ACP_DANNIE+0));//Вывод данных с АЦП
//----------------------------------------------------------------------------------
}
void Perexod_w_indicasia (void)//Возврат из начального меню "Настройка оборудования"
{
P1=0;//Обнуление первой цифры пароля
P2=0;//Обнуление второй цифры пароля
P3=0;//Обнуление третей цифры пароля 
P4=0;//Обнуление четвёртой цифры пароля 
P5=0;//Обнуление пятой цифры пароля
ClearDisplay();
modes=0;
}
void Perexod_w_menu (void)//Вход в меню "Настройка оборудования"
{
P1=0;//Обнуление первой цифры пароля
P2=0;//Обнуление второй цифры пароля
P3=0;//Обнуление третей цифры пароля 
P4=0;//Обнуление четвёртой цифры пароля 
P5=0;//Обнуление пятой цифры пароля
modes=1;
ClearDisplay();
}
void Menu_0 (void)//Меню нулевой уровень
{
Show (0,"НАЛАДОЧНОЕ МЕНЮ!");
Show (16,"ВЫХОД(М)");
ShowChar (30,0x7E);
Show (25,"ВХОД(");
Show (31,")");
}
void vvod_pasword (void)//Вход на ввод пароля
{
modes=2;
ClearDisplay();
}
//------------------------------ВВОД ПАРОЛЯ-------------------------------------
//------------------------------------------------------------------------------
void password_1 (void)//Ввод первой цифры
{
Show(1,"ВВЕДИТЕ ПАРОЛЬ:"); 
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
void wwod_P1 (void)//Переход на ввод второй цифры 
{
ClearDisplay();// Очистка ЖКИ
modes=3;
}
void password_2 (void)//Ввод второй цифры
{
Show(1,"ВВЕДИТЕ ПАРОЛЬ:"); 
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
ClearDisplay();//Переход на ввод третей цифры 
modes=4;
}
void password_3 (void)
{
Show(1,"ВВЕДИТЕ ПАРОЛЬ:");//Ввод третей цифры
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
ClearDisplay();//Переход на ввод четвёртой цифры 
modes=5;
}
void password_4 (void)
{
Show(1,"ВВЕДИТЕ ПАРОЛЬ:");//Ввод четвёртой цифры 
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
void wwod_P4 (void)//Переход на ввод пятой цифры 
{
ClearDisplay();
modes=6;
}
void password_5 (void)//Ввод пятой цифры 
{
Show(1,"ВВЕДИТЕ ПАРОЛЬ:"); 
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
ClearDisplay();//Переход на проверку пароля
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
  Show(4,"ОШИБКА!"); 
  Show(16,"НЕВЕРНЫЙ ПАРОЛЬ!"); 
}
//------------------------------------------------------------------------------
//--------------------------------Вход в просмотр данных------------------------
void Wxod_dannie (void)
{
ClearDisplay();
modes=17;  
}
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void Menu_1  (void)//Длинна окружности изм? 
{
 Show(1,"КОЭФФ.");
 Show(8,"ДЛИНЫ."); 
 Show(24,"ИЗМ.?"); 
 Show(29,"("); 
 Show(31,")"); 
 ShowChar(30,0x7E);
 ShowDigitZ(16,4,S);
 Show(21,"mm"); 
 reset_auto_repeat();//Выкл автоповтор
}
void Wxod_dlina (void)// Вход в коррекцию окружности
{
ClearDisplay();
modes=16;  
}

void Perexod_menu2 (void)//переход на ввод коэффициента  АЦП
{
ClearDisplay();
modes=10;
}
void Menu_2 (void)//Коэффициент АЦП изм?
{ reset_auto_repeat();//Выкл автоповтор
  Show(0,"КОЭФФИЦИЕНТ АЦП."); 
  Show(24,"ИЗМ.?"); 
  Show(29,"("); 
  Show(31,")"); 
  ShowChar(30,0x7E);
  ShowDigitZ(16,1,ACP/1000); 
  Show(17,",");
  ShowDigitZ(18,3,ACP);
}
void Perexod_exit_menu3 (void)
{
ClearDisplay();
modes=13;// переход на выход, обход остальных коээфициентов
}
void Perexod_exit_menu4 (void)
{
ClearDisplay();
modes=12;  
}
void Menu_3 (void)//Установка нуля для АЦП
{reset_auto_repeat();//Выкл автоповтор
Show(0,"КАЛИБРОВКА НУЛЯ."); 
Show(24,"ИЗМ.?"); 
Show(29,"("); 
Show(31,")"); 
ShowChar(30,0x7E);
ShowDigitZ(16,3,Korecsia_zero); 
}
void Menu_4 (void)//Установка смещения нуля
{reset_auto_repeat();//Выкл автоповтор
Show(1,"СМЕЩЕНИЕ НУЛЯ."); 
Show(24,"ИЗМ.?"); 
Show(29,"("); 
Show(31,")"); 
ShowChar(30,0x7E);
ShowDigitZ(16,4,Smehenie_zero); 
}
void Wxod_Smehenia (void)//вход смещения нуля
{
ClearDisplay();
modes=18; 
}
void Perexod_exit_menu5 (void)
{
ClearDisplay();
modes=13;

}
void Menu_5 (void)//Выход из меню запись данных в EPROM
{
Show(2,"ВЫХОД ИЗ МЕНЮ"); 
Show(19,"НАЛАДКИ!");
Show(27,"(");
ShowChar(28,0x7E);
Show (29,")");
}
void Perexod_menu_1 (void)//Повтор на Длинна окружности изм? 
{
ClearDisplay();
modes=9;  
}

void Save_dannie_Exit_Indicasia (void)
{
//Сохранить данные в пзу
ClearDisplay();
modes=0;  
}
//----------------------------Ввод данных---------------------------------------
void Wxod_instal_zero (void)//Вход на ввод нуля для АЦП
{
ClearDisplay();
modes=15;  
}
//-----------------------Ввод длинны окружности---------------------------------
void Wwod_dlina (void)
{
Show(1,"КОЭФФ.");
Show(8,"ДЛИНЫ."); 
Show(24,"СОХ.?"); 
Show(29,"("); 
Show(31,")"); 
ShowChar(30,0x7E);
ShowDigitZ(16,4,S); 
Show(21,"mm"); 
}
void Wwod_dlina_min (void)
{
set_auto_repeat();//Вкл автоповтор
if ( S<=0 ) S=0;
else S--;
}
void Wwod_dlina_plu (void)
{
set_auto_repeat();//Вкл автоповтор
if ( S>=2000 ) S=2000;
else S++;
}
void Wwod_dlina_exit (void)
{      /*Запись во флешь длинны оркужности*/
WriteEeprom (15,((unsigned char *)&S)[0]);// 1 байт
WriteEeprom (16,((unsigned char *)&S)[1]);// 2 байт
ClearDisplay();
modes=9;  
}
//----------------------------------Конец ввода длинны окружностьи----------------------------------------
//---------------------------------------------------------------------------------------------------
void Wxod_ACP (void)//Вход на ввод коэффициента АЦП
{
ClearDisplay();
modes=14;  
}
//--------------------------------------Ввод коэффициента для АЦП----------------------------
void Wwod_ACP (void)
{
Show(0,"КОЭФФИЦИЕНТ АЦП."); 
  Show(24,"СОХ.?"); 
  Show(29,"("); 
  Show(31,")"); 
  ShowChar(30,0x7E);
  ShowDigitZ(16,1,ACP/1000); 
  Show(17,",");
  ShowDigitZ(18,3,ACP);
}
void Wwod_ACP_min (void)
{
set_auto_repeat();//Вкл автоповтор
if ( ACP<=0 ) ACP=0;
else ACP--;
}
void Wwod_ACP_plu (void)
{
set_auto_repeat();//Вкл автоповтор
if ( ACP>=2000 ) ACP=2000;
else ACP++; 
}
void Wwod_ACP_exit (void)
{       /*Запись во флешь коэффициента АЦП*/
WriteEeprom (19,((unsigned char *)&ACP)[0]);// 1 байт
WriteEeprom (20,((unsigned char *)&ACP)[1]);// 2 байт
WriteEeprom (21,((unsigned char *)&ACP)[2]);// 3 байт
WriteEeprom (22,((unsigned char *)&ACP)[3]);// 4 байт
ClearDisplay();
modes=10;  
}
//----------------------------Конец ввода данных АЦП--------------------------------
//-------------------------Ввод коррекции нуля для АЦП----------------------------
void Wwod_zero (void)
{
Show(0,"КАЛИБРОВКА НУЛЯ."); 
Show(24,"СОХ.?"); 
Show(29,"("); 
Show(31,")"); 
ShowChar(30,0x7E);
ShowDigitZ(16,3,Korecsia_zero); 
}
void Wwod_zero_min (void)
{
set_auto_repeat();//Вкл автоповтор
if ( Korecsia_zero<=0 ) Korecsia_zero=0;
else Korecsia_zero--;
}
void Wwod_zero_plu (void)
{
set_auto_repeat();//Вкл автоповтор
if ( Korecsia_zero>=512 ) Korecsia_zero=512;
else Korecsia_zero++;
}
void Wwod_zero_exit (void)
{
      /*Запись во флешь коеффициента коррекции нуля*/
WriteEeprom (23,((unsigned char *)&Korecsia_zero)[0]);// 1 байт
WriteEeprom (24,((unsigned char *)&Korecsia_zero)[1]);// 2 байт
WriteEeprom (25,((unsigned char *)&Korecsia_zero)[2]);// 3 байт
WriteEeprom (26,((unsigned char *)&Korecsia_zero)[3]);// 4 байт
ClearDisplay();
modes=11;  
}
//-----------------------------------Конец ввода коррекции нуля---------------------
//-----------------------------------Ввод смещения нуля----------------------------
void Wwod_Smehenie (void)
{
Show(1,"СМЕЩЕНИЕ НУЛЯ."); 
Show(24,"COX.?"); 
Show(29,"("); 
Show(31,")"); 
ShowChar(30,0x7E);
ShowDigitZ(16,4,Smehenie_zero); 
}

void Wwod_Smehenie_plu (void)
{
set_auto_repeat();//Вкл автоповтор
if ( Smehenie_zero>=1000 ) Smehenie_zero=1000;
else Smehenie_zero++;
}

void Wwod_Smehenie_min (void)
{
set_auto_repeat();//Вкл автоповтор
if ( Smehenie_zero<=0 ) Smehenie_zero=0;
else Smehenie_zero--;
}
void Wwod_Smehenie_exit (void)
{
            /*Запись во флешь смещение нуля*/
WriteEeprom (27,((unsigned char *)&Smehenie_zero)[0]);// 1 байт
WriteEeprom (28,((unsigned char *)&Smehenie_zero)[1]);// 2 байт
WriteEeprom (29,((unsigned char *)&Smehenie_zero)[2]);// 3 байт
WriteEeprom (30,((unsigned char *)&Smehenie_zero)[3]);// 4 байт
ClearDisplay();
modes=12;  
}
//-----------------------------------Конец ввода смещения нуля--------------------
void Wwod_dannie (void)
{
Show(0,"S:"); 
__disable_interrupt();//Запретить глобальное прерывание
ShowDigitZ(2,5,Buf_impuls); 
__enable_interrupt(); // Разрешить глобальное прерывание 
Show(19,"об/мин"); 
__disable_interrupt();//Запретить глобальное прерывание
ShowDigitZ(16,3,nsSpeed::rezultat_skorosti);//об/мин
__enable_interrupt(); // Разрешить глобальное прерывание 
Show(8,"АЦП:");
__disable_interrupt();//Запретить глобальное прерывание
ShowDigitZ(12,4,acp_date);//Данные с АЦП
__enable_interrupt(); // Разрешить глобальное прерывание 
//---------------------вывод температуры------------------
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







//----------------------------Конец ввода коррекции нуля дл АЦП--------------------------------
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
{Menu_1,Perexod_menu2,Dummy,Dummy ,Wxod_dlina,Dummy },//Длинна окружности изм?
{Menu_2,Perexod_exit_menu3,Dummy,Dummy ,Wxod_ACP,Dummy },//Коэффициент  АЦП изм?
{Menu_3,Perexod_exit_menu4,Dummy,Dummy ,Wxod_instal_zero,Dummy },//Установка нуля для АЦП
{Menu_4,Perexod_exit_menu5,Dummy,Dummy,Wxod_Smehenia,Dummy},//Смещение нуля
{Menu_5,Perexod_menu_1,Dummy,Dummy ,Save_dannie_Exit_Indicasia,Dummy },//Выход из меню запись данных в ПЗУ
//---------------------------------------------------------------------------------------------------------
{Wwod_ACP,Dummy,Wwod_ACP_min,Wwod_ACP_plu ,Wwod_ACP_exit,Dummy },//ввод коэффициента АЦП

{Wwod_zero,Dummy,Wwod_zero_min,Wwod_zero_plu,Wwod_zero_exit,Dummy},//ввод коррекции нуля для АЦП

{Wwod_dlina,Dummy,Wwod_dlina_min, Wwod_dlina_plu,Wwod_dlina_exit,Dummy},//ввод длины окружности

{Wwod_dannie,Dummy,Exit_dannie,Dummy,Dummy,Dummy},//Просмотр данных

{Wwod_Smehenie,Dummy,Wwod_Smehenie_min ,Wwod_Smehenie_plu,Wwod_Smehenie_exit,Dummy},//Ввод смещение нуля

};





















void menu(void)
   
{ 
   unsigned char key;
  if ( scan_ready())
       {
        key=read_key();
       Function[modes][key]();
        }
  else
        { 
        Function[modes][0]();
        }
}