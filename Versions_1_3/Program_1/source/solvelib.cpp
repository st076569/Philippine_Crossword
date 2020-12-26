////////// solvelib //////////////////////////////////////////////////////////
// Автор    : Баталов Семен                                                 //
// Дата     : 08.11.2020                                                    //
// Описание : Здесь представлены инструменты для решения филиппинского      //
//            кроссворда.                                                   //
//////////////////////////////////////////////////////////////////////////////

#include "solvelib.h"

using namespace pcs;

////////// bool inRangeSize/Index ////////////////////////////////////////////
// Описание : solvelib.h                                                    //
//////////////////////////////////////////////////////////////////////////////
    
bool pcs::inRangeIndex(const int16_t& index)
{
    return index <= MAX_INDEX && index >= MIN_INDEX;
}
    
bool pcs::inRangeSizeX(const int16_t& sizeX)
{
    return sizeX <= MAX_SIZE_X && sizeX >= MIN_SIZE_X;
}

bool pcs::inRangeSizeY(const int16_t& sizeY)
{
    return sizeY <= MAX_SIZE_Y && sizeY >= MIN_SIZE_Y;
}

////////// probability/haveAction ////////////////////////////////////////////
// Описание : solvelib.h                                                    //
//////////////////////////////////////////////////////////////////////////////

Real pcs::temperature(const int32_t& number)
{
    if (number >= 0)
    {
        return MAX_T * exp(static_cast<double>(-number) * ALPHA);
    }
    return 0.0;
}

bool pcs::haveAction(const Real& chance)
{
    return (static_cast<double>(rand()) / RAND_MAX) < chance;
}

////////// struct Vector /////////////////////////////////////////////////////
// Описание : solvelib.h                                                    //
//////////////////////////////////////////////////////////////////////////////

// (1) Конструктор (обнуляет поля)
Vector::Vector()
{
    clear();
}

// (3) Конструктор от координат
Vector::Vector(const int16_t& newX, const int16_t& newY)
{
    x = newX;
    y = newY;
}

// (5) Обнуляет поля
void Vector::clear()
{
    x = 0;
    y = 0;
}

////////// struct Cell ///////////////////////////////////////////////////////
// Описание : solvelib.h                                                    //
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
// Описание : solvelib.h                                                    //
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
void Field::resize(const int16_t& sizeX, const int16_t& sizeY)
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
    }
    catch (...)
    {
        clear();
        throw;
    }
}

// (5) Перегрузка оператора ()
Cell& Field::operator()(const int16_t& x, const int16_t& y)
{
    if (!inRange(x, y))
    {
        throw err::Exception(4);
    }
    return cells_[x][y];
}

// (6) Перегрузка оператора ()
const Cell& Field::operator()(const int16_t& x, 
                              const int16_t& y) const
{
    return this->operator()(x, y);
}

// (7) Перегрузка оператора () (через Vector)
Cell& Field::operator()(const Vector& point)
{
    if (!inRange(point))
    {
        throw err::Exception(4);
    }
    return cells_[point.x][point.y];
}

// (8) Перегрузка оператора () (через Vector)
const Cell& Field::operator()(const Vector& point) const
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

// (11) "true" если не выходим за границу
bool Field::inRange(const int16_t& x, const int16_t& y) const
{
    // Находятся ли аргументы в диапазоне
    bool isOkX = x < sizeX_ && x >= 0;
    bool isOkY = y < sizeY_ && y >= 0;
    
    return isOkX && isOkY;
}

// (12) "true" если не выходим за границу (через Vector)
bool Field::inRange(const Vector& point) const
{
    return inRange(point.x, point.y);
}

// (13) Освобождает выделенную память
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

// (14) Деструктор
Field::~Field()
{
    clear();
}

////////// class TrackSeq ////////////////////////////////////////////////////
// Описание : solvelib.h                                                    //
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
Vector TrackSeq::getVector(const Vector& point)
{
    return getVector(point.x, point.y);
}

// (7) Вернуть позицию после очередного сдвига
Vector TrackSeq::getVector(const int16_t& x, const int16_t& y)
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
                throw err::Exception(555);
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

////////// class Solver //////////////////////////////////////////////////////
// Описание : solvelib.h                                                    //
//////////////////////////////////////////////////////////////////////////////

////////// public ////////////////////////////////////////////////////////////

// (1) Конструктор (обнуляет поля)
Solver::Solver()
{
    nPPaths_ = 0;
}

// (4) Инициализирует поле кроссворда
void Solver::setField(const Field& map)
{
    map_ = map;
}

