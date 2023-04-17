#pragma once
#include <random>
using namespace std;
random_device rd;
mt19937 gen(rd());

class CluadeRobi
{

    // ���Ա�,�����СΪ243(5��3�η�),ÿ��Ԫ�ش���һ�ֲ���
    int policyTable[243];

    // �����˵�ǰλ�ú���Χ5��λ�õ�״̬,��3���Ʊ�ʾ
    int curStatus;

    // ��ǰ�÷� 
    int score;

    // ���Ӵ�С���������� 
    int n = 10;
    int m = 10;
    int garbageNum = 50;

    // �Ŵ��㷨���� 
    int popSize = 100;        // ��Ⱥ��С 
    int eliteSize = 20;        // ��Ӣ����
    int mutationRate = 0.1;   // ������
    int grid[10][10];
    int curRow;
    int curCol;
    int score;

    // ����������� 

    void initPolicyTable();
    int getStatus();
    int getAction(int status);
    void move(int action);
    void evolve();
    int score(const int* policyTable);
public:
	int main();
};

