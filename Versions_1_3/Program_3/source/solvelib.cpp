////////// solvelib //////////////////////////////////////////////////////////
// Автор    : Баталов Семен                                                 //
// Дата     : 08.11.2020                                                    //
// Описание : Здесь описан класс, который содержит в себе все необходимые   //
//            методы для решения филлипинского кроссворда.                  //
//////////////////////////////////////////////////////////////////////////////

#include "solvelib.h"
#include <iostream>

using namespace pcs;

////////// class Solver //////////////////////////////////////////////////////
// Описание : solvelib.h                                                    //
//////////////////////////////////////////////////////////////////////////////

////////// public ////////////////////////////////////////////////////////////

// (1) Конструктор (обнуляет поля)
Solver::Solver()
{
    nAreas_ = 0;
    nId_    = 0;
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
    // Проверяем условия корректности поля
    bool isNormSize  = map_.isCorrectSize();
    bool isNormIndex = map_.isCorrectIndex();
    bool isNotEmpty  = !(map_.isEmpty());
    bool isNormNum   = isCorrectNumber();
    bool isNormRel   = isCorrectRelations();
    
    // Для дальнейшей работы необходимо выполнение всех условий
    return isNormSize && isNormIndex && isNormNum && isNotEmpty && isNormRel;
}