// (5) Возвращает поле
void Solver::getField(Field& map) const
{
    map = map_;
}

// (6) Проверка (первичная) корректности задания поля
bool Solver::isCorrectField()
{
    // Проверить, что размеры поля подходящие
    bool isNormSize   = inRangeSizeX(map_.getSizeX()) && 
                        inRangeSizeY(map_.getSizeY());
    
    // Если есть цифры на поле и они из диапазона
    bool isNormIndex  = true;
    bool haveIndex    = false;
    
    // Если все цифры > 1 образуют пары
    bool isNormNumber = true;
    
    // Отдельная проверка корректности для индекса 2
    bool isNorm2      = true;
    bool haveL        = false;
    bool haveR        = false;
    bool haveU        = false;
    bool haveD        = false;
    
    // Проверить, что есть != 0 числа и все они находятся в диапазоне
    for (int j = 0; j < map_.getSizeY(); ++j)
    { 
        for (int i = 0; i < map_.getSizeX(); ++i)
        {
            if (map_(i, j).index != 0)
            {
                haveIndex = true;
            }
            isNormIndex = isNormIndex && inRangeIndex(map_(i, j).index);
        }
    }
    
    // Проверить, что непустые клетки (>= 2) образуют пары
    if (isNormIndex && haveIndex)
    {
        std::vector<int16_t> numbers;
        numbers.resize(256, 0);
        for (int j = 0; j < map_.getSizeY(); ++j)
        {
            for (int i = 0; i < map_.getSizeX(); ++i)
            {
                ++numbers[map_(i, j).index];
            }
        }
        for (int i = 2; i < 256; ++i)
        {
            isNormNumber = isNormNumber && (numbers[i] % 2 == 0);
        }
    }
    
    // Проверяем все клетки с индексом 2 на наличие "соседа"
    if (isNormIndex && haveIndex && isNormNumber)
    {
        for (int j = 0; j < map_.getSizeY() && isNorm2; ++j)
        { 
            for (int i = 0; i < map_.getSizeX() && isNorm2; ++i)
            {
                if (map_(i, j).index == 2)
                {
                    haveL = false;
                    haveR = false;
                    haveU = false;
                    haveD = false;
                    
                    // Проверяем наличие "родственных соседей"
                    if (i != 0)
                    {
                        haveL = (map_(i - 1, j).index == 2);
                    }
                    if (i != map_.getSizeX() - 1)
                    {
                        haveR = (map_(i + 1, j).index == 2);
                    }
                    if (j != map_.getSizeY() - 1)
                    {
                        haveU = (map_(i, j + 1).index == 2);
                    }
                    if (j != 0)
                    {
                        haveD = (map_(i, j - 1).index == 2);
                    }
                    
                    // Условие наличия хотя бы одного "соседа"
                    isNorm2 = (haveL || haveR || haveU || haveD);
                }
            }
        }
    }
    return isNormSize && isNormIndex && isNormNumber && haveIndex && isNorm2;
}

// (7) Разводит пути на поле (в соответствии с правилами игры)
void Solver::solve(bool& wasSolved)
{
    // Предварительная инициализация
    wasSolved = false;
    
    // Если есть смысл строить пути
    if (isCorrectField())
    {
        int16_t step     = 0;
        int16_t stage    = 0;
        int16_t nMisNow  = 0;
        int16_t nMisBest = 0;
        
        // Подготовка поля
        prepare();
        
        // Удаляем и создаем пути до тех пор, пока не решим задачу 
        // или не достигнем критической вероятности выбора
        while (!wasSolved && temperature(step) >= MIN_T)
        {
            changeMap(nMisNow);
            
            if (nMisNow > nMisBest || haveAction(exp((nMisNow - nMisBest) / temperature(step))))
            {
                bestMap_ = map_;
                nMisBest = nMisNow;
                std::cout << std::setw(4) << step << ") : " << nMisBest << "; " << temperature(step) << '\n';
                ++step;
            }
            else
            {
                map_ = bestMap_;
                ++step;
            }
            
            if (temperature(step) < MIN_T && stage < STAGE_NUMBER)
            {
                ++stage;
                step *= 0.2;
            }
            
            // Если развели все, то выходим из цикла
            wasSolved = (getMisPathNumber() == 0);
        }
    }
}

