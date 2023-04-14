#include "Robi3.h"
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

/* 生成一个随机的基因（即策略表） */
int* Robi3::generate_random_gene() {
    static int gene[GENE_SIZE];
    for (int i = 0; i < GENE_SIZE; i++) {
        gene[i] = rand() % 6;
    }
    return gene;
}

/* 生成初始种群 */
int** Robi3::generate_initial_population() 
{
    static int** population=(int**)malloc(POP_SIZE*sizeof(int*));
    for (int i = 0; i < POP_SIZE; i++) 
    {
        int* gene = generate_random_gene();
        population[i] = (int*)malloc(GENE_SIZE * sizeof(int));
        for (int j = 0; j < GENE_SIZE; j++) 
        {
            population[i][j] = gene[j];
        }
    }
    return population;
}
//分别查看当前位置及其上下左右四个位置的状态
int Robi3::getCurrentState()
{
    struct RobotState robot;

    if (row > 0)
    {
        robot.up = cells[row - 1][col];
    }
    else
    {
        robot.up = 2;
    }

    if (col > 0)
    {
        robot.left = cells[row][col - 1];
    }
    else
    {
        robot.left = 2;
    }

    robot.curr = cells[row][col];
    
    if (col < 9)
    {
        robot.right = cells[row][col + 1];
    }
    else
    {
        robot.right = 2;
    }

    if (row < 9)
    {
        robot.down = cells[row + 1][col];
    }
    else
    {
        robot.down = 2;
    }

    int index = robot.up * 81 + robot.left * 27 + robot.curr * 9 + robot.right * 3 + robot.down;
    printf("(%d,%d)[%d,%d,%d,%d,%d]:%d\n\r",row,col,robot.up,robot.left,robot.curr,robot.right,robot.down,index);
    return index;
}
/* 根据某一个基因（即策略表）计算适应度函数值 */
int Robi3::evaluate_fitness(int* gene,bool debug=false) {
    // 初始化机器人状态
    row = 5;
    col = 5;
    int score = 0;

    // 模拟机器人行动过程
    for (int i = 0; i < 200; i++) 
    {
        // 根据当前状态获取行动策略
        int index = getCurrentState();
        char action = gene[index]; // 0表示向上，1表示向左，2表示向右，3表示向下，4表示捡垃圾
        //printf("[%d]:%d\n\r", index, action);
        if (action == 0) { // 向上移动
            if (row<=0) { // 碰到边界
                score -= 5;
                if (debug)
                {
                    printf("[xUW]");
                }
            }
            else {
                row--;
                if (debug)
                {
                    printf("U");
                }
            }
        }
        else if (action == 1) { // 向左移动
            if (col<=0) { // 碰到边界
                score -= 5;
                if (debug)
                {
                    printf("[xLW]");
                }
            }
            else {
                col--;
                if (debug)
                {
                    printf("L");
                }
            }
        }
        else if (action == 2) { // 向右移动
            if (col>=9) { // 碰到边界
                score -= 5;
                if (debug)
                {
                    printf("[xRW]");
                }
            }
            else {
                col++;
                if (debug)
                {
                    printf("R");
                }
            }
        }
        else if (action == 3) { // 向下移动
            if (row >=9) { // 碰到边界
                score -= 5;
                if (debug)
                {
                    printf("[xDW]");
                }
            }
            else {
                row++;
                if (debug)
                {
                    printf("D");
                }
            }
        }
        else if (action == 4) 
        { // 捡垃圾
            if (cells[row][col] == TRASH) 
            {
                score += 10;
                cells[row][col] = EMPTY; // 将这个格子置为空
                if (debug)
                {
                    printf("P");
                }
            }
        }
    }
    return score;
}

