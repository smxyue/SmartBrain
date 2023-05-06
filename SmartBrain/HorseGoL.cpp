#include "HorseGoL.h"

#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#define MAX_TRY 9999999
using namespace std;



// �жϵ�ǰλ���Ƿ�Խ��
bool HorseGoL::isValid(int x, int y) {
    return x >= 0 && x < N&& y >= 0 && y < N;
}

// �����̽����
bool HorseGoL::randomWalk(int x, int y) {
    memset(visited, false, sizeof(visited)); // ��ʼ��visited����
    visited[x][y] = true;
    int count = 1; // �Ѿ�������λ����
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
        if (choices.empty()) { // ���û�п�ѡ��λ�ã�����false
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
            index = rand() % choices.size(); // ���ѡ��һ����ѡ��λ��
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
        return true; // ����Ѿ����������е�λ�ã�����true
    return false;
}

int HorseGoL::main() {
    for(int x=0;x<8;x++)
        for (int y = 0;y < 8;y++)
        {
            if (randomWalk(x, y)) { // ��������Ͻ��ܹ��������е�λ�ã�����ɹ���Ϣ
                cout << "��";
            }
            else { // �������ʧ����Ϣ
                cout << "x";
            }
        }
    cout <<" " << '\t';
    
    return 0;
}