// (8) Задает направления обхода путей (для вывода на экран)
void Solver::setDirections()
{
    // Итератор
    Vector iPoint;
        
    // Проходим по всему полю
    for (iPoint.y = 0; iPoint.y < map_.getSizeY(); ++iPoint.y)
    {
        for (iPoint.x = 0; iPoint.x < map_.getSizeX(); ++iPoint.x)
        {
            setDirection(iPoint);
        }
    }
}

// (9) Освобождает выделенную память
void Solver::clear()
{
    nPPaths_ = 0;
    bestMap_.clear();
    points_.clear();
    track_.clear();
    path_.clear();
    map_.clear();
}

////////// private ///////////////////////////////////////////////////////////

// (1) Подготавливает поле (удаляет лишнее)
void Solver::prepare()
{
    // Итератор и счетчики
    Vector      iPoint;
    int16_t     nPoints = 0;
    int16_t     pos     = 0;
    int16_t     count   = 0;
    
    // Проходим по всему полю, очищая вспомогательную информацию
    for (iPoint.y = 0; iPoint.y < map_.getSizeY(); ++iPoint.y)
    {
        for (iPoint.x = 0; iPoint.x < map_.getSizeX(); ++iPoint.x)
        {
            if (map_(iPoint).index > 2)
            {
                ++nPoints;
            }
            map_(iPoint).way = NONE;
            map_(iPoint).num = 0;
            map_(iPoint).id  = 0;
        }
    }
    
    // Задаем размеры хранилищ
    try
    {
        // Хранилище сдвигов
        if (track_.size() != MAX_INDEX + 1)
        {
            track_.resize(MAX_INDEX + 1);
        }
        
        // Хранилище прошлых позиций
        if (path_.size() != MAX_INDEX + 1)
        {
            path_.resize(MAX_INDEX + 1);
        }
        
        // Хранилище клеток с цифрами > 2
        if (points_.size() != nPoints)
        {
            points_.resize(nPoints);
        }
        
        // Проходим по всему полю, записываем клетки с цифрами > 2
        for (iPoint.y = 0; iPoint.y < map_.getSizeY(); ++iPoint.y)
        {
            for (iPoint.x = 0; iPoint.x < map_.getSizeX(); ++iPoint.x)
            {
                ++count;
                if (map_(iPoint).index > 2)
                {
                    points_[pos] = iPoint;
                    ++pos;
                }
                else
                if (map_(iPoint).index > 0)
                {
                    map_(iPoint).id = count;
                }
            }
        }
        
        nPPaths_ = nPoints / 2;
        bestMap_ = map_;
    }
    catch (...)
    {
        clear();
        throw;
    }
}

// (2) Создает новый путь из точки "point" в родственную точку
void Solver::createPath(const Vector& point, const int16_t& id)
{
    // Имеет ли смысл строить путь?
    if (map_(point).id == 0 && map_(point).index != 0)
    {
        bool    isFinished = false;     // Нашли путь или он не сущ-ет
        bool    forward    = true;      // Направление последнего сдвига
        bool    indexMatch = false;     // Совпадение индексов клеток
        bool    stepMatch  = false;     // Дошли до конца ветви пути
        int16_t step       = 0;         // Текущий сдвиг от начала пути
        int32_t count      = 0;         // Счетчик циклов
        
        // Текущая позиция
        Vector curPoint;
        
        for (int i = 0; i < REPLAY_NUMBER && !isFinished; ++i)
        {
            // Обновляем счетчик, сдвиг, текущую позицию
            count    = 0;
            step     = 0;
            curPoint = point;
            
            // Удаляем остатки пути
            deletePath(id);
            
            // Случайным перебором находим подходящий путь
            while (!isFinished && count < CYCLES_NUMBER)
            {
                // Проверяем, в какой точке поля оказались
                if (!map_.inRange(curPoint))
                {
                    --step;
                    forward  = false;
                    curPoint = path_[step];
                }
                else
                if (map_(curPoint).id == 0)
                {
                    indexMatch = (map_(point).index == map_(curPoint).index);
                    stepMatch  = (step == map_(point).index - 1);
                    
                    if (indexMatch && stepMatch)
                    {
                        map_(curPoint).id  = id;
                        map_(curPoint).num = step;
                        isFinished = true;
                    }
                    else
                    if (map_(curPoint).index == 0 && !stepMatch || step == 0)
                    {
                        map_(curPoint).id  = id;
                        map_(curPoint).num = step;
                        path_[step]        = curPoint;
                        
                        track_[step].regenerate();
                        curPoint = track_[step].getVector(curPoint);
                        ++step;
                        forward  = true;
                    }
                    else
                    {
                        --step;
                        forward  = false;
                        curPoint = path_[step];
                    }
                }
                else
                if (map_(curPoint).id == id && !forward)
                {
                    if (!track_[step].haveFinished())
                    {
                        curPoint = track_[step].getVector(curPoint);
                        ++step;
                        forward = true;
                    }
                    else
                    {
                        map_(curPoint).id  = 0;
                        map_(curPoint).num = 0;
                        map_(curPoint).way = NONE;
                        
                        if (step > 0)
                        {
                            --step;
                            forward  = false;
                            curPoint = path_[step];
                        }
                        else
                        {
                            isFinished = true;
                        }
                    }
                }
                else
                {
                    --step;
                    forward  = false;
                    curPoint = path_[step];
                }
                ++count;
            }
        }
        
        // Если не смогли построить
        if (!isFinished)
        {
            // Удаляем остатки пути
            deletePath(id);
        }
    }
}