/* 选择操作：根据归一化的适应度函数值选择优秀个体 */
void Robi3::selection(int** population, int* fitness) {
    static int new_population[POP_SIZE][GENE_SIZE];
    int total_fitness = 0;
    double cumulative_fitness[POP_SIZE];

    //fixFitness(fitness);
    //再不改变原始得分的情况下计算包括负值在内的得分的期望概率
    int min = 0;
    for (int i = 0;i < POP_SIZE;i++)
    {
        if (min > fitness[i])
            min = fitness[i];
    }

    for (int i = 0; i < POP_SIZE; i++) 
    {
        total_fitness += fitness[i];
    }
    if (min < 0)
        total_fitness -= min * POP_SIZE;

    if (total_fitness == 0)
        total_fitness = 1;

    for (int i = 0; i < POP_SIZE; i++) 
    {
        int val = fitness[i];
        if (min < 0)
            val -= min;
        cumulative_fitness[i] = val / (double)total_fitness;
        
        if (i > 0) {
            cumulative_fitness[i] += cumulative_fitness[i - 1] ;
        }
    }
    //选择一半
    for (int i = 0; i < POP_SIZE/2 ; i++) 
    {
        int selected_index = 0;
        double r = rand() / (double)(RAND_MAX + 1);
        while (cumulative_fitness[selected_index] <r) 
        {
            selected_index++;
        }
        if (selected_index < POP_SIZE)
        {
            for (int j = 0; j < GENE_SIZE; j++) {
                new_population[i][j] = population[selected_index][j];
             }
        }
        else
        {
            printf("out of boundary %d %f %f\n\r",i,r,cumulative_fitness[POP_SIZE-1]);
        }
    }
     // 将新种群复制回原来的数组
    for (int i = 0; i < POP_SIZE/2 ; i++) {
        for (int j = 0; j < GENE_SIZE; j++) {
            population[i][j] = new_population[i][j];
        }
    }
}

/* 交叉操作：对两个父代基因进行随机交叉，生成两个新的子代基因 */
void Robi3::crossover(int* parent1, int* parent2, int* child1, int* child2) {
    double crossover_random = (double)rand() / RAND_MAX;
    if (crossover_random <= CROSSOVER_RATE) {
        int crossover_point = rand() % GENE_SIZE;
        for (int i = 0; i < crossover_point; i++) {
            child1[i] = parent1[i];
            child2[i] = parent2[i];
        }
        for (int i = crossover_point; i < GENE_SIZE; i++) {
            child1[i] = parent2[i];
            child2[i] = parent1[i];
        }
    }
    else {
        for (int i = 0; i < GENE_SIZE; i++) {
            child1[i] = parent1[i];
            child2[i] = parent2[i];
        }
    }
}

/* 变异操作：对某一个基因进行随机变异，改变其中的一个值 */
void Robi3::mutation(int* gene) {
    for (int i = 0; i < GENE_SIZE; i++) {
        double mutation_random = (double)rand() / RAND_MAX;
        if (mutation_random <= MUTATION_RATE) {
            gene[i] = rand() % 6;
        }
    }
}
void Robi3::initCells()
{
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            cells[i][j] = (rand() % 2 == 0) ? EMPTY : TRASH;
        }
    }
}
void Robi3::printCells()
{
    for (int i = 0;i < 10;i++)
    {
        for (int j = 0;j < 10;j++)
            printf("%d", cells[i][j]);
        printf("\n\r");
    }
}
int Robi3::main() 
{
    // 随机初始化格子状态
    srand(time(NULL));
    // 初始化种群
    int** population = generate_initial_population();
    // 开始迭代
    int iteration = 0;
    while (iteration < MAX_ITERATION) 
    {
        // 计算适应度函数值
        int fitness[POP_SIZE];
        for (int i = 0; i < POP_SIZE; i++) {
            initCells();
            fitness[i] = evaluate_fitness(population[i],false);
        }
        // 选择操作
        selection(population, fitness);
        // 交叉和变异操作
        for (int i = 0; i < POP_SIZE / 2; i+=2) {
            //int parent1_index = rand() % POP_SIZE;
            //int parent2_index = rand() % POP_SIZE;
            //crossover(population[parent1_index], population[parent2_index],population[i * 2], population[i * 2 + 1]);
            crossover(population[i], population[i+1],population[i * 2], population[i * 2 + 1]);
            mutation(population[i * 2]);
            mutation(population[i * 2 + 1]);
        }
        // 记录最优适应度函数值
        int best_fitness = fitness[0];
        for (int i = 1; i < POP_SIZE; i++) {
            if (fitness[i] > best_fitness) {
                best_fitness = fitness[i];
            }
        }
        // 打印当前迭代次数和最优适应度函数值
        printf("%d, Best Fitness: %d\n", iteration, best_fitness);
        iteration++;
    }
    // 输出最终的最优策略表
    printf("Best Gene:\n\r");
    for (int i = 0; i < GENE_SIZE; i++) {
        printf("%d", population[0][i]);
    }
    printf("\n");
    initCells();
    printCells();

    int s = evaluate_fitness(population[0],true);
    printf("\n\rScore:%d\n\r", s);
    return 0;
}