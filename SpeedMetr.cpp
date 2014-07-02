
#include "SpeedMetr.h"
#include "main.h"

//-------------------------------Переменные, флаги для скоростимера--------------------------------------------------------------------------------------
unsigned int rezultat_skorosti=0;//Результат скорости скорость об/мин 
unsigned int shethik_impulsow=0;//Cчётчик импульсов
unsigned char flag_old=0;//старое значение импульса
unsigned char flag_new=0;//новое значение импульса
unsigned char flag_reset_shet=0;//Флаг остановки измерения-Если 1 счёт разрешон 0-запрещён
