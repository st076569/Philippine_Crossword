#ifndef INSTRUMENTSLIB_H
#define INSTRUMENTSLIB_H

////////// instrumentslib ////////////////////////////////////////////////////
// Автор    : Баталов Семен                                                 //
// Дата     : 08.11.2020                                                    //
// Описание : Здесь представлены инструменты для решения филиппинского      //
//            кроссворда.                                                   //
//////////////////////////////////////////////////////////////////////////////

#include "errlib.h"

#include <iostream>
#include <vector>
#include <cmath>

// Символ индикатора
#define INDICATE_SYMBOL '#'

namespace pcs
{
    // Максимальный и минимальный размеры поля кроссворда по "X"
    const int16_t MAX_SIZE_X = 100;
    const int16_t MIN_SIZE_X = 5;
    
    // Максимальный и минимальный размеры поля кроссворда по "Y"
    const int16_t MAX_SIZE_Y = 100;
    const int16_t MIN_SIZE_Y = 5;
    
    // Максимальное и минимальное возможное число в клетке
    const int16_t MAX_INDEX  = 255;
    const int16_t MIN_INDEX  = 0;
    
    // Максимальное ограничивающее количество шагов при построении пути 
    const int16_t PATH_CYCLES_NUMBER = 10000;
    
    // Максимальное ограничивающее количество повторов поиска пути
    const int16_t PATH_REPLAY_NUMBER = 2;
    
    // Максимальное ограничивающее количество проходов при построении путей
    const int16_t MAIN_REPLAY_NUMBER = 8000;
    const int16_t INDICATOR_NUMBER   = 100;
    
    // Перечисление направлений, нужных для ориентирования путей на поле
    enum Direction {LEFT, RIGHT, VERTICAL, MIDDLE, NONE};
    
    ////////// struct Vector /////////////////////////////////////////////////
    // Эта структура используется для представления местоположения точек    //
    // (клеток) на поле, для более удобной адресации в поле ячеек.          //
    //////////////////////////////////////////////////////////////////////////
    
    struct Vector
    {
        public :
            
            int16_t x;      // Абсцисса точки
            int16_t y;      // Ордината точки
            
        public :
            
            // (1) Конструктор (обнуляет поля)
            Vector();
            
            // (2) Конструктор копирования
            Vector(const Vector& vector) = default;
            
            // (3) Конструктор от координат
            Vector(int16_t newX, int16_t newY);
            
            // (4) Перегрузка оператора присваивания
            Vector& operator=(const Vector& vector) = default;
            
            // (5) Перегрузка оператора равенства
            bool operator==(Vector right) const;
            
            // (6) Перегрузка оператора неравенства
            bool operator!=(Vector right) const;
            
            // (7) Обнуляет поля
            void clear();
            
            // (8) Деструктор
            ~Vector() = default;
    };
    
    ////////// distance //////////////////////////////////////////////////////
    // Эта функция возвращает расстояние между точками "p1", "p2". В смысле //
    // кол-ва задействованных клеток на поле.                               //
    //////////////////////////////////////////////////////////////////////////
    
    int16_t distance(Vector p1, Vector p2);
    
    ////////// bool inRangeSizeX/SizeY/Index /////////////////////////////////
    // Возвращают "true", если значение аргумента не выходит за границы.    //
    //////////////////////////////////////////////////////////////////////////
    
    bool inRangeIndex(int16_t index);
    
    bool inRangeSizeX(int16_t sizeX);
    
    bool inRangeSizeY(int16_t sizeY);
    
    ////////// max/min ///////////////////////////////////////////////////////
    // Эти процедуры возвращают соответственно максимум и минимум из двух   //
    // аргументов.                                                          //
    //////////////////////////////////////////////////////////////////////////
    
    int16_t max(int16_t var1, int16_t var2);
    
    int16_t min(int16_t var1, int16_t var2);
    
    ////////// createPermutation /////////////////////////////////////////////
    // Шаблонная процедура, которая производит случайную перестановку       //
    // элементов вектора "array".                                           //
    // Перегрузка с параметром "num" переставляет первые "num" элементов.   //
    //////////////////////////////////////////////////////////////////////////
    
    template <class T>
    void createPermutation(std::vector<T>& array, int16_t num)
    {
        // Если есть смысл производить перестановку
        if (num > 1 && num <= array.size())
        {
            T       temp;
            int16_t pos = 0;
            
            // Создаем случайную перестановку первых "num" элементов
            for (int16_t i = num - 1; i > 0; --i)
            {
                pos        = rand() % (i + 1);
                temp       = array[i];
                array[i]   = array[pos];
                array[pos] = temp;
            }
        }
    }
    
    template <class T>
    void createPermutation(std::vector<T>& array)
    {
        T       temp;
        int16_t pos = 0;
        
        // Создаем случайную перестановку всех элементов
        for (int16_t i = array.size() - 1; i > 0; --i)
        {
            pos        = rand() % (i + 1);
            temp       = array[i];
            array[i]   = array[pos];
            array[pos] = temp;
        }
    }
    
    ////////// struct Cell ///////////////////////////////////////////////////
    // Эта структура описывает ячейку поля кроссворда (число в ячейке есть  //
    // index).                                                              //
    //                                                                      //
    // Для пустой ячейки   : index = 0                                      //
    // Для непустой ячейки : 0 < index <= MAX_INDEX                         //
    //////////////////////////////////////////////////////////////////////////
    
    struct Cell
    {
        public :
            
            int16_t   index;    // Значение в данной клетке
            int16_t   id;       // ID пути, которым занята ячейка
            int16_t   num;      // Порядковый номер в пути
            int16_t   protoId;  // "id" пути с началом в этой клетке
            Direction way;      // Направление движения в клетке
            
