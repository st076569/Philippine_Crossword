////////// rwconfig //////////////////////////////////////////////////////////
// Автор    : Баталов Семен                                                 //
// Дата     : 08.11.2020                                                    //
// Описание : Здесь представлены инструменты для записи в текстовый файл    //
//            схемы филиппинского кроссворда и для ее чтения из него.       //
//////////////////////////////////////////////////////////////////////////////

#include "rwconfig.h"

////////// void scanf ////////////////////////////////////////////////////////
// Описание : rwconfig.h                                                    //
//////////////////////////////////////////////////////////////////////////////
    
bool rwc::isEqual(char ch1, char ch2, bool reg)
{
    // Равенство с учетом регистра
    bool equal1 = (ch1 == ch2);
    
    // Равенство без учета регистра
    bool equal2 = ((ch1 + ' ' == ch2) || (ch2 + ' ' == ch1));
    
    // Проверка равенства
    return !reg && (equal1 || equal2) || reg && equal1;
}

void rwc::scanf(std::ifstream& inf, int16_t& value)
{
    if (inf.is_open() && !inf.eof() && !inf.fail())
    {
        inf >> value;
        if (inf.fail())
        {
            inf.clear();
            throw err::RwcException(0);
        }
    }
    else
    {
        if (inf.fail())
        {
            inf.clear();
        }
        throw err::RwcException(2);
    }
}
    
void rwc::scanf(std::ifstream& inf, const char* name, bool reg)
{
    if (inf.is_open() && !inf.eof() && !inf.fail())
    {
        std::string readStr;
        std::string nameStr    = name;
        bool        isEqualStr = false;
        
        // Читаем строку и сравниваем с "name"
        inf >> readStr;
        if (inf.fail())
        {
            inf.clear();
            throw err::RwcException(0);
        }
        
        // Сравниваем строки
        if (nameStr.length() == readStr.length())
        {
            isEqualStr = true;
            
            // Сравниваем строки с точностью до регистра
            for (int i = 0; i < nameStr.length() && isEqualStr; ++i)
            {
                isEqualStr = isEqual(nameStr[i], readStr[i], reg);
            }
        }
        
        // Если нет равенства, то прерываем работу
        if (!isEqualStr)
        {
            throw err::RwcException(3);
        }
    }
    else
    {
        if (inf.fail())
        {
            inf.clear();
        }
        throw err::RwcException(2);
    }
}

////////// void scanf "Field" ////////////////////////////////////////////////
// Описание : rwconfig.h                                                    //
//////////////////////////////////////////////////////////////////////////////
    
void rwc::scanf(std::ifstream& inf, pcs::Field& field)
{
    if (inf.is_open() && !inf.eof() && !inf.fail())
    {
        int16_t sizeX = 0;
        int16_t sizeY = 0;
        
        // Читаем размер
        scanf(inf, SIZE_COMMAND, false);
        scanf(inf, sizeX);
        scanf(inf, sizeY);
        
        // Изменяем размер поля в соответствии с данными из файла
        scanf(inf, FIELD_COMMAND, false);
        
        // Проверка выхода значений за допустимые границы
        try
        {
            field.resize(sizeX, sizeY);
        }
        catch (const err::PcsException& ex)
        {
            throw err::RwcException(1);
        }
        catch (...)
        {
            throw;
        }
        
        // Читаем все индексы поля
        for (int j = 0; j < sizeY; ++j)
        {
            for (int i = 0; i < sizeX; ++i)
            {
                scanf(inf, field(i, j).index);
                
                // Если значение вышло за границы
                if (!pcs::inRangeIndex(field(i, j).index))
                {
                    throw err::RwcException(1);
                }
            }
        }
    }
    else
    {
        if (inf.fail())
        {
            inf.clear();
        }
        throw err::RwcException(2);
    }
}
    
////////// void printf "Field" ///////////////////////////////////////////////
// Описание : rwconfig.h                                                    //
//////////////////////////////////////////////////////////////////////////////

void rwc::printf(std::ofstream& outf, pcs::Field& field)
{
    if (outf.is_open() && !outf.fail())
    {
        outf.fill('#');
        if (field.getSizeX() > 0 && field.getSizeY() > 0)
        {
            // Верхняя разделяющая
            outf << "///";
            for (int i = 0; i < field.getSizeX(); ++i)
            {
                outf << "////";
            }
            outf << "//\n";
            
            // Вывод содержимого поля
            for (int j = 0; j < field.getSizeY(); ++j)
            {
                outf << "// ";
                
                // Вывод строки поля
                for (int i = 0; i < field.getSizeX(); ++i)
                {
                    switch (field(i, j).way)
                    {
                        case pcs::LEFT :
                            outf << LEFT_IMAGE << ' ';
                            break;
                        
                        case pcs::RIGHT :
                            outf << RIGHT_IMAGE << ' ';
                            break;
                        
                        case pcs::VERTICAL :
                            outf << VERTICAL_IMAGE << ' ';
                            break;
                        
                        case pcs::MIDDLE :
                            outf << MIDDLE_IMAGE << ' ';
                            break;
                        
                        default :
                            if (field(i, j).index == 0)
                            {
                                outf << "000 ";
                            }
                            else
                            {
                                outf << std::setw(3) << field(i, j).index;
                                outf << ' ';
                            }
                            break;
                    }
                }
                outf << "//\n";
            }
            
            // Нижняя разделяющая
            outf << "///";
            for (int i = 0; i < field.getSizeX(); ++i)
            {
                outf << "////";
            }
            outf << "//\n";
        }
        outf.fill(' ');
    }
    else
    {
        if (outf.fail())
        {
            outf.clear();
        }
        throw err::RwcException(2);
    }
}