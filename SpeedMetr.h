
#ifndef SpeedMetr__h
#define SpeedMetr__h

namespace nsSpeed
{
extern unsigned int rezultat_skorosti;  // Результат скорости скорость об/мин 
extern unsigned int shethik_impulsow;   // Cчётчик импульсов
extern unsigned char flag_old;          // старое значение импульса
extern unsigned char flag_new;          // новое значение импульса
extern unsigned char flag_reset_shet;   // Флаг остановки измерения-Если 1 счёт разрешон 0-запрещён
}

#endif

