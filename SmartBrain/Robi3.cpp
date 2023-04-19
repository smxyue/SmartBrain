#include "Robi3.h"
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

/* ����һ������Ļ��򣨼����Ա� */
char* Robi3::generate_random_gene() {
    static char gene[GENE_SIZE];
    for (int i = 0; i < GENE_SIZE; i++) {
        gene[i] = rand() % 7;
    }
    return gene;
}

/* ���ɳ�ʼ��Ⱥ */
char** Robi3::generate_initial_population() 
{
    static char** population=(char**)malloc(POP_SIZE*sizeof(char*));
    for (int i = 0; i < POP_SIZE; i++) 
    {
        char* gene = generate_random_gene();
        population[i] = (char*)malloc(GENE_SIZE * sizeof(char));
        for (int j = 0; j < GENE_SIZE; j++) 
        {
            population[i][j] = gene[j];
        }
    }
    return population;
}
char* Robi3::deIndex(int index)
{
    static char state[5];
    state[4] = index % 3;
    state[0] = index / 81;
    state[1] = (index % 81) / 27;
    state[2] = (index % 27) / 9;
    state[3] = (index % 9) / 3;
    return state;
}
void Robi3::printStrategy(int index,char action)
{
    RobotState state;
    state.curr = index % 3;
    state.up = index / 81;
    state.down = (index% 81) / 27;
    state.left = (index %27)/ 9;
    state.right = (index % 9) / 3;
    //0=���ϣ�1=���£�2=����3=���ң�4=������5=��ʰ���ӣ�6=����ƶ�

    char goAction;
    switch (action)
    {
    case 0:
        goAction='A';
        break;
    case 1:
        goAction = 'V';
        break;
    case 2:
        goAction = '<';
        break;
    case 3:
        goAction = '>';
        break;
    case 4:
        goAction = '?';
        break;
    case 5:
        goAction = 'X';
        break;
    default:
        goAction = 'O';
        break;
    }

    printf(" %d\n\r", state.up);
    printf("%d%d%d  %C \n\r", state.left, state.curr, state.right,goAction);
    printf(" %d\n\r", state.down);
}
int Robi3::getStateNo(char workCell[][10])
{
    char grid[4] = { 0,0,0,0};//����������
    if (row == 0)
    {
        grid[0] = 2;
    }
    else
    {
        grid[0] = workCell[row - 1][col];
    }
    if (row == 9)
    {
        grid[1] = 2;
    }
    else
    {
        grid[1] = workCell[row + 1][col];
    }
    if (col == 0)
    {
        grid[2] = 2;
    }
    else
    {
        grid[2] = workCell[row][col - 1];
    }
    if (col == 9)
    {
        grid[3] = 2;
    }
    else
    {
        grid[3] = workCell[row][col + 1];
    }
    int index = grid[0] * 3 * 3 * 3 * 3 + grid[1] * 3 * 3 * 3 + grid[2] * 3 * 3 + grid[3] * 3 + cells[row][col];
    return index;
}
int Robi3::goStep(char ch,char workCell[][10])
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
        if (workCell[row][col] == 1)
        {
            workCell[row][col] = 0;
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
void Robi3::cloneCells(char dst[][10])
{
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 10; j++)
            dst[i][j] = cells[i][j];
    }
}
/* ����ĳһ�����򣨼����Ա�������Ӧ�Ⱥ���ֵ */
int Robi3::evaluate_fitness(char* gene,bool debug=false) {
    char workCell[10][10];
    cloneCells(workCell);
    // ��ʼ��������״̬
    row = 0;
    col = 0;
    int score = 0;
    
    // ģ��������ж�����
    for (int i = 0; i < 200; i++)
    {
        // ���ݵ�ǰ״̬��ȡ�ж�����
        int index = getStateNo(workCell);
        int action = gene[index]; 
        score += goStep(action,workCell);
    }
    free(workCell);
    return score;
}

