#ifndef SOLVELIB_H
#define SOLVELIB_H

////////// solvelib //////////////////////////////////////////////////////////
// Автор    : Баталов Семен                                                 //
// Дата     : 08.11.2020                                                    //
// Описание : Здесь представлены инструменты для решения филиппинского      //
//            кроссворда.                                                   //
//////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <cstdint>
#include <vector>
#include <cmath>
#include <iomanip>

#include "errlib.h"

namespace pcs
{
    // Общий вещественный тип
    typedef float Real;
    
    // Максимальный и минимальный размеры поля кроссворда по "X"
    const int16_t MAX_SIZE_X = 100;
    const int16_t MIN_SIZE_X = 10;
    
    // Максимальный и минимальный размеры поля кроссворда по "Y"
    const int16_t MAX_SIZE_Y = 100;
    const int16_t MIN_SIZE_Y = 10;
    
    // Максимальное и минимальное возможное число в клетке
    const int16_t MAX_INDEX  = 255;
    const int16_t MIN_INDEX  = 0;
    
    // Коэффициент в экспоненте (поиск вероятности)
    const Real    ALPHA        = 0.001;
    const Real    MAX_T        = 20;
    const Real    MIN_T        = 0.005;
    const int16_t STAGE_NUMBER = 2;
    
    // Максимальное ограничивающее количество шагов при построении пути 
    const int32_t CYCLES_NUMBER = 10000;
    
    // Максимальное ограничивающее количество повторов поиска пути
    const int32_t REPLAY_NUMBER = 5;
    
    // Перечисление направлений, нужных для ориентирования путей на поле
    enum Direction {LEFT, RIGHT, VERTICAL, MIDDLE, NONE};
    
    ////////// bool inRangeSize/Index ////////////////////////////////////////
    // Возвращают "true", если значение аргумента не выходит за границы.    //
    //////////////////////////////////////////////////////////////////////////
    
    bool inRangeIndex(const int16_t& index);
    
    bool inRangeSizeX(const int16_t& sizeX);
    
    bool inRangeSizeY(const int16_t& sizeY);
    
    ////////// probability/haveAction ////////////////////////////////////////
    // 1) probability :                                                     //
    //      Возвращает вероятность события от 0 до 1 в зависимости от       //
    //      "number". С увеличением "number" вероятность уменьшается.       //
    // 2) haveAction :                                                      //
    //      Возвращают "true", если произошло событие, вероятность          //
    //      возникновения которого есть "chance".                           //
    //////////////////////////////////////////////////////////////////////////
    
    Real temperature(const int32_t& number);
    
    bool haveAction(const Real& chance);
    
    ////////// struct Vector /////////////////////////////////////////////////
    // Эта структура используется для более удобной адресации в поле ячеек. //
    //////////////////////////////////////////////////////////////////////////
    
    struct Vector
    {
        public :
            
            int16_t x;          // Абсцисса точки
            int16_t y;          // Ордината точки
            
        public :
            
            // (1) Конструктор (обнуляет поля)
            Vector();
            
            // (2) Конструктор копирования
            Vector(const Vector& vector) = default;
            
            // (3) Конструктор от координат
            Vector(const int16_t& newX, const int16_t& newY);
            
            // (4) Перегрузка оператора присваивания
            Vector& operator=(const Vector& vector) = default;
            
            // (5) Обнуляет поля
            void clear();
            
            // (6) Деструктор
            ~Vector() = default;
    };
    
    ////////// struct Cell ///////////////////////////////////////////////////
    // Эта структура описывает ячейку поля кроссворда (число в ячейке есть  //
    // index).                                                              //
    // Для пустой ячейки   : index = 0                                      //
    // Для непустой ячейки : 0 < index <= MAX_INDEX                         //
    //////////////////////////////////////////////////////////////////////////
    
    struct Cell
    {
        public :
            
            int16_t   index;    // Значение в данной клетке
            int16_t   id;       // ID пути, которым занята ячейка
            int16_t   num;      // Порядковый номер в пути
            int16_t   protoId;  // "id" пути с началов в этой клетке
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
            void resize(const int16_t& sizeX, const int16_t& sizeY);
            
            // (5) Перегрузка оператора ()
            Cell& operator()(const int16_t& x, const int16_t& y);
            
            // (6) Перегрузка оператора ()
            const Cell& operator()(const int16_t& x, const int16_t& y) const;
            
            // (7) Перегрузка оператора () (через Vector)
            Cell& operator()(const Vector& point);
            
            // (8) Перегрузка оператора () (через Vector)
            const Cell& operator()(const Vector& point) const;
            
