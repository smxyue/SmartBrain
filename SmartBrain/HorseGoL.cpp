#include "HorseGoL.h"

#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#define MAX_TRY 9999999
using namespace std;



// 判断当前位置是否越界
bool HorseGoL::isValid(int x, int y) {
    return x >= 0 && x < N&& y >= 0 && y < N;
}

// 随机试探函数
bool HorseGoL::randomWalk(int x, int y) {
    memset(visited, false, sizeof(visited)); // 初始化visited数组
    visited[x][y] = true;
    int count = 1; // 已经遍历的位置数
    int tryed = 0;
    cout << "(" << x << "," << y << "):";
    while (count < N * N && tryed < MAX_TRY) 
    {
        vector<pair<int, int>> choices;
        for (pair<int, int> move:moves) {
            int nx = x + move.first, ny = y + move.second;
            if (isValid(nx, ny)) {
                choices.push_back({ nx, ny });
            }
        }
        if (choices.empty()) { // 如果没有可选的位置，返回false
            return false;
        }
        int index = -1;
        for (int i = 0;i < choices.size();i++)
            if (visited[choices[i].first][choices[i].second]==false)
            {
                index = i;
                break;
            }
        if (index == -1)
            index = rand() % choices.size(); // 随机选择一个可选的位置
        x = choices[index].first;
        y = choices[index].second;
        if (visited[x][y] == false)
        {
            visited[x][y] = true;
            count++;
        }
        tryed++;
    }
    cout  << tryed;
    if (count==N*N)
        return true; // 如果已经遍历了所有的位置，返回true
    return false;
}

int HorseGoL::main() {
    for(int x=0;x<8;x++)
        for (int y = 0;y < 8;y++)
        {
            if (randomWalk(x, y)) { // 如果从左上角能够遍历所有的位置，输出成功信息
                cout << "√";
            }
            else { // 否则输出失败信息
                cout << "x";
            }
        }
    cout <<" " << '\t';
    
    return 0;
}