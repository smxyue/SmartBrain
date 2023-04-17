#include "CluadeRobi.h"
#include <iostream>
#include <vector> 
#include <random>
#include <algorithm>

// ��ʼ�����Ա� 
void CluadeRobi::initPolicyTable() {
    for (int i = 0; i < 243; i++) {
        policyTable[i] = gen() % 6;  // �������0-5֮��Ĳ���
    }
}

// ��ȡ�����˵�ǰ״̬
int CluadeRobi::getStatus() {
    int status = 0;
    for (int i = 0; i < 5; i++) {
        int val;
        if (i == 2) val = 1;   // ����λ��Ϊ����������,��Ϊ1
        else {
            int row = i / 3 - 1;   // ������Ե���,��
            int col = i % 3 - 1;
            if (row >= 0 && row < n && col >= 0 && col < m)
                val = (grid[row][col] == 1);   // ����λ���Ƿ�������
            else
                val = 2;   // �����߽�,��ʾΪ2
        }
        status = status * 3 + val;  // ������5��λ�õ�״̬����3����ƴ��
    }
    return status;
}

// ���ݲ��Ա�ȷ����������һ���ж�
int CluadeRobi::getAction(int status) {
    return policyTable[status];
}

// �������ж�,��������λ�ú͵÷� 
void CluadeRobi::move(int action) {
    int row = curRow;
    int col = curCol;
    switch (action) {
    case 0: row--; break;      // ��
    case 1: row++; break;      // ��
    case 2: col--; break;      // ��
    case 3: col++; break;      // ��
    case 4: {                   // ������
        grid[row][col] = 0;    // ���õ�ǰλ��Ϊ0,��ʾ������
        score += 10;           // �÷�����10��
        break;
    }
    case 5:                    // ����
        break;
    }
    // ����Ƿ�����ײǽ,����Ӧ�ĳͷ���λ������
    if (row < 0 || row >= n || col < 0 || col >= m) {
        score -= 5;
        row = curRow;
        col = curCol;
    }
    curRow = row;
    curCol = col;
}

// �Ŵ��㷨�Ľ�������
void CluadeRobi::evolve() {
    vector<int> policyTables;      // ��Ⱥ,�洢���ɲ��Ա�

    // 1. ��ʼ����Ⱥ
    for (int i = 0; i < popSize; i++) {
        initPolicyTable();        // �������һ�ݲ��Ա�
        policyTables.push_back(policyTable);
    }

    // 2. ������Ⱥ��ѡ��Ӣ 
    sort(policyTables.begin(), policyTables.end(),     // ���յ÷�������Ⱥ
        [](const int* a, const int* b) {return score(a) > score(b);});
    vector<int> eliteTables;   // ��Ӣ���Ա� 
    for (int i = 0; i < eliteSize; i++) {
        eliteTables.push_back(policyTables[i]);
    }

    // 3. ��Ӣ���������������һ�� 
    vector<int> nextGen;
    for (int i = 0; i < popSize; i++) {
        // ��һ������ѡȡ��Ӣ���б��� 
        if (gen() % 100 > mutationRate * 100) {
            nextGen.push_back(eliteTables[gen() % eliteSize]);
        }
        else {           // ���򽻲��������µĲ��Ա�
            int table1 = eliteTables[gen() % eliteSize];
            int table2 = eliteTables[gen() % eliteSize];
            // �������
            for (int j = 0; j < 243; j++) {
                if (gen() % 2 == 0) policyTable[j] = table1[j];
                else policyTable[j] = table2[j];
            }
            // �������
            for (int j = 0; j < 243; j++) {
                if (gen() % 100 < mutationRate * 100) {
                    policyTable[j] = gen() % 6;     // ����Ϊ0-5֮������ֵ
                }
            }
            nextGen.push_back(policyTable);
        }
    }
    policyTables = nextGen;  // ��һ����Ⱥ�滻��ǰ��Ⱥ
}

// ����һ�ݲ��Ա�ĵ÷�
int CluadeRobi::score(const int* policyTable) {
    // ��policyTable����ȫ�ֲ��Ա�
    for (int i = 0; i < 243; i++) {
        ::policyTable[i] = policyTable[i];
    }

    int totalScore = 0;   // �ܵ÷�
    // ÿ�δ����Ͻǿ�ʼ��ɨ
    curRow = 0;
    curCol = 0;
    curStatus = getStatus();  // ��ȡ��ʼ״̬
    for (int i = 0; i < 200; i++) {  // ��ɨ200��
        int action = getAction(curStatus);    // ���ݲ��Ա�͵�ǰ״̬ѡ���ж�
        move(action);           // �������ж�
        curStatus = getStatus();   // ���µ�ǰ״̬
        totalScore += score;        // �ܵ÷��ۼ�
    }
    score = 0;   // ��ȫ�ֵ÷�����Ϊ0
    return totalScore;
}

int CluadeRobi::main()
{
        initPolicyTable();    // ��ʼ�����Ա�

        // �Ŵ��㷨��������,ѡ�����Ų��Ա� 
        for (int i = 0; i < 1000; i++) {
            evolve();
        }

        cout << "���Ų��Ա�:" << endl;
        for (int i = 0; i < 243; i++) {
            cout << ::policyTable[i] << " ";
            if ((i + 1) % 5 == 0) cout << endl;  // ÿ�����5������
        }
    }