/* ѡ����������ݹ�һ������Ӧ�Ⱥ���ֵѡ��������� */
void Robi3::selection(char** population, int* fitness) {
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
void Robi3::crossover(char* parent1, char* parent2, char* child1, char* child2) {
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
void Robi3::mutation(char* gene) {
    double mutation_random = (double)rand() / RAND_MAX;
    if (mutation_random <= MUTATION_RATE)
    {
        int index;
        char* state;
        do
        {
            index = rand() % GENE_SIZE;
            state = deIndex(index);
         } 
         while (state[4] == 2 || (state[0]==2 && state[1]==2) || (state[2]==2 && state[3]==2));
         gene[index] = rand() % 7;
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
void Robi3::printCells(char workCell[][10] )
{
    for (int i = 0;i < 10;i++)
    {
        for (int j = 0;j < 10;j++)
            printf("%d", workCell[i][j]);
        printf("\n\r");
    }
}
void Robi3::test1()
{
    initCells();
    printCells(cells);
}
void Robi3::test()
{
    char G[244] = "254355153256235251056355461151336154151034156110550150052030256256132252350325112052333054055231255051336154150665264150266506012264453605631520256431054354632404350334153250253251352352045150130156213436252353223135051260513356201524514343434";
    RobotState state;
    int useless = 0;
    for(int i=0;i<=2;i++)
        for(int j=0;j<=2;j++)
            for(int l=0;l<=2;l++)
                for(int m=0;m<=2;m++)
                    for (int n = 0;n <= 2;n++)
                    {
                        state.up = i;
                        state.down = j;
                        state.left = l;
                        state.right =m;
                        state.curr = n;
                        if (!(state.curr == 2 || (state.up == 2 && state.down == 2) || (state.left == 2 && state.right == 2)))
                        {
                            int index = state.up * 81 + state.down * 27 + state.left * 9 + state.right * 3 + state.curr;
                            printStrategy(index, G[index] - '0');
                        }
                        else
                        {
                            useless++;
                        }
                    }
    printf("useless code:%d", useless);
}
void Robi3::testG()
{
    char G[244] = "254355153256235251056355461151336154151034156110550150052030256256132252350325112052333054055231255051336154150665264150266506012264453605631520256431054354632404350334153250253251352352045150130156213436252353223135051260513356201524514343434";
    col = 0;
    row = 0;
    int score = 0;
    initCells();
    printCells(cells);
    char workCell[10][10];
    cloneCells(workCell);
    for (int i = 0; i < 200; i++)
    {
        int stateNo = getStateNo(workCell);
        char ch = G[stateNo] - '0';
        score += goStep(ch,workCell);
        char* ss = deIndex(stateNo);
        printStrategy(stateNo, ch);
        printf("%3d[%d,%d](%d%d%d%d%d):%d\n\r\n\r", i,row, col, ss[0], ss[1], ss[2], ss[3], ss[4], score);
    }
    printf("M mehtod socre:%d\n\r", score);
    printf("workCell strat:\n\r");
    printCells(workCell);

}

int Robi3::main() 
{
    // �����ʼ������״̬
    srand(time(0));
    // ��ʼ����Ⱥ
    char** population = generate_initial_population();
    // ��ʼ����
    int iteration = 0;
    initCells();
    while (iteration < MAX_ITERATION) 
    {
        // ������Ӧ�Ⱥ���ֵ
        int fitness[POP_SIZE];
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
        printf("%5d, Best Fitness: %5d\n", iteration, best_fitness);
        iteration++;
    }
    // ������յ����Ų��Ա�
    printf("Best Gene:\n\r");
    for (int i = 0; i < GENE_SIZE; i++) {
        printf("%d", population[0][i]);
    }
    printf("\n");
    initCells();
    //printCells((char**)cells);

    int s = evaluate_fitness(population[0],true);
    printf("\n\rScore:%d\n\r", s);
    return 0;
}