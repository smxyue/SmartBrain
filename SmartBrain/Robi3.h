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
    /* ���������״̬�ṹ�� */
    struct RobotState {
        char up, down,left, right, curr ;
    };

    /* �������״̬���ͣ�0��ʾ�գ�1��ʾ������2��ʾ�߽磩 */
    typedef enum { EMPTY = 0, TRASH = 1, BOUNDARY = 2 } CellState;
    typedef enum{UP,LEFT,RIGHT,D0OWN,PICK} ActionCode;

    /* ����ȫ�ֱ���������״̬���顢�����˵�ǰ״̬ */
    char cells[10][10];

    /* �����Ŵ��㷨��Ҫ�õ��ĺ��� */
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