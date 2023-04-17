#pragma once
#include <random>
#define NNN  10
#define MMM  10

using namespace std;
random_device rd;
mt19937 gen(rd());

class CluadeRobi
{

    // ���Ա�,�����СΪ243(5��3�η�),ÿ��Ԫ�ش���һ�ֲ���
    static int policyTable[243];

    // �����˵�ǰλ�ú���Χ5��λ�õ�״̬,��3���Ʊ�ʾ
    static int curStatus;

    // ��ǰ�÷� 
    static int  score;

    // ���Ӵ�С���������� 
    int garbageNum = 50;

    // �Ŵ��㷨���� 
    int popSize = 100;        // ��Ⱥ��С 
    int eliteSize = 20;        // ��Ӣ����
    int mutationRate = 0.1;   // ������
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

