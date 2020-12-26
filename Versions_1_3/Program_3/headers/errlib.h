#ifndef ERRLIB_H
#define ERRLIB_H

////////// errlib ////////////////////////////////////////////////////////////
// Автор    : Баталов Семен                                                 //
// Дата     : 08.11.2020                                                    //
// Описание : Здесь представлены инструменты для обработки исключительных   //
//            ситуаций в модулях для решения филиппинского кроссворда.      //
//////////////////////////////////////////////////////////////////////////////

#include <cstdint>
#include <exception>

namespace err
{
    ////////// class Exception ///////////////////////////////////////////////
    // Дочерний класс, унаследованный от "std::exception". Содежит в себе   //
    // код ошибки "errCode_" (0 - отсутствие ошибок).                       //
    //////////////////////////////////////////////////////////////////////////
    
    class Exception : public std::exception
    {
        private :
            
            int16_t errCode_;       // Код ошибки
            
        public :
            
            // (1) Конструктор (обнуляет поле)
            Exception();
            
            // (2) Конструктор (инициализирует поле)
            Exception(int16_t newErrCode);
	    
            // (3) Конструктор копирования
            Exception(const Exception& exeption) = default;
	    
            // (4) Перегрузка оператора присваивания
            Exception& operator=(const Exception& exeption) = default;
            
            // (5) Виртуальный метод возврата сообщения об ошибке
            virtual const char* what() const noexcept = 0;
            
            // (5) Возвращает код ошибки
            int16_t error() const;
            
            // (6) Деструктор
            virtual ~Exception() = default;
    };
    
    ////////// class PcsException ////////////////////////////////////////////
    // Дочерний класс, унаследованный от "Exception". Описывает             //
    // исключительные ситуации в блоке инструментов решения кроссворда.     //
    //////////////////////////////////////////////////////////////////////////
    
    class PcsException : public Exception
    {
        public :
            
            // (1) Конструктор (обнуляет поле)
            PcsException() = default;
            
            // (2) Конструктор (инициализирует поле)
            PcsException(int16_t newErrCode) : Exception(newErrCode) {};
            
            // (3) Конструктор копирования
            PcsException(const PcsException& exeption) = default;
            
            // (4) Перегрузка оператора присваивания
            PcsException& operator=(const PcsException& exeption) = default;
            
            // (5) Виртуальный метод возврата сообщения об ошибке
            virtual const char* what() const noexcept;
            
            // (6) Деструктор
            virtual ~PcsException() = default;
    };
    
    ////////// class RwcException ////////////////////////////////////////////
    // Дочерний класс, унаследованный от "Exception". Описывает             //
    // исключительные ситуации в блоке файлового ввода и вывода.            //
    //////////////////////////////////////////////////////////////////////////
    
    class RwcException : public Exception
    {
        public :
            
            // (1) Конструктор (обнуляет поле)
            RwcException() = default;
            
            // (2) Конструктор (инициализирует поле)
            RwcException(int16_t newErrCode) : Exception(newErrCode) {};
            
            // (3) Конструктор копирования
            RwcException(const RwcException& exeption) = default;
            
            // (4) Перегрузка оператора присваивания
            RwcException& operator=(const RwcException& exeption) = default;
            
            // (5) Виртуальный метод возврата сообщения об ошибке
            virtual const char* what() const noexcept;
            
            // (6) Деструктор
            virtual ~RwcException() = default;
    };
}


#endif