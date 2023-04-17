#include "CluadeRobi.h"
#include <iostream>
#include <vector> 
#include <random>
#include <algorithm>

// 初始化策略表 
void CluadeRobi::initPolicyTable() {
    for (int i = 0; i < 243; i++) {
        policyTable[i] = gen() % 6;  // 随机生成0-5之间的策略
    }
}

// 获取机器人当前状态
int CluadeRobi::getStatus() {
    int status = 0;
    for (int i = 0; i < 5; i++) {
        int val;
        if (i == 2) val = 1;   // 中心位置为机器人自身,恒为1
        else {
            int row = i / 3 - 1;   // 计算相对的行,列
            int col = i % 3 - 1;
            if (row >= 0 && row < n && col >= 0 && col < m)
                val = (grid[row][col] == 1);   // 相邻位置是否有垃圾
            else
                val = 2;   // 超出边界,表示为2
        }
        status = status * 3 + val;  // 将相邻5个位置的状态进行3进制拼接
    }
    return status;
}

// 根据策略表确定机器人下一步行动
int CluadeRobi::getAction(int status) {
    return policyTable[status];
}

// 机器人行动,并更新其位置和得分 
void CluadeRobi::move(int action) {
    int row = curRow;
    int col = curCol;
    switch (action) {
    case 0: row--; break;      // 上
    case 1: row++; break;      // 下
    case 2: col--; break;      // 左
    case 3: col++; break;      // 右
    case 4: {                   // 捡垃圾
        grid[row][col] = 0;    // 设置当前位置为0,表示捡到垃圾
        score += 10;           // 得分增加10分
        break;
    }
    case 5:                    // 不动
        break;
    }
    // 检查是否出界或撞墙,并相应的惩罚和位置修正
    if (row < 0 || row >= n || col < 0 || col >= m) {
        score -= 5;
        row = curRow;
        col = curCol;
    }
    curRow = row;
    curCol = col;
}

// 遗传算法的进化过程
void CluadeRobi::evolve() {
    vector<int> policyTables;      // 种群,存储若干策略表

    // 1. 初始化种群
    for (int i = 0; i < popSize; i++) {
        initPolicyTable();        // 随机生成一份策略表
        policyTables.push_back(policyTable);
    }

    // 2. 评估种群并选择精英 
    sort(policyTables.begin(), policyTables.end(),     // 按照得分排序种群
        [](const int* a, const int* b) {return score(a) > score(b);});
    vector<int> eliteTables;   // 精英策略表 
    for (int i = 0; i < eliteSize; i++) {
        eliteTables.push_back(policyTables[i]);
    }

    // 3. 精英随机交叉变异产生下一代 
    vector<int> nextGen;
    for (int i = 0; i < popSize; i++) {
        // 以一定概率选取精英进行变异 
        if (gen() % 100 > mutationRate * 100) {
            nextGen.push_back(eliteTables[gen() % eliteSize]);
        }
        else {           // 否则交叉变异产生新的策略表
            int table1 = eliteTables[gen() % eliteSize];
            int table2 = eliteTables[gen() % eliteSize];
            // 随机交叉
            for (int j = 0; j < 243; j++) {
                if (gen() % 2 == 0) policyTable[j] = table1[j];
                else policyTable[j] = table2[j];
            }
            // 随机变异
            for (int j = 0; j < 243; j++) {
                if (gen() % 100 < mutationRate * 100) {
                    policyTable[j] = gen() % 6;     // 变异为0-5之间的随机值
                }
            }
            nextGen.push_back(policyTable);
        }
    }
    policyTables = nextGen;  // 下一代种群替换当前种群
}

// 评估一份策略表的得分
int CluadeRobi::score(const int* policyTable) {
    // 用policyTable代替全局策略表
    for (int i = 0; i < 243; i++) {
        ::policyTable[i] = policyTable[i];
    }

    int totalScore = 0;   // 总得分
    // 每次从左上角开始清扫
    curRow = 0;
    curCol = 0;
    curStatus = getStatus();  // 获取初始状态
    for (int i = 0; i < 200; i++) {  // 清扫200步
        int action = getAction(curStatus);    // 根据策略表和当前状态选择行动
        move(action);           // 机器人行动
        curStatus = getStatus();   // 更新当前状态
        totalScore += score;        // 总得分累加
    }
    score = 0;   // 将全局得分重置为0
    return totalScore;
}

int CluadeRobi::main()
{
        initPolicyTable();    // 初始化策略表

        // 遗传算法迭代进化,选择最优策略表 
        for (int i = 0; i < 1000; i++) {
            evolve();
        }

        cout << "最优策略表:" << endl;
        for (int i = 0; i < 243; i++) {
            cout << ::policyTable[i] << " ";
            if ((i + 1) % 5 == 0) cout << endl;  // 每行输出5个策略
        }
    }

