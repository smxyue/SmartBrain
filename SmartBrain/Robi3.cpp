#include "Robi3.h"
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

/* ����һ������Ļ��򣨼����Ա� */
int* Robi3::generate_random_gene() {
    static int gene[GENE_SIZE];
    for (int i = 0; i < GENE_SIZE; i++) {
        gene[i] = rand() % 7;
    }
    return gene;
}

/* ���ɳ�ʼ��Ⱥ */
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
    char grid[4] = { 0,0,0,0 };//����������
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
    //0=���ƶ���1=�����ƶ���2=���ƶ���3=�����ƶ���4=������5=��ʰ���ӣ�6=����ƶ�
    //�������й��Ӳ����ռ���10�֡��ռ����Ӷ���������û�У�-1�֡����ײ����ǽ���ᱻ��5�֣�������ԭ���ĸ��ӡ�
    //0=���ϣ�1=���£�2=����3=���ң�4=������5=��ʰ���ӣ�6=����ƶ�
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
/* ����ĳһ�����򣨼����Ա�������Ӧ�Ⱥ���ֵ */
int Robi3::evaluate_fitness(int* gene,bool debug=false) {
    char** rooms = cloneCells();
    // ��ʼ��������״̬
    row = 0;
    col = 0;
    int score = 0;

    // ģ��������ж�����
    for (int i = 0; i < 200; i++)
    {
        // ���ݵ�ǰ״̬��ȡ�ж�����
        int index = getStateNo(rooms);
        int action = gene[index]; 
        score += goStep(action, rooms);
    }
    return score;
}

/* ѡ����������ݹ�һ������Ӧ�Ⱥ���ֵѡ��������� */
void Robi3::selection(int** population, int* fitness) {
    static int new_population[POP_SIZE][GENE_SIZE];
    int total_fitness = 0;
    double cumulative_fitness[POP_SIZE];

    //fixFitness(fitness);
    //�ٲ��ı�ԭʼ�÷ֵ�����¼��������ֵ���ڵĵ÷ֵ���������
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
    //ѡ��

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

        //����
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
     // ������Ⱥ���ƻ�ԭ��������
    for (int i = 0; i < POP_SIZE; i++) {
        for (int j = 0; j < GENE_SIZE; j++) {
            population[i][j] = new_population[i][j];
        }
    }
}

/* ��������������������������������棬���������µ��Ӵ����� */
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

/* �����������ĳһ���������������죬�ı����е�һ��ֵ */
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
    // �����ʼ������״̬
    srand(time(0));
    // ��ʼ����Ⱥ
    int** population = generate_initial_population();
    // ��ʼ����
    int iteration = 0;
    while (iteration < MAX_ITERATION) 
    {
        // ������Ӧ�Ⱥ���ֵ
        int fitness[POP_SIZE];
        initCells();
        for (int i = 0; i < POP_SIZE; i++) {
            fitness[i] = evaluate_fitness(population[i],false);
        }
        // ѡ�����
        selection(population, fitness);

        // ����ͱ������
        for (int i = 0; i < POP_SIZE-2; i+=2) {
            //crossover(population[i], population[i+1],population[i], population[i + 1]);
            //crossover(population[i], population[i+1],population[i * 2+1], population[i * 2 + 2]);
            mutation(population[i ]);
            mutation(population[i + 1]);
        }
        // ��¼������Ӧ�Ⱥ���ֵ
        int best_fitness = fitness[0];
        for (int i = 1; i < POP_SIZE; i++) {
            if (fitness[i] > best_fitness) {
                best_fitness = fitness[i];
            }
        }
        // ��ӡ��ǰ����������������Ӧ�Ⱥ���ֵ
        printf("%5d, Best Fitness: %d\n", iteration, best_fitness);
        iteration++;
    }
    // ������յ����Ų��Ա�
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