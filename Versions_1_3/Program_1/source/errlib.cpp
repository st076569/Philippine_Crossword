////////// errlib ////////////////////////////////////////////////////////////
// Автор    : Баталов Семен                                                 //
// Дата     : 08.11.2020                                                    //
// Описание : Здесь представлены инструменты для обработки исключительных   //
//            ситуаций в модулях для решения филиппинского кроссворда.      //
//////////////////////////////////////////////////////////////////////////////

#include "errlib.h"

////////// class Exception ///////////////////////////////////////////////////
// Описание : errlib.h                                                      //
//////////////////////////////////////////////////////////////////////////////

// (1) Конструктор (обнуляет поле)
err::Exception::Exception()
{
    errCode_ = 0;
}

// (2) Конструктор (инициализирует поле)
err::Exception::Exception(const int16_t& newErrCode)
{
    errCode_ = newErrCode;
}

// (3) Конструктор копирования
err::Exception::Exception(const Exception& exception)
{
    *this = exception;
}

// (4) Перегрузка оператора присваивания
err::Exception& err::Exception::operator=(const err::Exception& exception)
{
    if (&exception != this)
    {
        errCode_ = exception.errCode_;
    }
    return *this;
}

// (5) Возвращает код ошибки
int16_t err::Exception::error() const
{
    return errCode_;
}