// (7) Подготавливает поле
void Solver::prepare()
{
    Vector  iPoint;
    int16_t nPoints = 0;
    
    // Оставляем на поле только индексы
    map_.refresh();
    
    // Считаем кол-во нетривиальных клеток на поле
    for (iPoint.y = 0; iPoint.y < map_.getSizeY(); ++iPoint.y)
    {
        for (iPoint.x = 0; iPoint.x < map_.getSizeX(); ++iPoint.x)
        {
            if (map_(iPoint).index > 2)
            {
                ++nPoints;
            }
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
        
        // Хранилище нетривиальных клеток
        if (points_.size() != nPoints)
        {
            idArray_.resize(nPoints / 2);
            points_.resize(0);
            areas_.resize(nPoints);
        }
    }
    catch (...)
    {
        clear();
        throw;
    }
    
    // Проходим по всему полю, записываем клетки с цифрами > 2
    for (iPoint.y = 0; iPoint.y < map_.getSizeY(); ++iPoint.y)
    {
        for (iPoint.x = 0; iPoint.x < map_.getSizeX(); ++iPoint.x)
        {
            if (map_(iPoint).index == 1 || map_(iPoint).index == 2)
            {
                map_(iPoint).id = map_(iPoint).protoId;
            }
            else
            if (map_(iPoint).index > 0)
            {
                points_.push_back(iPoint);
            }
        }
    }
}

// (8) Разводит пути на поле (если это удается)
void Solver::solve(bool& wasSolved)
{
    // Предварительная инициализация
    wasSolved = false;
    Vector curPoint;
    int32_t count = 0;
    
    // Строим пути, каждый раз в новом порядке
    for (int16_t i = 0; i < MAIN_REPLAY_NUMBER && !wasSolved; ++i)
    {
        // Создаем случайную перестановку нетрив. точек
        //createPermutation(points_);
        
        // Заполняем нетривиальными точками
        fillStack();
        
        // Пока не опустошим строим пути
        while (!waiting_.empty() && count < 600000)
        {
            count++;
            curPoint = waiting_.front();
            //std::cout << "(" << curPoint.x << "," << curPoint.y << ") ";
            waiting_.pop();
            
            if (map_(curPoint).id == 0)
            {
                createPath(curPoint);
            }
        }
        wasSolved = (getMisPathNumber() == 0);
    }
    
    /*
    for (int i = 0; i < points_.size(); ++i)
    {
        if (map_(points_[i]).id == 0)
        {
            std::cout << "(" << points_[i].x << ", " << points_[i].y << ") ";
        }
    }
    */
}

// (9) Задает направления обхода путей (для вывода на экран)
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

// (10) Освобождает выделенную память
void Solver::clear()
{
    map_.clear();
    path_.clear();
    areas_.clear();
    track_.clear();
    points_.clear();
    idArray_.clear();
    nAreas_ = 0;
    nId_    = 0;
}

////////// private ///////////////////////////////////////////////////////////

// (1) Составляет область поиска соседей
void Solver::createCheckingArea(Vector point, Area& area)
{
    area.maxX = min(map_.getSizeX() - 1, point.x + map_(point).index - 1);
    area.minX = max(0, point.x - map_(point).index + 1);
    area.maxY = min(map_.getSizeY() - 1, point.y + map_(point).index - 1);
    area.minY = max(0, point.y - map_(point).index + 1);
}

// (2) Составляет рабочую область для двух точек
void Solver::createWorkingArea(Vector p1, Vector p2, Area& area)
{
    // Величина расширения области
    int16_t shift = (map_(p1).index - distance(p1, p2)) / 2;

    // Продлеваем границы на величину "shift"
    area.maxX = min(map_.getSizeX() - 1, max(p1.x + shift, p2.x + shift));
    area.minX = max(0, min(p1.x - shift, p2.x - shift));
    area.maxY = min(map_.getSizeY() - 1, max(p1.y + shift, p2.y + shift));
    area.minY = max(0, min(p1.y - shift, p2.y - shift));
}

// (3) Точки являются соседями?
bool Solver::areRelatedPoints(Vector p1, Vector p2)
{
    // Совпадение индексов
    if (map_(p1).index == map_(p2).index)
    {
        // Точки разные
        if (p1 != p2)
        {
            // Расстояние между точками
            int16_t differ = map_(p1).index - distance(p1, p2);
            
            // Условие сущ-я пути между точками
            if (differ >= 0 && differ % 2 == 0)
            {
                return true;
            }
        }
    }
    return false;
}

// (4) Создает рабочие области для "point"
void Solver::createWorkingAreas(Vector point)
{
    Vector iPoint;
    Area   area;
    
    // Создаем область поиска и начинаем отсчет заново
    nAreas_ = 0;
    createCheckingArea(point, area);
    
    // Проходим по области поиска, ищем соседей
    for (iPoint.y = area.minY; iPoint.y <= area.maxY; ++iPoint.y)
    {
        for (iPoint.x = area.minX; iPoint.x <= area.maxX; ++iPoint.x)
        {
            if (areRelatedPoints(point, iPoint))
            {
                createWorkingArea(point, iPoint, areas_[nAreas_]);
                ++nAreas_;
            }
        }
    }
}

// (5) Строит (по возможности) все пути
void Solver::createAllPaths()
{
    // Проходим по всем нетривиальным точкам и строим пути
    for (int16_t i = 0; i < points_.size(); ++i)
    {
        if (map_(points_[i]).id == 0)
        {
            createPath(points_[i]);
        }
    }
}

// (6) Строит путь из точки "point"
void Solver::createPath(Vector point)
{
    bool wasCreated = false;
    bool wasDeleted = true;
    
    // Имеет ли смысл строить путь?
    if (map_(point).id == 0 && map_(point).index != 0)
    {
        // Создать рабочие области, начинаем отсчет "id" заново
        createWorkingAreas(point);
        nId_ = 0;
        
        // Переставляем случайным образом рабочие области
        createPermutation(areas_, nAreas_);
        
        // Пытаемся провести путь из точки "point"
        while (!wasCreated && wasDeleted)
        {
            // Перебираем соседей поочередно
            for (int16_t i = 0; i < nAreas_ && !wasCreated; ++i)
            {
                createAreaPath(point, areas_[i], wasCreated);
            }
            
            // Если не удалось построить, то удаляем случайный путь "id"
            if (!wasCreated)
            {
                deleteRandomPath(wasDeleted);
            }
        }
    }
}

// (7) Создает новый путь из точки "point" в родственную точку
void Solver::createAreaPath(Vector point, Area area, bool& wasCreated)
{
    // Первичная инициализация
    wasCreated = false;
    
    // Имеет ли смысл строить путь?
    if (map_(point).id == 0 && map_(point).index != 0 && area.isCorrect())
    {
        bool    isFinished = false;     // Нашли путь или он не сущ-ет
        bool    forward    = true;      // Направление последнего сдвига
        bool    indexMatch = false;     // Совпадение индексов клеток
        bool    stepMatch  = false;     // Дошли до конца ветви пути
        int16_t step       = 0;         // Текущий сдвиг от начала пути
        int32_t count      = 0;         // Счетчик циклов
        
        // Текущая позиция
        Vector curPoint;
        
        for (int16_t i = 0; i < PATH_REPLAY_NUMBER && !isFinished; ++i)
        {
            // Обновляем счетчик, сдвиг, текущую позицию
            count    = 0;
            step     = 0;
            curPoint = point;
            forward  = true;
            
            // Удаляем остатки пути
            deletePath(point);
            
            // Случайным перебором находим подходящий путь
            while (!isFinished && count < PATH_CYCLES_NUMBER)
            {
                // Проверяем, в какой точке поля оказались
                if (!area.inRange(curPoint))
                {
                    // Вышли за границы области
                    --step;
                    forward  = false;
                    curPoint = path_[step];
                }
                else
                if (map_(curPoint).id == 0)
                {
                    indexMatch = (map_(point).index == map_(curPoint).index);
                    stepMatch  = (step == map_(point).index - 1);
                    
                    // Анализ очередной свободной точки 
                    if (indexMatch && stepMatch)
                    {
                        map_(curPoint).id  = map_(point).protoId;
                        map_(curPoint).num = step;
                        isFinished = true;
                        wasCreated = true;
                    }
                    else
                    if (map_(curPoint).index == 0 && !stepMatch || step == 0)
                    {
                        map_(curPoint).id  = map_(point).protoId;
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
                if (map_(curPoint).id == map_(point).protoId && !forward)
                {
                    // Произошел обратный ход
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
                        
                        // Анализируем шаг, на котором оказались
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
                    checkAddId(point, curPoint);
                    --step;
                    forward  = false;
                    curPoint = path_[step];
                }
                ++count;
            }
        }
        
        // Если не смогли построить, то удаляем остатки пути
        if (!isFinished)
        {
            deletePath(point);
        }
    }
}

// (8) Удаляет путь с началом в точке "point"
void Solver::deletePath(Vector point)
{
    Vector iPoint;
    Area   area;
    
    // Создаем область удаления
    createCheckingArea(point, area);
    
    // Проходим по области поиска, ищем соседей
    for (iPoint.y = area.minY; iPoint.y <= area.maxY; ++iPoint.y)
    {
        for (iPoint.x = area.minX; iPoint.x <= area.maxX; ++iPoint.x)
        {
            // Если точка занята, то отчищаем ее
            if (map_(iPoint).id == map_(point).protoId)
            {
                map_(iPoint).id  = 0;
                map_(iPoint).num = 0;
                map_(iPoint).way = NONE;
            }
        }
    }
}

// (8) Удаляет путь с номером "id"
void Solver::deletePath(int16_t id)
{
    if (id > 0)
    {
        deletePath(getPosition(id));
    }
}

// (10) Удаляет мешающий путь и добавляет его начало в стек 
void Solver::deleteRandomPath(bool& wasDeleted)
{
    if (nId_ > 0)
    {
        // Выбираем случайный мешающий путь для удаления
        int16_t delIdNum = rand() % nId_;
        //Vector  delPosition = getPosition(idArray_[delIdNum]);
        
        // Добавляем начало мешающего пути в стек ожидания
        //waiting_.push(delPosition);
        
        pushDeleted(idArray_[delIdNum]);
        
        // Удаляем мешающий путь на поле
        deletePath(idArray_[delIdNum]);
        
        // Убрать "id" пути из массива мешающих "id"
        for (int16_t i = delIdNum; i < nId_ - 1; ++i)
        {
            idArray_[i] = idArray_[i + 1];
        }
        --nId_;
        
        // Успешно завершаем
        wasDeleted = true;
    }
    else
    {
        wasDeleted = false;
    }
}

// (9) Возвращает количество непостроенных путей
int16_t Solver::getMisPathNumber()
{
    int16_t nMisPaths = 0;
    
    // Считаем кол-во непостроенных путей из точек "points_"
    for (int i = 0; i < points_.size(); ++i)
    {
        if (map_(points_[i]).id == 0)
        {
            ++nMisPaths;
        }
    }
    return nMisPaths / 2;
}

// (10) Проверяет наличие соседей точки "point"
bool Solver::haveFriends(Vector point)
{
    Vector iPoint;
    Area   area;
    bool   haveFriend = false;
    
    // Создаем область поиска
    createCheckingArea(point, area);
    
    // Проходим по области поиска, ищем соседей
    for (iPoint.y = area.minY; iPoint.y <= area.maxY; ++iPoint.y)
    {
        for (iPoint.x = area.minX; iPoint.x <= area.maxX; ++iPoint.x)
        {
            if (areRelatedPoints(point, iPoint))
            {
                haveFriend = true;
            }
        }
    }
    return haveFriend;
}

// (11) Проверяет наличие соседей во всех клетках > 1
bool Solver::isCorrectRelations()
{
    Vector iPoint;
    bool   haveRel = true;
    
    // Проходим по области поиска, ищем соседей
    for (iPoint.y = 0; iPoint.y < map_.getSizeY(); ++iPoint.y)
    {
        for (iPoint.x = 0; iPoint.x < map_.getSizeX(); ++iPoint.x)
        {
            if (map_(iPoint).index > 1)
            {
                haveRel = haveRel && haveFriends(iPoint);
            }
        }
    }
    return haveRel;
}

// (12) Проверить, что непустые клетки (>= 2) образуют пары
bool Solver::isCorrectNumber()
{
    bool                 isNormNum = true;
    std::vector<int16_t> numbers;
    
    // Составляем вектор всех возможных значений индексов
    numbers.resize(MAX_INDEX + 1, 0);
    
    // Записываем, сколько клеток поля соотв. каждому индексу
    for (int j = 0; j < map_.getSizeY(); ++j)
    {
        for (int i = 0; i < map_.getSizeX(); ++i)
        {
            ++numbers[map_(i, j).index];
        }
    }
    
    // Проверяем, что у каждого индекса из поля есть пара
    for (int i = 2; i < numbers.size(); ++i)
    {
        isNormNum = isNormNum && (numbers[i] % 2 == 0);
    }
    return isNormNum;
}

// (13) Устанавливает направление обхода в точке "point"
void Solver::setDirection(Vector point)
{
    if (map_(point).id != 0 && map_(point).index == 0)
    {
        bool haveL = false;
        bool haveR = false;
        bool haveU = false;
        bool haveD = false;
        
        // Ищем предыдущую и следующую точки пути
        findNear(point, haveL, haveR, haveU, haveD);
        
        // Записываем направление в точке "point"
        map_(point).way = getDirection(haveL, haveR, haveU, haveD);
    }
    else
    {
        map_(point).way = NONE;
    }
}

// (14) Находит предыдущую и следующую точки пути
void Solver::findNear(Vector dot, bool& hL, bool& hR, bool& hU, bool& hD)
{
    bool temp1 = false;
    bool temp2 = false;
    bool temp3 = false;
    
    // Устанавливаем положение соседних точек пути (если есть)
    if (map_.inRange(dot.x - 1, dot.y))
    {
        temp1 = map_(dot.x - 1, dot.y).id == map_(dot).id;
        temp2 = map_(dot).num + 1 == map_(dot.x - 1, dot.y).num;
        temp3 = map_(dot).num - 1 == map_(dot.x - 1, dot.y).num;
        hL = temp1 && (temp2 || temp3);
    }
    if (map_.inRange(dot.x + 1, dot.y))
    {
        temp1 = map_(dot.x + 1, dot.y).id == map_(dot).id;
        temp2 = map_(dot).num + 1 == map_(dot.x + 1, dot.y).num;
        temp3 = map_(dot).num - 1 == map_(dot.x + 1, dot.y).num;
        hR = temp1 && (temp2 || temp3);
    }
    if (map_.inRange(dot.x, dot.y + 1))
    {
        temp1 = map_(dot.x, dot.y + 1).id == map_(dot).id;
        temp2 = map_(dot).num + 1 == map_(dot.x, dot.y + 1).num;
        temp3 = map_(dot).num - 1 == map_(dot.x, dot.y + 1).num;
        hU = temp1 && (temp2 || temp3);
    }
    if (map_.inRange(dot.x, dot.y - 1))
    {
        temp1 = map_(dot.x, dot.y - 1).id == map_(dot).id;
        temp2 = map_(dot).num + 1 == map_(dot.x, dot.y - 1).num;
        temp3 = map_(dot).num - 1 == map_(dot.x, dot.y - 1).num;
        hD = temp1 && (temp2 || temp3);
    }
}

// (15) Возвращает направление пути в точке по ее соседям
Direction Solver::getDirection(bool hL, bool hR, bool hU, bool hD)
{
    // Устанавливаем направление в точке
    if ((hU || hD) && hL)
    {
        return LEFT;
    }
    else
    if ((hU || hD) && hR)
    {
        return RIGHT;
    }
    else
    if (hL && hR)
    {
        return MIDDLE;
    }
    else
    if (hU && hD)
    {
        return VERTICAL;
    }
    
    // В противном случае
    return NONE;
}

// (16) Добавляет новый "id" в массив всех "id" области
void Solver::addId(int16_t id)
{
    if (id > 0)
    {
        bool haveMatch = false;
        
        // Ищем совпадения в массиве "id"
        for (int16_t i = 0; i < nId_ && !haveMatch; ++i)
        {
            haveMatch = (haveMatch || id == idArray_[i]);
        }
        
        // Если нет совпадений, то добавляем
        if (!haveMatch)
        {
            idArray_[nId_] = id;
            ++nId_;
        }
    }
}

// (17) Добавляет "id" точки "curPoint" в список всех "id"
void Solver::checkAddId(Vector point, Vector curPoint)
{
    // Проверяем, мешает ли "curPoint" проведению пути
    bool b1 = (map_(curPoint).index == 0);
    bool b2 = (map_(curPoint).index == map_(point).index);
    bool b3 = (map_(curPoint).id != 0);
    bool b4 = (map_(curPoint).id != map_(point).protoId);
    
    // Добавляем "id" пути, если он мешает
    if ((b1 || b2) && b3 && b4)
    {
        addId(map_(curPoint).id);
    }
}

// () Возвращает координаты точки с таким "protoId"
Vector Solver::getPosition(int16_t id)
{
    Vector point;
    
    // "id" корректно задан?
    if (id > 0 && id <= map_.getSizeX() * map_.getSizeY())
    {
        --id;
        
        // Ищем целую часть от деления и остаток
        while (id >= map_.getSizeX())
        {
            id -= map_.getSizeX();
            ++point.y;
        }
        point.x = id;
    }
    else
    {
        throw err::PcsException(0);
    }
    return point;
}

// () Записывает в стек все нетривиальные точки
void Solver::fillStack()
{
    // Опустошаем стек
    while (!waiting_.empty())
    {
        waiting_.pop();
    }
    
    // Переставляем (случайно) нетривиальные точки
    createPermutation(points_);
    
    // Записываем в стек все нетривиальные точки
    for (int16_t i = 0; i < points_.size(); ++i)
    {
        waiting_.push(points_[i]);
    }
}

// () Возвращает в стек начало и конец удаленного пути
void Solver::pushDeleted(int16_t id)
{
    Vector iPoint;
    
    // Проходим по области поиска, ищем соседей
    for (iPoint.y = 0; iPoint.y < map_.getSizeY(); ++iPoint.y)
    {
        for (iPoint.x = 0; iPoint.x < map_.getSizeX(); ++iPoint.x)
        {
            if (map_(iPoint).index > 2 && map_(iPoint).id == id)
            {
                waiting_.push(iPoint);
            }
        }
    }
}