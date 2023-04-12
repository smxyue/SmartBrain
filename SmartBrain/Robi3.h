#pragma once
#define POP_SIZE 100
#define GENE_SIZE 243
#define MAX_ITERATION 500
#define CROSSOVER_RATE 0.8
#define MUTATION_RATE 0.02
#define FITNESS_THRESHOLD 1800


class Robi3
{
    /* 定义机器人状态结构体 */
    struct RobotState {
        int up, left, curr, right, down;
    };

    /* 定义格子状态类型（0表示空，1表示垃圾，2表示边界） */
    typedef enum { EMPTY = 0, TRASH = 1, BOUNDARY = 2 } CellState;

    /* 声明全局变量：格子状态数组、机器人当前状态 */
    CellState cells[50][50];
    struct RobotState robot;

    /* 声明遗传算法需要用到的函数 */
    int* generate_random_gene();
    int** generate_initial_population();
    int evaluate_fitness(int* gene);
    void selection(int** population, int* fitness);
    void crossover(int* parent1, int* parent2, int* child1, int* child2);
    void mutation(int* gene);
    int main();
};