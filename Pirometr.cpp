
#include "Pirometr.h"
#include "main.h"

unsigned char ks[10];
unsigned char InRs485[20];//Буфер для анализа принятой посылки
extern unsigned int  Pirometr;//Хранение данных с пирометра//имя переменной может быть любой
/* ========================================================= */
// запрос температуры из перометра "Термоскоп-600-1С" (rs485)
/* ========================================================= */
void termoskop(void)
{    
//--------- ":" -------
    WriteCom2Byte(0x3a);
//---adress device---
    WriteCom2Byte(0x30);
    WriteCom2Byte(0x31);
//-------function-------------
    WriteCom2Byte(0x30);
    WriteCom2Byte(0x34);
//---adress 1 reg--------------
    WriteCom2Byte(0x30);
    WriteCom2Byte(0x31);
    WriteCom2Byte(0x30);
    WriteCom2Byte(0x33);//0x30
//----------kol reg ----------//
    WriteCom2Byte(0x30);
    WriteCom2Byte(0x30);
    WriteCom2Byte(0x30);
    WriteCom2Byte(0x31);
//-------------ks-------------//
    WriteCom2Byte(0x46);
    WriteCom2Byte(0x36);//0x39
//-----------konec-------------//
    WriteCom2Byte(0x0D);
    WriteCom2Byte(0x0A);
//--------Конец запроса---------//
}
/* ========================================================= */
// приём температуры "Термоскоп-600-1С" (rs485)
/* ========================================================= */
void in_rs485(void)
{
unsigned char rxd;
unsigned char st; 
unsigned char mt;     
          if(ReadCom2Byte(&rxd)==1)
          { 
            InRs485[14]=InRs485[13]; // ":"
            InRs485[13]=InRs485[12]; // "0"
            InRs485[12]=InRs485[11]; // "1"
            InRs485[11]=InRs485[10]; // "0"
            InRs485[10]=InRs485[9];  // "4"
            InRs485[9]=InRs485[8];   // "0"
            InRs485[8]=InRs485[7];   // "2"
            InRs485[7]=InRs485[6];   //  t
            InRs485[6]=InRs485[5];   //  t
            InRs485[5]=InRs485[4];   //  t
            InRs485[4]=InRs485[3];   //  t
            InRs485[3]=InRs485[2];   // ks
            InRs485[2]=InRs485[1];   // ks
            InRs485[1]=InRs485[0];   // 0d
            InRs485[0]= rxd;         // 0a
              if ((InRs485[14]==0x3A)&&(InRs485[1]==0x0d)&&(InRs485[0]==0x0a))
             {
              //--------------------
               st = InRs485[7]-0x30;
               if(st > 9) st=st-7;
               st <<=4;
               mt = InRs485[6]-0x30;
               if(mt > 9) mt=mt-7;
               HIGH(Pirometr) = st | mt;          
               //--------------------
               st = InRs485[5]-0x30;
               if(st > 9) st=st-7;
               st <<=4;
               mt = InRs485[4]-0x30;
               if(mt > 9) mt=mt-7;
               LOW(Pirometr) = st | mt; 
               //------------------------
             }
          }
            //else {Pirometr=5000;}//если контрольная сумма несовпадает в перменную Pirometr записывается число-5000 -это значит вывод "????"
         // }
        //else  //Если ответ с пирометра не пришёл 
        //{
       //Pirometr=0;//Код ошибки - обрыв связи.Вывод на ЖКИ "обрыв"
       //} 
   
}










