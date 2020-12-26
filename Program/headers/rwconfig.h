#ifndef RWCONFIG_H
#define RWCONFIG_H

////////// rwconfig //////////////////////////////////////////////////////////
// Автор    : Баталов Семен                                                 //
// Дата     : 08.11.2020                                                    //
// Описание : Здесь представлены инструменты для записи в текстовый файл    //
//            схемы филиппинского кроссворда и для ее чтения из него.       //
//////////////////////////////////////////////////////////////////////////////

#include "errlib.h"
#include "solvelib.h"
#include "instrumentslib.h"

#include <fstream>
#include <iomanip>
#include <string>

// Зарезервированные слова для чтения поля из файла
#define SIZE_COMMAND  "size:"
#define FIELD_COMMAND "field:"

// Вид сочленений в построенном пути
#define LEFT_IMAGE     "-+ "
#define RIGHT_IMAGE    " +-"
#define VERTICAL_IMAGE " | "
#define MIDDLE_IMAGE   "---"

namespace rwc
{
    ////////// void scanf ////////////////////////////////////////////////////
    // 1) bool isEqual : проверяет символы "ch1" и "ch2" на равенство с     //
    //    учетом регистра, если "reg = true", и без его учета в противном   //
    //    случае.                                                           //
    // 2) void scanf   : чтение целочисленной переменной из потока "inf".   //
    // 3) void scanf   : проверка строки из потока "inf" на равенство       //
    //    строке "name" с учетом или без учета регистра.                    //
    //////////////////////////////////////////////////////////////////////////
    
    bool isEqual(char ch1, char ch2, bool reg = true);
    
    void scanf(std::ifstream& inf, int16_t& value);
    
    void scanf(std::ifstream& inf, const char* name, bool reg = true);
    
    ////////// void scanf "Field" ////////////////////////////////////////////
    // Функция чтения класса "Field" из файлового потока ввода "inf".       //
    //////////////////////////////////////////////////////////////////////////
    
    void scanf(std::ifstream& inf, pcs::Field& field);
    
    ////////// void printf "Field" ///////////////////////////////////////////
    // Функция записи класса "Field" в файловый потока вывода "of".         //
    //////////////////////////////////////////////////////////////////////////
    
    void printf(std::ofstream& of, pcs::Field& field);
}

#endif