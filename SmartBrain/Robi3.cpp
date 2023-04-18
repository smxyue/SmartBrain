#include "Robi3.h"
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

/* 生成一个随机的基因（即策略表） */
int* Robi3::generate_random_gene() {
    static int gene[GENE_SIZE];
    for (int i = 0; i < GENE_SIZE; i++) {
        gene[i] = rand() % 7;
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
void Robi3::printStrategy(int index,int action)
{
    RobotState state;
    state.curr = index % 3;
    state.up = index / 81;
    state.down = (index% 81) / 27;
    state.left = (index %27)/ 9;
    state.right = (index % 9) / 3;
    printf(" %d\n\r", state.up);
    printf("%d%d%d\n\r", state.left, state.curr, state.right);
    printf(" %d\n\r", state.down);
    switch (action)
    {
        case 0:
            printf("A");
            break;
        case 1:
            printf("V");
            break;
        case 2:
            printf("<");
            break;
        case 3:
            printf(">");
            break;
        case 4:
            printf("@");
            break;
        case 5:
            printf("*");
            break;
        default:
            printf("O");
            break;
    }
    printf("\n\r");
}
int Robi3::getStateNo(char** data)
{
    char grid[4] = { 0,0,0,0 };//上下左右中
    if (row == 0)
    {
        grid[0] = 2;
    }
    else
    {
        grid[0] = data[row - 1][col];
    }
    if (row == 9)
    {
        grid[1] = 2;
    }
    else
    {
        grid[1] = data[row + 1][col];
    }
    if (col == 0)
    {
        grid[2] = 2;
    }
    else
    {
        grid[2] = data[row][col - 1];
    }
    if (col == 9)
    {
        grid[3] = 2;
    }
    else
    {
        grid[3] = data[row][col + 1];
    }
    int index = grid[0] * 3 * 3 * 3 * 3 + grid[1] * 3 * 3 * 3 + grid[2] * 3 * 3 + grid[3] * 3 + cells[row][col];
    return index;
}
int Robi3::goStep(int ch,char** data)
{
    //0=向北移动，1=向南移动，2=向东移动，3=向西移动，4=不动，5=捡拾罐子，6=随机移动
    //格子中有罐子并且收集，10分。收集罐子而格子中又没有，-1分。如果撞到了墙，会被罚5分，并弹回原来的格子。
    //0=向上，1=向下，2=向左，3=向右，4=不动，5=捡拾罐子，6=随机移动
    if (ch == 6)
    {
        ch = rand() % 6;
    }
    switch (ch)
    {
    case 0:
        if (row > 0)
        {
            row--;
        }
        else
        {
            return -5;
        }
        break;
    case 1:
        if (row < 9)
        {
            row++;
        }
        else
        {
            return -5;
        }
        break;
    case 2:
        if (col > 0)
        {
            col--;
        }
        else
        {
            return -5;
        }
        break;
    case 3:
        if (col < 9)
        {
            col++;
        }
        else
        {
            return  -5;
        }
        break;
    case 4:
        break;
    case 5:
        if (data[row][col] == 1)
        {
            data[row][col] = 0;
            return 10;
        }
        else
        {
            return -1;
        }
        break;
    }
    return 0;
}
char** Robi3::cloneCells()
{
    static char** newCells= (char**)malloc(10*sizeof(char*));
    for (int i = 0; i < 10; i++)
    {
        newCells[i] = (char*)malloc(10 * sizeof(char));
        for (int j = 0; j < 10; j++)
            newCells[i][j] = cells[i][j];
    }
    return newCells;
}
/* 根据某一个基因（即策略表）计算适应度函数值 */
int Robi3::evaluate_fitness(int* gene,bool debug=false) {
    char** rooms = cloneCells();
    // 初始化机器人状态
    row = 0;
    col = 0;
    int score = 0;

    // 模拟机器人行动过程
    for (int i = 0; i < 200; i++)
    {
        // 根据当前状态获取行动策略
        int index = getStateNo(rooms);
        int action = gene[index]; 
        score += goStep(action, rooms);
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
        //printf("%d:%f\n\r", i, cumulative_fitness[i]);
    }
    //选择

    int newIndex = 0;
    for (int i = 0;i < POP_SIZE / 2;i++)
    {
        int temp[2][GENE_SIZE];
        for (int j = 0;j < 2;j++)
        {
            double rnd = static_cast<double>(rand()) / double(RAND_MAX + 1);
            for (int k = 0;k < POP_SIZE;k++)
            {
                if (k == 0)
                {
                    if (rnd < cumulative_fitness[0])
                    {
                        for (int m = 0;m < GENE_SIZE;m++)
                        {
                            temp[j][m] = population[0][m];
                        }
                    }
                }
                else if (rnd >= cumulative_fitness[k - 1] && rnd < cumulative_fitness[k])
                {
                    for (int m = 0;m < GENE_SIZE;m++)
                    {
                        temp[j][m] = population[k][m];
                    }
                }
            }
        }

        //交叉
        int crossed[2][GENE_SIZE];
        double crossRnd = static_cast<double>(rand()) / (double)(RAND_MAX + 1);

        if (crossRnd < CROSSOVER_RATE)
        {
            int rnd = (rand() % (GENE_SIZE - 1)) + 1;
            for (int j = 0; j < rnd; j++)
            {
                crossed[0][j] = temp[0][j];
                crossed[1][j] = temp[1][j];
            }
            for (int j = rnd; j < GENE_SIZE; j++)
            {
                crossed[0][j] = temp[1][j];
                crossed[1][j] = temp[0][j];
            }
        }
        else
        {
            for (int j = 0; j < GENE_SIZE; j++)
            {
                crossed[0][j] = temp[0][j];
                crossed[1][j] = temp[1][j];
            }
        }
        for (int j = 0;j < GENE_SIZE;j++)
        {
            new_population[newIndex][j] = crossed[0][j];
            new_population[newIndex + 1][j] = crossed[1][j];
        }
        newIndex += 2;

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
        int crossover_point = rand() % (GENE_SIZE-1) +1;
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
    for (int i = 0; i < GENE_SIZE; i++)
    {
        double mutation_random = (double)rand() / RAND_MAX;
        if (mutation_random <= MUTATION_RATE)
        {
            gene[i] = rand() % 7;
        }
    }
}
void Robi3::initCells()
{
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            cells[i][j] = (rand() % 2 == 0) ? 0 : 1;
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
void Robi3::test()
{
    char M[243] = "65635365625235325265635365615135315125235325215135315165635365625235325265635365605035305025235325205035305015135315125235325215135315105035305025235325205035305065635356252353252656353656151353151252353252151353151656353656252353252656353454";
    col = 0;
    row = 0;
    int score = 0;
    char** data = (char**)malloc(sizeof(char*) * 10);

    for (int i = 0; i < 10; i++)
    {
        data[i] = (char*)malloc(sizeof(char) * 10);
        for (int j = 0; j < 10; j++)
            data[i][j] = cells[i][j];
    }
    for (int i = 0; i < 200; i++)
    {
        int stateNo = getStateNo(data);
        char ch = M[stateNo] - '0';
        score += goStep(ch,data);
        printf("[%d,%d]:%d\n\r", row, col, score);
    }
    printf("M mehtod socre:%d\n\r", score);
}

int Robi3::main() 
{
    // 随机初始化格子状态
    srand(time(0));
    // 初始化种群
    int** population = generate_initial_population();
    // 开始迭代
    int iteration = 0;
    while (iteration < MAX_ITERATION) 
    {
        // 计算适应度函数值
        int fitness[POP_SIZE];
        initCells();
        for (int i = 0; i < POP_SIZE; i++) {
            fitness[i] = evaluate_fitness(population[i],false);
        }
        // 选择操作
        selection(population, fitness);

        // 交叉和变异操作
        for (int i = 0; i < POP_SIZE-2; i+=2) {
            //crossover(population[i], population[i+1],population[i], population[i + 1]);
            //crossover(population[i], population[i+1],population[i * 2+1], population[i * 2 + 2]);
            mutation(population[i ]);
            mutation(population[i + 1]);
        }
        // 记录最优适应度函数值
        int best_fitness = fitness[0];
        for (int i = 1; i < POP_SIZE; i++) {
            if (fitness[i] > best_fitness) {
                best_fitness = fitness[i];
            }
        }
        // 打印当前迭代次数和最优适应度函数值
        printf("%5d, Best Fitness: %d\n", iteration, best_fitness);
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
    //test();
    for (int i = 0; i < 243; i++)
    {
        //printStrategy(i, population[0][i]);
        //getchar();
    }
    return 0;
}