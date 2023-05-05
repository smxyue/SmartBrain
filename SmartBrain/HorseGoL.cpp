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
    while (count < N * N && tryed < MAX_TRY) 
    {
        vector<pair<int, int>> choices;
        for (pair<int, int> move:moves) {
            int nx = x + move.first, ny = y + move.second;
            if (isValid(nx, ny) && !visited[nx][ny]) {
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
    return true; // ����Ѿ����������е�λ�ã�����true
}

int HorseGoL::main() {
    srand(time(NULL)); // �������������
    if (randomWalk(0, 0)) { // ��������Ͻ��ܹ��������е�λ�ã�����ɹ���Ϣ
        cout << "����Ծ������е�λ��" << endl;
    }
    else { // �������ʧ����Ϣ
        cout << "���ܾ������е�λ��" << endl;
    }
    return 0;
}