#pragma once
#define POP_SIZE 100
#define GENE_SIZE 243
#define MAX_ITERATION 500
#define CROSSOVER_RATE 0.8
#define MUTATION_RATE 0.02
#define FITNESS_THRESHOLD 1800


class Robi3
{
    int row = 0;
    int col = 0;
    /* ���������״̬�ṹ�� */
    struct RobotState {
        int up, left, curr, right, down;
    };

    /* �������״̬���ͣ�0��ʾ�գ�1��ʾ������2��ʾ�߽磩 */
    typedef enum { EMPTY = 0, TRASH = 1, BOUNDARY = 2 } CellState;
    typedef enum{UP,LEFT,RIGHT,DOWN,PICK} ActionCode;

    /* ����ȫ�ֱ���������״̬���顢�����˵�ǰ״̬ */
    CellState cells[10][10];
    struct RobotState robot;

    /* �����Ŵ��㷨��Ҫ�õ��ĺ��� */
    int* generate_random_gene();
    int** generate_initial_population();
    int getCurrentState();
    int evaluate_fitness(int* gene);
    void selection(int** population, int* fitness);
    void crossover(int* parent1, int* parent2, int* child1, int* child2);
    void mutation(int* gene);

    void initCells();
    void printCells();
public:
    int main();
};