////////// instrumentslib ////////////////////////////////////////////////////
// Автор    : Баталов Семен                                                 //
// Дата     : 08.11.2020                                                    //
// Описание : Здесь представлены инструменты для решения филиппинского      //
//            кроссворда.                                                   //
//////////////////////////////////////////////////////////////////////////////

#include "instrumentslib.h"

using namespace pcs;

////////// struct Vector /////////////////////////////////////////////////////
// Описание : instrumentslib.h                                              //
//////////////////////////////////////////////////////////////////////////////

// (1) Конструктор (обнуляет поля)
Vector::Vector()
{
    clear();
}

// (3) Конструктор от координат
Vector::Vector(int16_t newX, int16_t newY)
{
    x = newX;
    y = newY;
}

// (5) Перегрузка оператора равенства
bool Vector::operator==(Vector right) const
{
    return (x == right.x) && (y == right.y);
}

// (6) Перегрузка оператора неравенства
bool Vector::operator!=(Vector right) const
{
    return !(*this == right);
}

// (7) Обнуляет поля
void Vector::clear()
{
    x = 0;
    y = 0;
}

////////// distance //////////////////////////////////////////////////////////
// Описание : instrumentslib.h                                              //
//////////////////////////////////////////////////////////////////////////////

int16_t pcs::distance(Vector p1, Vector p2)
{
    return abs(p1.x - p2.x) + abs(p1.y - p2.y) + 1;
}

////////// bool inRangeSize/Index ////////////////////////////////////////////
// Описание : instrumentslib.h                                              //
//////////////////////////////////////////////////////////////////////////////
    
bool pcs::inRangeIndex(int16_t index)
{
    return index <= MAX_INDEX && index >= MIN_INDEX;
}
    
bool pcs::inRangeSizeX(int16_t sizeX)
{
    return sizeX <= MAX_SIZE_X && sizeX >= MIN_SIZE_X;
}

bool pcs::inRangeSizeY(int16_t sizeY)
{
    return sizeY <= MAX_SIZE_Y && sizeY >= MIN_SIZE_Y;
}

////////// max/min ///////////////////////////////////////////////////////////
// Описание : instrumentslib.h                                              //
//////////////////////////////////////////////////////////////////////////////

int16_t pcs::max(int16_t var1, int16_t var2)
{
    if (var1 >= var2)
    {
        return var1;
    }
    else
    {
        return var2;
    }
}

int16_t pcs::min(int16_t var1, int16_t var2)
{
    if (var1 <= var2)
    {
        return var1;
    }
    else
    {
        return var2;
    }
}

////////// struct Cell ///////////////////////////////////////////////////////
// Описание : instrumentslib.h                                              //
//////////////////////////////////////////////////////////////////////////////

// (1) Конструктор (обнуляет поля)
Cell::Cell()
{
    clear();
}

// (4) Обнуляет поля
void Cell::clear()
{
    index   = 0;
    id      = 0;
    num     = 0;
    protoId = 0;
    way     = NONE;
}

////////// class Field ///////////////////////////////////////////////////////
// Описание : instrumentslib.h                                              //
//////////////////////////////////////////////////////////////////////////////

// (1) Конструктор (обнуляет поля)
Field::Field()
{
    cells_ = nullptr;
    sizeX_ = 0;
    sizeY_ = 0;
}

// (2) Конструктор копирования
Field::Field(const Field& field)
{
    *this = field;
}

// (3) Перегрузка оператора присваивания
Field& Field::operator=(const Field& field)
{
    if (this != &field)
    {
        try
        {
            if (field.sizeX_ != sizeX_ || field.sizeY_ != sizeY_)
            {
                resize(field.sizeX_, field.sizeY_);
            }
            for (int i = 0; i < sizeX_; ++i)
            {
                for (int j = 0; j < sizeY_; ++j)
                {
                    cells_[i][j] = field.cells_[i][j];
                }
            }
        }
        catch (...)
        {
            clear();
        }
    }
    return *this;
}

// (4) Изменяет размер поля, уничтожая имеющуюся информацию
void Field::resize(int16_t sizeX, int16_t sizeY)
{
    // Проверка на выход за границы аргументов
    bool inRange = inRangeSizeX(sizeX) && inRangeSizeY(sizeY);
    
    // Меняем размер поля
    try
    {
        if ((sizeX_ != sizeX || sizeY_ != sizeY) && inRange)
        {
            clear();
            sizeX_ = sizeX;
            sizeY_ = sizeY;
            
            // Выделяем память под двумерный массив
            cells_ = new Cell*[sizeX_];
            for (int i = 0; i < sizeX_; ++i)
            {
                cells_[i] = new Cell[sizeY_];
            }
            
            // Инициализируем все значения "protoId"
            for (int j = 0; j < sizeY_; ++j)
            {
                for (int i = 0; i < sizeX_; ++i)
                {
                    cells_[i][j].protoId = sizeX_ * j + i + 1;
                }
            }
        }
        else
        if (!inRange)
        {
            throw err::PcsException(0);
        }
    }
    catch (...)
    {
        clear();
        throw;
    }
}

// (5) Перегрузка оператора ()
Cell& Field::operator()(int16_t x, int16_t y)
{
    if (!inRange(x, y))
    {
        throw err::PcsException(0);
    }
    return cells_[x][y];
}

// (6) Перегрузка оператора ()
const Cell& Field::operator()(int16_t x, int16_t y) const
{
    return this->operator()(x, y);
}

// (7) Перегрузка оператора () (через Vector)
Cell& Field::operator()(Vector point)
{
    if (!inRange(point))
    {
        throw err::PcsException(0);
    }
    return cells_[point.x][point.y];
}

