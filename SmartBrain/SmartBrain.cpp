// SmartBrain.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <time.h>
#include <stdlib.h>
#include "Test.h"
#include "Robi3.h"
#include "RouletteSelect.h"
#include "pyGen.h"

#define GoTimes  10
int main()
{
    srand(time(NULL));
    std::cout << "Hello World!\n";
    //RandomGo2d myTest ;
    //DucksInPool myTest;
    //JingGame myTest;
    //Prisoner100 myTest;
    //BoyBoy myTest;
    //LetsMakeaDeal myTest;
    //Tetravex myTest;
    //SafeBoxPwd myTest;
    //RandomTest myTest;
    Robi3 myTest;
    //for(int i=0;i<243;i++)
    //for(int i=0;i<10;i++)
        myTest.main();
}
