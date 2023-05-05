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
    cout << "Start from (" << x << "," << y << ") ";
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
        int index = rand() % choices.size(); // ���ѡ��һ����ѡ��λ��
        x = choices[index].first;
        y = choices[index].second;
        if (visited[x][y] == false)
        {
            visited[x][y] = true;
            count++;
        }
        tryed++;
    }
    cout << tryed << " tried!  " ;
    if (count==N*N)
        return true; // ����Ѿ����������е�λ�ã�����true
    return false;
}

int HorseGoL::main() {
    int x = rand() % 8;
    int y = rand() % 8;
    if (randomWalk(x, y)) { // ��������Ͻ��ܹ��������е�λ�ã�����ɹ���Ϣ
        cout << "  ��" << endl;
    }
    else { // �������ʧ����Ϣ
        cout << "  x" << endl;
    }
    return 0;
}