            // (9) Возвращает размер по "X"
            int16_t getSizeX() const;
            
            // (10) Возвращает размер по "Y"
            int16_t getSizeY() const;
            
            // (11) "true" если не выходим за границу
            bool inRange(const int16_t& x, const int16_t& y) const;
            
            // (12) "true" если не выходим за границу (через Vector)
            bool inRange(const Vector& point) const;
            
            // (13) Освобождает выделенную память
            void clear();
            
            // (14) Деструктор
            ~Field();
    };
    
    ////////// class TrackSeq ////////////////////////////////////////////////
    // Объект класса генерирует в себе случайную последовательность сдвигов //
    // на поле относительно заданной точки (Влево, Вправо, Вверх, Вниз).    //
    //                                                                      //
    // 1) void regenerate()   : сгенерировать случайную последовательность. //
    // 2) Vector getVector(...) : запросить очередной случайный сдвиг.      //
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
            Vector getVector(const Vector& point);
            
            // (7) Вернуть позицию после очередного сдвига
            Vector getVector(const int16_t& x, const int16_t& y);
            
            // (8) Обнуляет поля, освобождает выделенную память
            void clear();
            
            // (9) Деструктор
            ~TrackSeq() = default;
    };
    
    ////////// class Solver //////////////////////////////////////////////////
    // Класс, предназначеный для решения кроссворда.                        //
    //                                                                      //
    // 1) void setField(...)          : инициализирует игровое поле.        //
    // 2) bool isCorrectField()       : возвращает "", если поле задано     //
    //    корректно (есть != 0 клетки, у каждой >= 2 клетки есть пара,      //
    //    размеры поля заданы корректно).                                   //
    // 3) void solve(bool& wasSolved) : разводит пути по полю кроссворда,   //
    //    предварительно проверив коррекность поля методом                  //
    //    "isCorrectField()" и подготовив его методом "prepare()".          //
    // 4) void setDirections()        : устанавливает направления путей     //
    //    в каждой клетке (LEFT, RIGHT, VERTICAL, MIDDLE, NONE).            //
    // 5) void getField(...)          : копирует игровое поле в аргумент.   //         
    //////////////////////////////////////////////////////////////////////////
    
    class Solver
    {
        private :
            
            Field map_;                     // Рабочее поле кроссворда
            Field bestMap_;                 // Лучшая конфигурация в МИО
            std::vector<TrackSeq> track_;   // Информация о сдвигах
            std::vector<Vector>   path_;    // Хранилище прошлых позиций
            std::vector<Vector>   points_;  // Вектор нетривиальных клеток
            int16_t               nPPaths_; // Max кол-во нетрив. путей
            
        public :
            
            // (1) Конструктор (обнуляет поля)
            Solver();
            
            // (2) Конструктор копирования
            Solver(const Solver& solver) = default;
            
            // (3) Перегрузка оператора присваивания
            Solver& operator=(const Solver& solver) = default;
            
            // (4) Инициализирует поле кроссворда
            void setField(const Field& map);
            
            // (5) Возвращает поле
            void getField(Field& map) const;
            
            // (6) Проверка (первичная) корректности задания поля
            bool isCorrectField();
            
            // (7) Разводит пути на поле (в соответствии с правилами игры)
            void solve(bool& wasSolved);
            
            // (8) Задает направления обхода путей (для вывода на экран)
            void setDirections();
            
            // (9) Освобождает выделенную память
            void clear();
            
            // (10) Деструктор
            ~Solver() = default;
            
        private :
            
            // (1) Подготавливает поле (удаляет лишнее)
            void prepare();
            
            // (2) Создает новый путь из точки "point" в родственную точку
            void createPath(const Vector& point, const int16_t& id);
            
            // (3) Создает пути из всех незадействованных точек
            void createRandomPaths();
            
            // (4) Удаляет путь с номером "id"
            void deletePath(int16_t id /*Vector point*/);
            
            // (5) Удаляет пути в соответствии с вероятностью "chance"
            void deleteRandomPaths();
            
            // (6) Производим случайное изменение поля "map_"
            void changeMap(int16_t& nInvolved);
            
            // (7) Устанавливает направление обхода в точке "point"
            void setDirection(const Vector& point);
            
            // (8) Возвращает количество непостроенных путей
            int16_t getMisPathNumber();
            
            // (9) Возвращает количество задействованных точек поля
            int16_t getInvolvedCellsNumber();
            
            // (10) Производит случайную перестановку в векторе "points_"
            void createPermutation();
            
            // (11) Находит следующую точку для удаления, если она есть
            void findNextToDelete(Vector& curPoint, bool& wasFound);
    };
}

#endif