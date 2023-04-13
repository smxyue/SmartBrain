#include "Robi3.h"
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

/* 生成一个随机的基因（即策略表） */
int* Robi3::generate_random_gene() {
    static int gene[GENE_SIZE];
    for (int i = 0; i < GENE_SIZE; i++) {
        gene[i] = rand() % 5;
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

int Robi3::getCurrentState()
{
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
    return index;
}
/* 根据某一个基因（即策略表）计算适应度函数值 */
int Robi3::evaluate_fitness(int* gene) {
    // 初始化机器人状态
    row = 0;
    col = 0;
    int score = 0;
    // 模拟机器人行动过程
    for (int i = 0; i < 200; i++) {
        // 根据当前状态获取行动策略
        int index = getCurrentState();
        int action = gene[index]; // 0表示向上，1表示向左，2表示向右，3表示向下，4表示捡垃圾
        if (action == 0) { // 向上移动
            if (row<=0) { // 碰到边界
                score -= 5;
            }
            else {
                row--;
            }
        }
        else if (action == 1) { // 向左移动
            if (col<=0) { // 碰到边界
                score -= 5;
            }
            else {
                col--;
            }
        }
        else if (action == 2) { // 向右移动
            if (col>=9) { // 碰到边界
                score -= 5;
            }
            else {
                col++;
            }
        }
        else if (action == 3) { // 向下移动
            if (row >=9) { // 碰到边界
                score -= 5;
            }
            else {
                row++;
            }
        }
        else if (action == 4) 
        { // 捡垃圾
            if (cells[row][col] == TRASH) 
            {
                score += 10;
                cells[row][col] = EMPTY; // 将这个格子置为空
            }
        }
    }
    return score;
}
void fixFitness(int* fitness)
{
    int min = 0;
    for (int i = 0;i < POP_SIZE;i++)
    {
        if (min < fitness[i])
            min = fitness[i];
    }
    if (min < 0)
    {
        min *= -1;
        for (int i = 0;i < POP_SIZE;i++)
            fitness[i] += min;
    }
}
/* 选择操作：根据归一化的适应度函数值选择优秀个体 */
void Robi3::selection(int** population, int* fitness) {
    static int new_population[POP_SIZE][GENE_SIZE];
    int total_fitness = 0;
    double normalized_fitness[POP_SIZE];
    double cumulative_fitness[POP_SIZE];
    double random_values[POP_SIZE];

    fixFitness(fitness);
    
    for (int i = 0; i < POP_SIZE; i++) {
        total_fitness += fitness[i];
    }
    if (total_fitness == 0)
        total_fitness = 1;
    for (int i = 0; i < POP_SIZE; i++) {
        normalized_fitness[i] = (double)fitness[i] / total_fitness;
        if (i == 0) {
            cumulative_fitness[i] = normalized_fitness[i];
        }
        else {
            cumulative_fitness[i] = cumulative_fitness[i - 1] + normalized_fitness[i];
        }
    }
    for (int i = 0; i < POP_SIZE - 1; i++) {
        random_values[i] = (double)rand() / RAND_MAX;
    }
    random_values[POP_SIZE - 1] = 1.0; // 最后一个随机数设为1，以确保最后一个个体一定会被选中
    int selected_index = 0;
    for (int i = 0; i < POP_SIZE - 1; i++) {
        while (random_values[i] > cumulative_fitness[selected_index]) {
            selected_index++;
        }
        for (int j = 0; j < GENE_SIZE; j++) {
            new_population[i][j] = population[selected_index][j];
        }
    }
    // 将最优秀的个体直接复制到下一代
    int best_index = 0;
    for (int i = 1; i < POP_SIZE; i++) {
        if (fitness[i] > fitness[best_index]) {
            best_index = i;
        }
    }
    for (int j = 0; j < GENE_SIZE; j++) {
        new_population[POP_SIZE - 1][j] = population[best_index][j];
    }
    // 将新种群复制回原来的数组
    for (int i = 0; i < POP_SIZE; i++) {
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
            gene[i] = rand() % 5;
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
    initCells();
    printCells();
    // 初始化种群
    int** population = generate_initial_population();
    // 开始迭代
    int iteration = 0;
    int best_fitness = -10000;
    while (iteration < MAX_ITERATION && best_fitness < FITNESS_THRESHOLD) {
        // 计算适应度函数值
        int fitness[POP_SIZE];
        for (int i = 0; i < POP_SIZE; i++) {
            fitness[i] = evaluate_fitness(population[i]);
        }
        // 选择操作
        selection(population, fitness);
        // 交叉和变异操作
        for (int i = 0; i < POP_SIZE / 2; i++) {
            int parent1_index = rand() % POP_SIZE;
            int parent2_index = rand() % POP_SIZE;
            crossover(population[parent1_index], population[parent2_index],
                population[i * 2], population[i * 2 + 1]);
            mutation(population[i * 2]);
            mutation(population[i * 2 + 1]);
        }
        // 记录最优适应度函数值
        best_fitness = fitness[0];
        for (int i = 1; i < POP_SIZE; i++) {
            if (fitness[i] > best_fitness) {
                best_fitness = fitness[i];
            }
        }
        // 打印当前迭代次数和最优适应度函数值
        printf("Iteration: %d, Best Fitness: %d\n", iteration, best_fitness);
        iteration++;
    }
    // 输出最终的最优策略表
    printf("Best Gene:");
    for (int i = 0; i < GENE_SIZE; i++) {
        printf("%d ", population[0][i]);
    }
    printf("\n");
    return 0;
}