        public :
            
            // (1) Конструктор (обнуляет поля)
            Cell();
            
            // (2) Конструктор копирования
            Cell(const Cell& cell) = default;
            
            // (3) Перегрузка оператора присваивания
            Cell& operator=(const Cell& cell) = default;
            
            // (4) Обнуляет поля
            void clear();
            
            // (5) Деструктор
            ~Cell() = default;
    };
    
    ////////// class Field ///////////////////////////////////////////////////
    // Класс, описывающий игровое поле кроссворда (содержит информацию о    //
    // всех ячейках).                                                       //
    //                                                                      //
    // В общем случае :                                                     //
    // 1) MIN_SIZE_X <= sizeX_ <= MAX_SIZE_X                                //
    // 2) MIN_SIZE_Y <= sizeY_ <= MAX_SIZE_Y                                //
    //////////////////////////////////////////////////////////////////////////
    
    class Field
    {
        private :
            
            Cell**  cells_;     // Массив клеток - поле кроссворда
            int16_t sizeX_;     // Размер поля по "X"
            int16_t sizeY_;     // Размер поля по "Y"
            
        public :
            
            // (1) Конструктор (обнуляет поля)
            Field();
            
            // (2) Конструктор копирования
            Field(const Field& field);
            
            // (3) Перегрузка оператора присваивания
            Field& operator=(const Field& field);
            
            // (4) Изменяет размер поля, уничтожая имеющуюся информацию
            void resize(int16_t sizeX, int16_t sizeY);
            
            // (5) Перегрузка оператора ()
            Cell& operator()(int16_t x, int16_t y);
            
            // (6) Перегрузка оператора ()
            const Cell& operator()(int16_t x, int16_t y) const;
            
            // (7) Перегрузка оператора () (через Vector)
            Cell& operator()(Vector point);
            
            // (8) Перегрузка оператора () (через Vector)
            const Cell& operator()(Vector point) const;
            
            // (9) Возвращает размер по "X"
            int16_t getSizeX() const;
            
            // (10) Возвращает размер по "Y"
            int16_t getSizeY() const;
            
            // (11) Проверяет поле на отсутствие индексов
            bool isEmpty() const;
            
            // (12) Проверяет соответствие размеров поля
            bool isCorrectSize() const;
            
            // (13) Проверяет корректность задания индексов
            bool isCorrectIndex() const;
            
            // (14) Оставляет индексы и обнуляет все остальное
            void refresh();
            
            // (15) "true" если не выходим за границу
            bool inRange(int16_t x, int16_t y) const;
            
            // (16) "true" если не выходим за границу (через Vector)
            bool inRange(Vector point) const;
            
            // (17) Освобождает выделенную память
            void clear();
            
            // (14) Деструктор
            ~Field();
    };
    
    ////////// class TrackSeq ////////////////////////////////////////////////
    // Объект класса генерирует в себе случайную последовательность сдвигов //
    // на поле относительно заданной точки (Влево, Вправо, Вверх, Вниз).    //
    //                                                                      //
    // 1) regenerate()   : сгенерировать случайную последовательность.      //
    // 2) getVector(...) : запросить очередной случайный сдвиг.             //
    // 3) haveFinished() : проверить, все ли возможные сдвиги использованы. //
    //////////////////////////////////////////////////////////////////////////
    
    class TrackSeq
    {
        private :
            
            std::vector<int16_t> seq_;      // Случайная последовательность
            int16_t              number_;   // Номер шага
            
        public :
            
            // (1) Конструктор (обнуляет поля)
            TrackSeq();
            
            // (2) Конструктор копирования
            TrackSeq(const TrackSeq& sequence) = default;
            
            // (3) Перегрузка оператора присваивания
            TrackSeq& operator=(const TrackSeq& sequence) = default;
            
            // (4) Сгенерировать новую случайную последовательность
            void regenerate();
            
            // (5) Возвращает "true", если использовали все варианты сдвига
            bool haveFinished() const;
            
            // (6) Вернуть позицию после очередного сдвига (через Vector)
            Vector getVector(Vector point);
            
            // (7) Вернуть позицию после очередного сдвига
            Vector getVector(int16_t x, int16_t y);
            
            // (8) Обнуляет поля, освобождает выделенную память
            void clear();
            
            // (9) Деструктор
            ~TrackSeq() = default;
    };
    
    ////////// struct Area ///////////////////////////////////////////////////
    // Структура описывает прямоугольую область на поле "Field".            //
    // Используется в основном для ограничения зоны действия алгоритма      //
    // построения пути.                                                     //
    //////////////////////////////////////////////////////////////////////////
    
    struct Area
    {
        public :
            
            int16_t minX;   // Координата левой границы
            int16_t maxX;   // Координата правой границы
            int16_t maxY;   // Координата верхней границы
            int16_t minY;   // Координата нижней границы
            
        public :
            
            // (1) Конструктор (обнуляет поля)
            Area();
            
            // (2) Конструктор копирования
            Area(const Area& newArea) = default;
            
            // (3) Перегрузка оператора присваивания
            Area& operator=(const Area& newArea) = default;
            
            // (4) Проверка попадания точки "point" в область
            bool inRange(Vector point) const;
            
            // (5) Проверка попадания точки (x, y) в область
            bool inRange(int16_t x, int16_t y) const;
            
            // (6) Проверка корректности границ области
            bool isCorrect() const;
            
            // (7) Обнуляет поля, освобождает выделенную память
            void clear();
            
            // (8) Деструктор
            ~Area() = default;
    };
}

#endif