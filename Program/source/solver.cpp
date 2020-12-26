////////// Philippine solver /////////////////////////////////////////////////
// Автор    : Баталов Семен                                                 //
// Дата     : 08.11.2020                                                    //
// Описание : Данная программма предназначена для решения филиппинского     //
//            кроссворда.                                                   //
//////////////////////////////////////////////////////////////////////////////

#include "instrumentslib.h"
#include "solvelib.h"
#include "rwconfig.h"
#include <ctime>

namespace pcs
{
    // Команда выхода из меню
    const std::string comEscape = "escape";
    
    // Записывает правила пользования
    void instruction()
    {
        std::cout << '\n';
        std::cout << "////////// Philippine solver ///////////////////////////////////\n";
        std::cout << "// !!! Write 'escape', if you want to close the programm !!!  //\n";
        std::cout << "//                                                            //\n";
        std::cout << "// Inf  : This programm provides instruments for solving      //\n";
        std::cout << "//        philippine crossword.                               //\n";
        std::cout << "//                                                            //\n";
        std::cout << "// Plan : 1) Write 'input file name' including your map.      //\n";
        std::cout << "//        2) The program starts automatically.                //\n";
        std::cout << "//        3) Choose 'output file name' for your 'solved' map. //\n";
        std::cout << "////////////////////////////////////////////////////////////////\n";
        std::cout << '\n';
    }
    
    // Читает входные данные
    void scanCommand(std::string& input, bool& wasExit)
    {
        std::cin >> input;
        if (std::cin.fail())
        {
            std::cin.clear();
            throw err::RwcException(2);
        }
        
        // Сравниваем строки
        if (input.length() == comEscape.length())
        {
            // Предварительная инициализация
            wasExit = true;
            
            // Сравниваем строки с точностью до регистра
            for (int i = 0; i < input.length() && wasExit; ++i)
            {
                wasExit = rwc::isEqual(input[i], comEscape[i], false);
            }
        }
        else
        {
            wasExit = false;
        }
    }
    
    // Открывает входной файл
    void openInf(std::ifstream& inf, bool& wasExit)
    {
        std::string fileName;
        bool        wasOpened = false;
        
        // Предварительная инициализация
        wasExit = false;
        
        // Пока не откроем файл или не выйдем
        while (!wasExit && !wasOpened)
        {
            std::cout << "Enter 'input file name' : ";
            scanCommand(fileName, wasExit);
            
            if (!wasExit)
            {
                char name[fileName.length() + 1];
                
                // Записали строку С-style
                for (int i = 0; i < fileName.length(); ++i)
                {
                    name[i] = fileName[i];
                }
                
                // Открываем файл
                inf.open(name);
                if (!inf.fail())
                {
                    wasOpened = true;
                }
                else
                {
                    inf.clear();
                    std::cout << "Can not open file ";
                    std::cout << "'" << fileName << "', try again!\n";
                }
            }
        }
    }
    
    // Запускает процедуру решения кроссворда
    void run(pcs::Solver& solver, bool& wasSolved)
    {
        float time1 = 0;
        float time2 = 0;
        
        if (solver.isCorrectField())
        {
            // Подготовка к работе
            solver.prepare();
            std::cout << " | ";
            
            // Решаем и замеряем время
            time1 = static_cast<float> (clock()) / CLOCKS_PER_SEC;
            solver.solve(wasSolved);
            time2 = static_cast<float> (clock()) / CLOCKS_PER_SEC;
            
            // Выписываем время работы
            std::cout << " [" << time2 - time1 << " sec]";
            
            // Если не получилось решить
            if (!wasSolved)
            {
                std::cout << "\nAlgorithm steps are limited!\n";
            }
        }
        else
        {
            std::cout << "Field contain is not correct!\n";
            wasSolved = false;
        }
    }
    
    // Открывает выходной файл
    void openOutf(std::ofstream& outf, bool& wasExit)
    {
        std::string fileName;
        bool        wasOpened = false;
        
        // Предварительная инициализация
        wasExit = false;
        
        // Пока не откроем файл или не выйдем
        while (!wasExit && !wasOpened)
        {
            std::cout << "Enter 'output file name' : ";
            scanCommand(fileName, wasExit);
            
            if (!wasExit)
            {
                char name[fileName.length() + 1];
                
                // Записали строку С-style
                for (int i = 0; i < fileName.length(); ++i)
                {
                    name[i] = fileName[i];
                }
                
                // Открываем файл
                outf.open(name);
                if (!outf.fail())
                {
                    wasOpened = true;
                }
                else
                {
                    outf.clear();
                    std::cout << "Can not open file ";
                    std::cout << "'" << fileName << "', try again!\n";
                }
            }
        }
    }
    
    // Предоставляет интерфейс и решает кроссворд
    void solveCrossword()
    {
        bool          wasExit   = false;
        bool          wasSolved = false;
        std::ifstream inFile;
        std::ofstream outFile;
        pcs::Field    field;
        pcs::Solver   solver;
        
        pcs::instruction();
        while (!wasExit)
        {
            pcs::openInf(inFile, wasExit);
            if (!wasExit)
            {
                // Читаем поле из файла
                rwc::scanf(inFile, field);
                inFile.close();
                solver.setField(field);
                
                // Разводим пути по полю
                run(solver, wasSolved);
                
                // Если удалось развести
                if (wasSolved)
                {
                    std::cout << "\nSuccesfully solved!\n";
                    
                    // Открываем выходной файл
                    pcs::openOutf(outFile, wasExit);
                    
                    // Если удалось открыть
                    if (!wasExit)
                    {
                        // Устанавливаем направления обхода
                        solver.setDirections();
                        solver.getField(field);
                        
                        // Записываем разведенное поле в файл
                        rwc::printf(outFile, field);
                        outFile.close();
                    }
                }
                else
                {
                    std::cout << "Can not solve this example, try again!\n";
                }
            }
            
            // Очистка для дальнейшей работы
            solver.clear();
            field.clear();
        }
    }
}

int main()
{
    try
    {
        srand(time(0));
        pcs::solveCrossword();
    }
    catch (const err::Exception& ex)
    {
        std::cerr << "Error : " << ex.what() << " (" << ex.error() << ")\n";
    }
    catch (const std::exception& ex)
    {
        std::cerr << "Error : " << ex.what() << '\n';
    }
    catch (...)
    {
        std::cerr << "Error : Interrior unknown!";
    }
    std::cout << "Done!\n";
    
    return 0;
}
