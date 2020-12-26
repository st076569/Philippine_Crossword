#include "solvelib.h"
#include "rwconfig.h"
#include <iostream>
#include <ctime>

int main()
{
    pcs::Field  testField;
    pcs::Solver testSolver;
    pcs::TrackSeq seq;
    pcs::Vector test;
    bool        flag1 = true;
    bool        flag2 = true;
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
            testSolver.solve(flag1);
            if (flag1)
            {
                std::cout << "\n Done!";
            }
            std::cout << "\n End!";
            testSolver.setDirections();
            testSolver.getField(testField);
        }
        
        /*
        if (testSolver.isCorrectField())
        {
            testSolver.prepare();
            std::cout << "\n";
            for (int i = 0; i < testSolver.points_.size(); ++i)
            {
                std::cout << std::setw(2) << testSolver.points_[i].x << "; ";
            }
            std::cout << "\n";
            for (int i = 0; i < testSolver.points_.size(); ++i)
            {
                std::cout << std::setw(2) << testSolver.points_[i].y << "; ";
            }
            std::cout << "\n";
            
            testSolver.createPermutation();
            std::cout << "\n";
            for (int i = 0; i < testSolver.points_.size(); ++i)
            {
                std::cout << std::setw(2) << testSolver.points_[i].x << "; ";
            }
            std::cout << "\n";
            for (int i = 0; i < testSolver.points_.size(); ++i)
            {
                std::cout << std::setw(2) << testSolver.points_[i].y << "; ";
            }
            std::cout << "\n";
            
            testSolver.createPermutation();
            std::cout << "\n";
            for (int i = 0; i < testSolver.points_.size(); ++i)
            {
                std::cout << std::setw(2) << testSolver.points_[i].x << "; ";
            }
            std::cout << "\n";
            for (int i = 0; i < testSolver.points_.size(); ++i)
            {
                std::cout << std::setw(2) << testSolver.points_[i].y << "; ";
            }
            std::cout << "\n";
            
            testSolver.createPermutation();
            std::cout << "\n";
            for (int i = 0; i < testSolver.points_.size(); ++i)
            {
                std::cout << std::setw(2) << testSolver.points_[i].x << "; ";
            }
            std::cout << "\n";
            for (int i = 0; i < testSolver.points_.size(); ++i)
            {
                std::cout << std::setw(2) << testSolver.points_[i].y << "; ";
            }
            std::cout << "\n";
        }
        */
        
        outFile.open("testFieldOut.txt", std::ios::out);
        rwc::printf(outFile, testField);
        outFile.close();
    }
    catch (const err::Exception& ex)
    {
        std::cerr << "\n Error : " << ex.error();
    }
    
    return 0;
}
