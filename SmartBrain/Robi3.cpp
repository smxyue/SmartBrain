#include "Robi3.h"
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

/* ����һ������Ļ��򣨼����Ա� */
int* Robi3::generate_random_gene() {
    static int gene[GENE_SIZE];
    for (int i = 0; i < GENE_SIZE; i++) {
        gene[i] = rand() % 5;
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
/* ����ĳһ�����򣨼����Ա�������Ӧ�Ⱥ���ֵ */
int Robi3::evaluate_fitness(int* gene) {
    // ��ʼ��������״̬
    row = 0;
    col = 0;
    int score = 0;
    // ģ��������ж�����
    for (int i = 0; i < 200; i++) {
        // ���ݵ�ǰ״̬��ȡ�ж�����
        int index = getCurrentState();
        int action = gene[index]; // 0��ʾ���ϣ�1��ʾ����2��ʾ���ң�3��ʾ���£�4��ʾ������
        if (action == 0) { // �����ƶ�
            if (row<=0) { // �����߽�
                score -= 5;
            }
            else {
                row--;
            }
        }
        else if (action == 1) { // �����ƶ�
            if (col<=0) { // �����߽�
                score -= 5;
            }
            else {
                col--;
            }
        }
        else if (action == 2) { // �����ƶ�
            if (col>=9) { // �����߽�
                score -= 5;
            }
            else {
                col++;
            }
        }
        else if (action == 3) { // �����ƶ�
            if (row >=9) { // �����߽�
                score -= 5;
            }
            else {
                row++;
            }
        }
        else if (action == 4) 
        { // ������
            if (cells[row][col] == TRASH) 
            {
                score += 10;
                cells[row][col] = EMPTY; // �����������Ϊ��
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
/* ѡ����������ݹ�һ������Ӧ�Ⱥ���ֵѡ��������� */
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
    random_values[POP_SIZE - 1] = 1.0; // ���һ���������Ϊ1����ȷ�����һ������һ���ᱻѡ��
    int selected_index = 0;
    for (int i = 0; i < POP_SIZE - 1; i++) {
        while (random_values[i] > cumulative_fitness[selected_index]) {
            selected_index++;
        }
        for (int j = 0; j < GENE_SIZE; j++) {
            new_population[i][j] = population[selected_index][j];
        }
    }
    // ��������ĸ���ֱ�Ӹ��Ƶ���һ��
    int best_index = 0;
    for (int i = 1; i < POP_SIZE; i++) {
        if (fitness[i] > fitness[best_index]) {
            best_index = i;
        }
    }
    for (int j = 0; j < GENE_SIZE; j++) {
        new_population[POP_SIZE - 1][j] = population[best_index][j];
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

/* �����������ĳһ���������������죬�ı����е�һ��ֵ */
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
    // �����ʼ������״̬
    srand(time(NULL));
    initCells();
    printCells();
    // ��ʼ����Ⱥ
    int** population = generate_initial_population();
    // ��ʼ����
    int iteration = 0;
    int best_fitness = -10000;
    while (iteration < MAX_ITERATION && best_fitness < FITNESS_THRESHOLD) {
        // ������Ӧ�Ⱥ���ֵ
        int fitness[POP_SIZE];
        for (int i = 0; i < POP_SIZE; i++) {
            fitness[i] = evaluate_fitness(population[i]);
        }
        // ѡ�����
        selection(population, fitness);
        // ����ͱ������
        for (int i = 0; i < POP_SIZE / 2; i++) {
            int parent1_index = rand() % POP_SIZE;
            int parent2_index = rand() % POP_SIZE;
            crossover(population[parent1_index], population[parent2_index],
                population[i * 2], population[i * 2 + 1]);
            mutation(population[i * 2]);
            mutation(population[i * 2 + 1]);
        }
        // ��¼������Ӧ�Ⱥ���ֵ
        best_fitness = fitness[0];
        for (int i = 1; i < POP_SIZE; i++) {
            if (fitness[i] > best_fitness) {
                best_fitness = fitness[i];
            }
        }
        // ��ӡ��ǰ����������������Ӧ�Ⱥ���ֵ
        printf("Iteration: %d, Best Fitness: %d\n", iteration, best_fitness);
        iteration++;
    }
    // ������յ����Ų��Ա�
    printf("Best Gene:");
    for (int i = 0; i < GENE_SIZE; i++) {
        printf("%d ", population[0][i]);
    }
    printf("\n");
    return 0;
}