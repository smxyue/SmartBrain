// SmartBrain.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <time.h>
#include <stdlib.h>
#include "Test.h"
#define GoTimes  10
int main()
{
    //srand(time(0));
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
    Test myTest;
    //for(int i=0;i<243;i++)
            int** m=myTest.getit();
            myTest.showit(m);
}