// (8) Перегрузка оператора () (через Vector)
const Cell& Field::operator()(Vector point) const
{
    return this->operator()(point);
}

// (9) Возвращает размер по "X"
int16_t Field::getSizeX() const
{
    return sizeX_;
}

// (10) Возвращает размер по "Y"
int16_t Field::getSizeY() const
{
    return sizeY_;
}

// (11) Проверяет поле на отсутствие индексов
bool Field::isEmpty() const
{
    bool haveIndex = false;
    
    // Проходим по полю и проверяем наличие индексов
    for (int16_t j = 0; j < sizeY_; ++j)
    {
        for (int16_t i = 0; i < sizeX_; ++i)
        {
            haveIndex = haveIndex || cells_[i][j].index != 0;
        }
    }
    return !haveIndex;
}

// (12) Проверяет соответствие размеров поля
bool Field::isCorrectSize() const
{
    return inRangeSizeX(sizeX_) && inRangeSizeY(sizeY_);
}

// (13) Проверяет корректность задания индексов
bool Field::isCorrectIndex() const
{
    bool isInRange = true;
    
    // Проходим по полю и проверяем корректность индексов
    for (int16_t j = 0; j < sizeY_; ++j)
    {
        for (int16_t i = 0; i < sizeX_; ++i)
        {
            isInRange = isInRange && inRangeIndex(cells_[i][j].index);
        }
    }
    return isInRange;
}

// (14) Оставляет индексы и обнуляет все остальное
void Field::refresh()
{
    // Проходим по полю, оставляя только индексы
    for (int16_t j = 0; j < sizeY_; ++j)
    {
        for (int16_t i = 0; i < sizeX_; ++i)
        {
            cells_[i][j].way = NONE;
            cells_[i][j].num = 0;
            cells_[i][j].id  = 0;
        }
    }
}

// (15) "true" если не выходим за границу
bool Field::inRange(int16_t x, int16_t y) const
{
    // Находятся ли аргументы в диапазоне
    bool isOkX = x < sizeX_ && x >= 0;
    bool isOkY = y < sizeY_ && y >= 0;
    
    return isOkX && isOkY;
}

// (16) "true" если не выходим за границу (через Vector)
bool Field::inRange(Vector point) const
{
    return inRange(point.x, point.y);
}

// (17) Освобождает выделенную память
void Field::clear()
{
    if (cells_ != nullptr)
    {
        for (int i = 0; i < sizeX_; ++i)
        {
            if (cells_[i] != nullptr)
            {
                delete[] cells_[i];
            }
        }
        delete[] cells_;
        cells_ = nullptr;
    }
    sizeX_ = 0;
    sizeY_ = 0;
}

// (18) Деструктор
Field::~Field()
{
    clear();
}

////////// class TrackSeq ////////////////////////////////////////////////////
// Описание : instrumentslib.h                                              //
//////////////////////////////////////////////////////////////////////////////

// (1) Конструктор (обнуляет поля)
TrackSeq::TrackSeq()
{
    number_ = 0;
}

// (4) Сгенерировать новую случайную последовательность
void TrackSeq::regenerate()
{
    int16_t temp    = 0;
    bool    isEqual = false;
    
    // Задать нужный размер
    number_ = 0;
    if (seq_.size() != 4)
    {
        seq_.resize(4);
    }
    
    // Заполнить вектор случайной перестановкой из {0, 1, 2, 3}
    while (number_ < 4)
    {
        temp = rand() % 4;
        isEqual = false;
        for (int i = 0; i < number_ && !isEqual; ++i)
        {
            isEqual = seq_[i] == temp;
        }
        if (!isEqual)
        {
            seq_[number_] = temp;
            ++number_;
        }
    }
    number_ = 0;
}

// (5) Возвращает "true", если использовали все варианты сдвига
bool TrackSeq::haveFinished() const
{
    return number_ == 4;
}

// (6) Вернуть позицию после очередного сдвига (через Vector)
Vector TrackSeq::getVector(Vector point)
{
    return getVector(point.x, point.y);
}

// (7) Вернуть позицию после очередного сдвига
Vector TrackSeq::getVector(int16_t x, int16_t y)
{
    Vector temp(x, y);
    
    // Произвести соответствующий сдвиг
    if (number_ < 4 && seq_.size() == 4)
    {
        switch (seq_[number_])
        {
            case 0 :
                --temp.x;
                break;
            
            case 1 :
                ++temp.x;
                break;
            
            case 2 :
                ++temp.y;
                break;
            
            case 3 :
                --temp.y;
                break;
            
            default :
                throw err::PcsException(1);
                break;
        }
        ++number_;
    }
    return temp;
}

// (8) Обнуляет поля, освобождает выделенную память
void TrackSeq::clear()
{
    number_ = 0;
    seq_.clear();
}

////////// struct Area ///////////////////////////////////////////////////////
// Описание : instrumentslib.h                                              //
//////////////////////////////////////////////////////////////////////////////

// (1) Конструктор (обнуляет поля)
Area::Area()
{
    clear();
}

// (4) Проверка попадания точки "point" в область
bool Area::inRange(Vector point) const
{
    return inRange(point.x, point.y);
}

// (5) Проверка попадания точки (x, y) в область
bool Area::inRange(int16_t x, int16_t y) const
{
    return (x <= maxX) && (x >= minX) && (y <= maxY) && (y >= minY);
}

// (6) Проверка корректности границ области
bool Area::isCorrect() const
{
    return (minX <= maxX) && (minY <= maxY);
}

// (7) Обнуляет поля, освобождает выделенную память
void Area::clear()
{
    minX = 0;
    maxX = 0;
    minY = 0;
    maxY = 0;
}