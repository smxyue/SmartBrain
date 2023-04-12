#pragma once
#define POP_SIZE 100
#define GENE_SIZE 243
#define MAX_ITERATION 500
#define CROSSOVER_RATE 0.8
#define MUTATION_RATE 0.02
#define FITNESS_THRESHOLD 1800


class Robi3
{
    /* ���������״̬�ṹ�� */
    struct RobotState {
        int up, left, curr, right, down;
    };

    /* �������״̬���ͣ�0��ʾ�գ�1��ʾ������2��ʾ�߽磩 */
    typedef enum { EMPTY = 0, TRASH = 1, BOUNDARY = 2 } CellState;

    /* ����ȫ�ֱ���������״̬���顢�����˵�ǰ״̬ */
    CellState cells[50][50];
    struct RobotState robot;

    /* �����Ŵ��㷨��Ҫ�õ��ĺ��� */
    int* generate_random_gene();
    int** generate_initial_population();
    int evaluate_fitness(int* gene);
    void selection(int** population, int* fitness);
    void crossover(int* parent1, int* parent2, int* child1, int* child2);
    void mutation(int* gene);
    int main();
};