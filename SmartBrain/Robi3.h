#pragma once
#define POP_SIZE 200
#define GENE_SIZE 243
#define MAX_ITERATION 100
#define CROSSOVER_RATE 0.8
#define MUTATION_RATE 0.1

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
    char* generate_random_gene();
    char** generate_initial_population();
    int getStateNo(char[][10]);
    char* deIndex(int);
    int goStep(char ch, char[][10]);
    int evaluate_fitness(char* gene,bool);
    void selection(char** population, int* fitness);
    void crossover(char* parent1, char* parent2, char* child1, char* child2);
    void mutation(char* gene);

    void initCells();
    void cloneCells(char[][10]);
    void printCells(char[][10]);
    void printStrategy(int index, char action);
public:
    int main();
    void test();
    void test1();
    void testG();

};