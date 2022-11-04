// SmartBrain.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include "RandomGo2d.h"
#include <time.h>
#include <stdlib.h>
#include "DucksInPool.h"
#include "JingGame.h"
#include "Prisoner100.h"
#include "BoyBoy.h"
#include "LetsMakeaDeal.h"
#define GoTimes  10000
int main()
{
    srand(time(0));
    std::cout << "Hello World!\n";
    //RandomGo2d myTest ;
    DucksInPool myTest;
    //JingGame myTest;
    //Prisoner100 myTest;
    //BoyBoy myTest;
    //LetsMakeaDeal myTest;
    myTest.test(GoTimes);
}
