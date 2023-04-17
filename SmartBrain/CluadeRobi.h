#pragma once
#include <random>
using namespace std;
random_device rd;
mt19937 gen(rd());

class CluadeRobi
{

    // 策略表,数组大小为243(5的3次方),每个元素代表一种策略
    int policyTable[243];

    // 机器人当前位置和周围5个位置的状态,用3进制表示
    int curStatus;

    // 当前得分 
    int score;

    // 格子大小和垃圾数量 
    int n = 10;
    int m = 10;
    int garbageNum = 50;

    // 遗传算法参数 
    int popSize = 100;        // 种群大小 
    int eliteSize = 20;        // 精英数量
    int mutationRate = 0.1;   // 变异率
    int grid[10][10];
    int curRow;
    int curCol;
    int score;

    // 随机数生成器 

    void initPolicyTable();
    int getStatus();
    int getAction(int status);
    void move(int action);
    void evolve();
    int score(const int* policyTable);
public:
	int main();
};

