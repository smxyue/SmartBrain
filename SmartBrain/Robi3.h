#pragma once
#define POP_SIZE 200
#define GENE_SIZE 243
#define MAX_ITERATION 1000
#define CROSSOVER_RATE 0.8
#define MUTATION_RATE 0.3
#define FITNESS_THRESHOLD 1800


class Robi3
{
    int row = 0;
    int col = 0;
    /* 定义机器人状态结构体 */
    struct RobotState {
        char up, down,left, right, curr ;
    };

    /* 定义格子状态类型（0表示空，1表示垃圾，2表示边界） */
    typedef enum { EMPTY = 0, TRASH = 1, BOUNDARY = 2 } CellState;
    typedef enum{UP,LEFT,RIGHT,D0OWN,PICK} ActionCode;

    /* 声明全局变量：格子状态数组、机器人当前状态 */
    char cells[10][10];

    /* 声明遗传算法需要用到的函数 */
    int* generate_random_gene();
    int** generate_initial_population();
    int getCurrentState();
    int getStateNo();
    int goStep(int ch);
    int evaluate_fitness(int* gene,bool);
    void selection(int** population, int* fitness);
    void crossover(int* parent1, int* parent2, int* child1, int* child2);
    void mutation(int* gene);

    void initCells();
    void printCells();
    void test();
    void printStrategy(int index,int action);
public:
    int main();
};