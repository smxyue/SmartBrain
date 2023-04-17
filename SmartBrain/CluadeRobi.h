#pragma once
#include <random>
#define NNN  10
#define MMM  10

using namespace std;
random_device rd;
mt19937 gen(rd());

class CluadeRobi
{

    // 策略表,数组大小为243(5的3次方),每个元素代表一种策略
    static int policyTable[243];

    // 机器人当前位置和周围5个位置的状态,用3进制表示
    static int curStatus;

    // 当前得分 
    static int  score;

    // 格子大小和垃圾数量 
    int garbageNum = 50;

    // 遗传算法参数 
    int popSize = 100;        // 种群大小 
    int eliteSize = 20;        // 精英数量
    int mutationRate = 0.1;   // 变异率
    static int grid[10][10];
    static int curRow;
    static int curCol;

    void initPolicyTable();
    static int getStatus();
    static int getAction(int status);
    static void move(int action);
    void evolve();
    int static caclScore(const int* policyTable);
public:
	int main();
};

