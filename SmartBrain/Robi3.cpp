#include "Robi3.h"
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

/* ����һ������Ļ��򣨼����Ա� */
int* Robi3::generate_random_gene() {
    static int gene[GENE_SIZE];
    for (int i = 0; i < GENE_SIZE; i++) {
        gene[i] = rand() % 3;
    }
    return gene;
}

/* ���ɳ�ʼ��Ⱥ */
int** Robi3::generate_initial_population() {
    static int population[POP_SIZE][GENE_SIZE];
    for (int i = 0; i < POP_SIZE; i++) {
        int* gene = generate_random_gene();
        for (int j = 0; j < GENE_SIZE; j++) {
            population[i][j] = gene[j];
        }
    }
    return population;
}

/* ����ĳһ�����򣨼����Ա�������Ӧ�Ⱥ���ֵ */
int Robi3::evaluate_fitness(int* gene) {
    // ��ʼ��������״̬
    robot.up = cells[0][1];
    robot.left = cells[1][0];
    robot.curr = cells[0][0];
    robot.right = cells[1][0];
    robot.down = cells[1][1];
    int score = 0;
    // ģ��������ж�����
    for (int i = 0; i < 200; i++) {
        // ���ݵ�ǰ״̬��ȡ�ж�����
        int index = robot.up * 81 + robot.left * 27 + robot.curr * 9 + robot.right * 3 + robot.down;
        int action = gene[index]; // 0��ʾ���ϣ�1��ʾ����2��ʾ���ң�3��ʾ���£�4��ʾ������
        if (action == 0) { // �����ƶ�
            if (robot.up == BOUNDARY) { // �����߽�
                score -= 5;
            }
            else {
                robot.up = cells[i - 1][i];
                robot.left = cells[i][i - 1];
                robot.curr = cells[i][i];
                robot.right = cells[i][i + 1];
                robot.down = cells[i + 1][i];
            }
        }
        else if (action == 1) { // �����ƶ�
            if (robot.left == BOUNDARY) { // �����߽�
                score -= 5;
            }
            else {
                robot.up = cells[i][i - 1];
                robot.left = cells[i + 1][i];
                robot.curr = cells[i][i];
                robot.right = cells[i - 1][i];
                robot.down = cells[i][i + 1];
            }
        }
        else if (action == 2) { // �����ƶ�
            if (robot.right == BOUNDARY) { // �����߽�
                score -= 5;
            }
            else {
                robot.up = cells[i][i + 1];
                robot.left = cells[i - 1][i];
                robot.curr = cells[i][i];
                robot.right = cells[i + 1][i];
                robot.down = cells[i][i - 1];
            }
        }
        else if (action == 3) { // �����ƶ�
            if (robot.down == BOUNDARY) { // �����߽�
                score -= 5;
            }
            else {
                robot.up = cells[i + 1][i];
                robot.left = cells[i][i + 1];
                robot.curr = cells[i][i];
                robot.right = cells[i][i - 1];
                robot.down = cells[i - 1][i];
            }
        }
        else { // ������
            if (robot.curr == TRASH) {
                score += 10;
                cells[i][i] = EMPTY; // �����������Ϊ��
            }
        }
    }
    return score;
}

/* ѡ����������ݹ�һ������Ӧ�Ⱥ���ֵѡ��������� */
void Robi3::selection(int** population, int* fitness) {
    static int new_population[POP_SIZE][GENE_SIZE];
    int total_fitness = 0;
    double normalized_fitness[POP_SIZE];
    double cumulative_fitness[POP_SIZE];
    double random_values[POP_SIZE - 1];
    for (int i = 0; i < POP_SIZE; i++) {
        total_fitness += fitness[i];
    }
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
            gene[i] = rand() % 3;
        }
    }
}
int Robi3::main() {
    // �����ʼ������״̬
    srand(time(NULL));
    for (int i = 0; i < 50; i++) {
        for (int j = 0; j < 50; j++) {
            if (i == 0 && j == 0) { // ���λ��
                cells[i][j] = EMPTY;
            }
            else if (i == 0 || j == 0 || i == 49 || j == 49) { // �߽�
                cells[i][j] = BOUNDARY;
            }
            else { // ����������
                cells[i][j] = (rand() % 2 == 0) ? EMPTY : TRASH;
            }
        }
    }
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