// (3) Создает пути из всех не задействованных точек
void Solver::createRandomPaths()
{
    // Производим перестановку координат клеток с цифрами
    createPermutation();
    
    // Строим случайные пути
    for (int i = 0; i < points_.size(); ++i)
    {
        if (map_(points_[i]).id == 0)
        {
            createPath(points_[i], map_(points_[i]).protoId);
        }
    }
}

// (4) Удаляет путь с номером "id"
void Solver::deletePath(int16_t id /*Vector point*/)
{
    if (id != 0)
    {
        Vector  iPoint;
        
        // Проходим по всему полю находя занятые точки
        for (iPoint.y = 0; iPoint.y < map_.getSizeY(); ++iPoint.y)
        {
            for (iPoint.x = 0; iPoint.x < map_.getSizeX(); ++iPoint.x)
            {
                // Если точка занята, то добавляем
                if (map_(iPoint).id == id)
                {
                    // Обновляем клетку
                    map_(iPoint).id  = 0;
                    map_(iPoint).num = 0;
                    map_(iPoint).way = NONE;
                }
            }
        }
    }
    
    /*
    if (map_(point).id != 0)
    {
        // Коорд. текущей точки
        Vector prevPoint;
        bool   haveExit = false;
        
        // Пошагово удаляем путь
        while (!haveExit)
        {
            // Находим очередную точку для удаления
            prevPoint = point;
            findNextToDelete(point, haveExit);
            haveExit  = !haveExit;
            
            // Обновляем клетку
            map_(prevPoint).id  = 0;
            map_(prevPoint).num = 0;
            map_(prevPoint).way = NONE;
        }
    }
    */
}

// (5) Удаляет пути в соответствии с вероятностью "chance"
void Solver::deleteRandomPaths()
{
    // Кол-во путей для удаления
    int16_t delNum = 1 + rand() % nPPaths_;
    
    // Производим перестановку координат клеток с цифрами
    createPermutation();
    
    // Удаляет в случайное кол-во "delNum" путей
    for (int i = 0; i < points_.size() && delNum > 0; ++i)
    {
        if (map_(points_[i]).id != 0)
        {
            deletePath(map_(points_[i]).id);
            --delNum;
        }
    }
}

// (6) Производим случайное изменение поля "map_"
void Solver::changeMap(int16_t& nInvolved)
{
    // Удаляем часть путей и строим заново
    deleteRandomPaths();
    createRandomPaths();
    
    // Считаем количество задействованных клеток поля
    nInvolved = getInvolvedCellsNumber();
}

