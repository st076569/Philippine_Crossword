#include "solvelib.h"
#include "rwconfig.h"
#include <iostream>
#include <ctime>

int main()
{
    pcs::Field  testField;
    pcs::Solver testSolver;
    bool        flag1 = true;
    bool        flag2 = true;
    pcs::Area   area;
    std::ifstream inFile;
    std::ofstream outFile;
    
    srand(time(0));
    
    try
    {
        inFile.open("testField1.txt", std::ios::in);
        rwc::scanf(inFile, testField);
        inFile.close();
        
        testSolver.setField(testField);
        if (testSolver.isCorrectField())
        {
            testSolver.prepare();
            testSolver.solve(flag1);
            if (flag1)
            {
                std::cout << " Done!";
            }
            std::cout << "\n End!";
        }
        
        testSolver.setDirections();
        testSolver.getField(testField);
        
        outFile.open("testFieldOut.txt", std::ios::out);
        rwc::printf(outFile, testField);
        outFile.close();
    }
    catch (const err::Exception& ex)
    {
        std::cerr << "Error : " << ex.what() << " (" << ex.error() << ")\n";
    }
    catch (...)
    {
        std::cerr << "Error : unknown!";
    }
    
    return 0;
}
