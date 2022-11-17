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
#include "Tetravex.h"
#include "SafeBoxPwd.h"
#include "RandomTest.h"
#include "Robi.h"
#define GoTimes  10
int main()
{
    srand(time(0));
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
    Robi myTest;
    for(int i=0;i<10;i++)
        myTest.myGo();
}