// (7) Устанавливает направление обхода в точке "point"
void Solver::setDirection(const Vector& point)
{
    if (map_(point).id != 0 && map_(point).index == 0)
    {
        bool haveLeft  = false;
        bool haveRight = false;
        bool haveUp    = false;
        bool haveDown  = false;
        bool temp1     = false;
        bool temp2     = false;
        bool temp3     = false;
        
        // Устанавливаем положение соседних точек пути (если есть)
        if (map_.inRange(point.x - 1, point.y))
        {
            temp1 = map_(point.x - 1, point.y).id == map_(point).id;
            temp2 = map_(point).num + 1 == map_(point.x - 1, point.y).num;
            temp3 = map_(point).num - 1 == map_(point.x - 1, point.y).num;
            haveLeft = temp1 && (temp2 || temp3);
        }
        if (map_.inRange(point.x + 1, point.y))
        {
            temp1 = map_(point.x + 1, point.y).id == map_(point).id;
            temp2 = map_(point).num + 1 == map_(point.x + 1, point.y).num;
            temp3 = map_(point).num - 1 == map_(point.x + 1, point.y).num;
            haveRight = temp1 && (temp2 || temp3);
        }
        if (map_.inRange(point.x, point.y + 1))
        {
            temp1 = map_(point.x, point.y + 1).id == map_(point).id;
            temp2 = map_(point).num + 1 == map_(point.x, point.y + 1).num;
            temp3 = map_(point).num - 1 == map_(point.x, point.y + 1).num;
            haveUp = temp1 && (temp2 || temp3);
        }
        if (map_.inRange(point.x, point.y - 1))
        {
            temp1 = map_(point.x, point.y - 1).id == map_(point).id;
            temp2 = map_(point).num + 1 == map_(point.x, point.y - 1).num;
            temp3 = map_(point).num - 1 == map_(point.x, point.y - 1).num;
            haveDown = temp1 && (temp2 || temp3);
        }
        
        // Устанавливаем направление в точке
        if ((haveUp || haveDown) && haveLeft)
        {
            map_(point).way = LEFT;
        }
        else
        if ((haveUp || haveDown) && haveRight)
        {
            map_(point).way = RIGHT;
        }
        else
        if (haveLeft && haveRight)
        {
            map_(point).way = MIDDLE;
        }
        else
        if (haveUp && haveDown)
        {
            map_(point).way = VERTICAL;
        }
        else
        {
            map_(point).way = NONE;
        }
    }
    else
    {
        map_(point).way = NONE;
    }
}

// (8) Возвращает количество непостроенных путей
int16_t Solver::getMisPathNumber()
{
    int16_t nMisPaths = 0;
    
    //
    for (int i = 0; i < points_.size(); ++i)
    {
        if (map_(points_[i]).id == 0)
        {
            ++nMisPaths;
        }
    }
    
    return nMisPaths / 2;
}

// (9) Возвращает количество задействованных точек поля
int16_t Solver::getInvolvedCellsNumber()
{
    Vector  iPoint;
    int16_t nInvolved = 0;
    
    // Проходим по всему полю находя занятые точки
    for (iPoint.y = 0; iPoint.y < map_.getSizeY(); ++iPoint.y)
    {
        for (iPoint.x = 0; iPoint.x < map_.getSizeX(); ++iPoint.x)
        {
            // Если точка занята, то добавляем
            if (map_(iPoint).id != 0)
            {
                ++nInvolved;
            }
        }
    }
    return nInvolved;
}

// (10) Производит случайную перестановку в векторе "points_"
void Solver::createPermutation()
{
    Vector  temp;
    int16_t pos = 0;
    
    // Создаем случайную перестановку
    for (int16_t i = points_.size() - 1; i > 0; --i)
    {
        pos          = rand() % (i + 1);
        temp         = points_[i];
        points_[i]   = points_[pos];
        points_[pos] = temp;
    }
}

// (11) Находит следующую точку для удаления, если она есть
void Solver::findNextToDelete(Vector& curPoint, bool& wasFound)
{
    // Коорд. текущей точки
    int16_t i  = curPoint.x;
    int16_t j  = curPoint.y;
        
    // Инициализация вспомогательных переменных
    bool haveL = false;
    bool haveR = false;
    bool haveU = false;
    bool haveD = false;
    
    // Проверяем наличие "родственных соседей"
    if (i != 0)
    {
        haveL = (map_(i - 1, j).id == map_(i, j).id) &&
                (abs(map_(i - 1, j).num - map_(i, j).num) == 1);
    }
    if (i != map_.getSizeX() - 1)
    {
        haveR = (map_(i + 1, j).id == map_(i, j).id) &&
                (abs(map_(i + 1, j).num - map_(i, j).num) == 1);
    }
    if (j != map_.getSizeY() - 1)
    {
        haveU = (map_(i, j + 1).id == map_(i, j).id) &&
                (abs(map_(i, j + 1).num - map_(i, j).num) == 1);
    }
    if (j != 0)
    {
        haveD = (map_(i, j - 1).id == map_(i, j).id) &&
                (abs(map_(i, j - 1).num - map_(i, j).num) == 1);
    }
    
    // Смещаемся в точку соседнюю точку пути
    if (haveL)
    {
        --i;
    }
    if (haveR)
    {
        ++i;
    }
    if (haveU)
    {
        ++j;
    }
    if (haveD)
    {
        --j;
    }
    
    // Если есть соседняя точка, то переходим в нее
    wasFound   = (haveL || haveR || haveU || haveD);
    curPoint.x = i;
    curPoint.y = j;
}