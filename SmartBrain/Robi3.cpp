#include "Robi3.h"
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <corecrt_math.h>

/* 生成一个随机的基因（即策略表） */
char* Robi3::generate_random_gene() {
    static char gene[GENE_SIZE];
    for (int i = 0; i < GENE_SIZE; i++) {
        gene[i] = rand() % 7;
    }
    return gene;
}

/* 生成初始种群 */
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
    //0=向上，1=向下，2=向左，3=向右，4=不动，5=捡拾罐子，6=随机移动

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
        goAction = '.';
        break;
    case 5:
        goAction = 'X';
        break;
    case 6:
        goAction = '*';
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
    char grid[4] = { 0,0,0,0};//上下左右中
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
    int index = grid[0] * 3 * 3 * 3 * 3 + grid[1] * 3 * 3 * 3 + grid[2] * 3 * 3 + grid[3] * 3 + workCell[row][col];
    return index;
}
int Robi3::goStep(char ch,char workCell[][10])
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
/* 根据某一个基因（即策略表）计算适应度函数值 */
int Robi3::evaluate_fitness(char* gene,bool debug=false) {
    // 初始化机器人状态
    int score = 0;
    for (int k = 0;k < 200;k++)
    {
        row = 0;
        col = 0;
        initCells();
        // 模拟机器人行动过程
        for (int i = 0; i < 200; i++)
        {
            // 根据当前状态获取行动策略
            int index = getStateNo(cells);
            int action = gene[index];
            int fen =goStep(action, cells);
            if (fen > 0)
                score += fen;
        }
    }
    return score/200;
}

void Normalize(int*score,double*nor) {
    int min_val = INT_MAX;
    for (int i = 0; i < POP_SIZE; i++) {
        if (score[i] < min_val) {
            score[i];
        }
    }
    if (min_val <= 0) { // 处理负数情况
        min_val = abs(min_val) + 1;
    }
    for (int i = 0; i < POP_SIZE; i++) {
        nor[i] = log10(score[i] + min_val);  // 使用 log 函数进行归一化
    }
}

/* 选择操作：根据归一化的适应度函数值选择优秀个体 */
void Robi3::selection(char** population, int* fitness) {
    static int new_population[POP_SIZE][GENE_SIZE];
    double cumulative_fitness[POP_SIZE];

    //fixFitness(fitness);
    Normalize(fitness, cumulative_fitness);

    double total = 0;
    for (int i = 0;i < POP_SIZE;i++)
        total += cumulative_fitness[i];
    if (total == 0)
        total = 1;
    for (int i = 0;i < POP_SIZE;i++)
    {
        cumulative_fitness[i] = cumulative_fitness[i] / total;
        if (i > 0)
            cumulative_fitness[i] += cumulative_fitness[i - 1];
    }
    cumulative_fitness[POP_SIZE - 1] = 1.0;
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
        double crossRnd = static_cast<double>(rand()) / (double)(RAND_MAX + 1);

        if (crossRnd < CROSSOVER_RATE)
        {
            int rnd = (rand() % (GENE_SIZE - 1)) + 1;
            for (int j = 0; j < rnd; j++)
            {
                new_population[newIndex][j] = temp[0][j];
                new_population[newIndex + 1][j] = temp[1][j];
            }
            for (int j = rnd; j < GENE_SIZE; j++)
            {
                new_population[newIndex][j] = temp[1][j];
                new_population[newIndex + 1][j] = temp[0][j];
            }
        }
        else
        {
            for (int j = 0; j < GENE_SIZE; j++)
            {
                new_population[newIndex][j] = temp[0][j];
                new_population[newIndex + 1][j] = temp[1][j];
            }
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

/* 变异操作：对某一个基因进行随机变异，改变其中的一个值 */
void Robi3::mutation(char* gene) {
    int mut_count = (int)GENE_SIZE * MUTATION_RATE;
    if (mut_count <= 0)
        mut_count = 1;
    for (int i = 0;i < mut_count;i++)
    {
        int index;
        char* state;
        do
        {
            index = rand() % GENE_SIZE;
            state = deIndex(index);
        } while (state[4] == 2 || (state[0] == 2 && state[1] == 2) || (state[2] == 2 && state[3] == 2));
        //printf("%d(%d%d%d%d%d)\n\r", i, state[0], state[1], state[2], state[3], state[4]);
        int newAction = rand() % 7;
        //while (gene[index] == newAction || newAction == 4 || newAction == 6)
        while (gene[index] == newAction)
        {
            newAction = rand() % 7;
        }
        gene[index] = newAction;
    }
}
void Robi3::initCells()
{
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            cells[i][j] = 0;
        }
    }
    for (int i = 0;i < 50;i++)
    {
        int x = rand() % 10;
        int y = rand() % 10;
        while (cells[y][x] == 1)
        {
            x = rand() % 10;
            y = rand() % 10;
        }
        cells[y][x] = 1;
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
void Robi3::reversCell(char ws[][10])
{
    for (int i = 0;i < 10;i++)
    {
        for (int j = 0;j < 10;j++)
        {
            if (ws[i][j] == 0)
                ws[i][j] = 1;
            else
                ws[i][j] = 0;
        }
    }
    ws[0][0] = 1;
}
void Robi3::test1()
{
    char** population = generate_initial_population();
    for (int i = 0;i < POP_SIZE;i++)
    {
        printf("\n\r");
        for (int j = 0;j < GENE_SIZE;j++)
        {
            printf("%d", population[i][j]);
        }
    }
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
    char M[244] = "65635365625235325265635365615135315125235325215135315165635365625235325265635365605035305025235325205035305015135315125235325215135315105035305025235325205035305065635356252353252656353656151353151252353252151353151656353656252353252656353454";
    char G[244] = "254355153256235251056355461151336154151034156110550150052030256256132252350325112052333054055231255051336154150665264150266506012264453605631520256431054354632404350334153250253251352352045150130156213436252353223135051260513356201524514343434";
    col = 0;
    row = 0;
    int score = 0;
    initCells();
    char workCell[10][10];
    cloneCells(workCell);
    for (int i = 0; i < 200; i++)
    {
        int stateNo = getStateNo(workCell);
        char ch = G[stateNo] - '0';
        score += goStep(ch,workCell);
    }
    printf("G mehtod socre:%d\n\r", score);
    if (score < 0)
    {
        printf("cells state:\n\r");
        printCells(cells);
        printf("\n\r\n\r");
        cloneCells(workCell);
        col = 0;
        row = 0;
        score = 0;
        for (int i = 0; i < 200; i++)
        {
            int stateNo = getStateNo(workCell);
            char ch = G[stateNo] - '0';
            score += goStep(ch, workCell);
            printStrategy(stateNo, ch);
            printf("\n\r%3d(%d %d):%d\n\r\n\r", i, row, col, score);
        }

    }
}

int Robi3::main() 
{
    // 随机初始化格子状态
    srand(time(0));
    // 初始化种群
    char** population = generate_initial_population();
    // 开始迭代
    int iteration = 0;
    initCells();
    while (iteration < MAX_ITERATION) 
    {
        // 计算适应度函数值
        int fitness[POP_SIZE];
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
        printf("%5d, Best Fitness: %5d\n", iteration, best_fitness);
        iteration++;
    }
    // 输出最终的最优策略表
    printf("Best Gene:\n\r");
    for (int i = 0; i < GENE_SIZE; i++) {
        printf("%d", population[0][i]);
    }
    printf("\n");
    initCells();
    //printCells((char**)cells);

    int s = evaluate_fitness(population[0],true);
    printf("\n\rScore:%d\n\r\n\r\n\r", s);
    col = 0;
    row = 0;
    int score = 0;
    for (int i = 0;i < 0;i++)
    {
        int index = getStateNo(cells);
        int ch = population[0][index];
        score += goStep(ch, cells);
        printStrategy(index, ch);
        printf("\n\r[%d，%d]:%d\n\r\n\r", row, col, score);
    }
    return 0;
}

/*
* 
https://blog.csdn.net/lynn0085/article/details/79016012

https://blog.csdn.net/huwp001/article/details/114627398

# -*- coding: utf-8 -*-
"""
func.py
Created on Tue Mar  9 17:40:51 2021

@author: huwp001
"""
import numpy as np

# 执行策略
#  罗比有7种可能选择：北移、南移、东移、西移、随机移动、不动、收集罐子
#   0=向北移动，1=向南移动，2=向东移动，3=向西移动，4=不动，5=捡拾罐子，6=随机移动
#  每个动作都会受到奖赏或惩罚。如果罗比所在的格子中有罐子并且收集起来了，就会得到10分的奖赏。
#  如果进行收集罐子的动作而格子中又没有罐子，就会被罚1分。如果撞到了墙，会被罚5分，并弹回原来的格子。
def people_work(people, pts):
    haspts = np.array([[999,999],[888,888]])
    p = np.array([1,1])
    lines = list()
    lines.append(p)
    score = 0   #分数
    sidect = 0  #撞墙次数
    failcolct = 0   #拾取失败的次数
    for step in people:
        if step==6:
            step = np.random.randint(4, size=1)[0]
        p2 = p
        if step==0:
            p2 = p + [0,1]
        if step==1:
            p2 = p + [0,-1]
        if step==2:
            p2 = p + [1,0]
        if step==3:
            p2 = p + [-1,0]
        # 检测p2 是否撞墙
        if p2.__contains__(0) or p2.__contains__(11):
            score = score - 5
            lines.append(p2)
            lines.append(p)
            sidect = sidect + 1
        else:
            p = p2
            lines.append(p)
        if step==4:
            p2 = p
        if step==5:
            if (pts == p2).all(1).any() and (haspts == p2).all(1).any()==False:
                # 有物体
                score = score + 10
                haspts = np.row_stack((haspts, p2))
            else:
                score = score - 1
                failcolct = failcolct + 1
    #print('拾取易拉罐%d次，撞墙%d次， 拾取失败%d次 ' % (len(haspts)-2, sidect, failcolct))
    #print(haspts)
    return lines, score

def mutation(gene):
    #基因突变， 基因链上随机取3个值
    for i in range(3):
        ixs = np.random.randint(len(gene), size=1)[0]
        gene[ixs] = np.random.randint(7, size=1)[0]
    return gene

def makelove(p1, p2):
    # 基因重组, 随机得到一个拆分点，重新组合
    s1 = np.random.randint(len(p1), size=1)[0]
    c1 = mutation(np.append(p1[0:s1],p2[s1:]))
    c2 = mutation(np.append(p2[:s1],p1[s1:]))
    return c1, c2

# 构造房间，放置垃圾易拉罐，注意放置了50%的易拉罐
def room():
    xxs = list()
    for i in range(10):
        xx = np.linspace(1, 1, 5)
        d = np.pad(xx,(0,5),'constant',constant_values=(0,0))
        np.random.shuffle(d)
        xxs.append(d)
    k = 0
    xxs = np.array(xxs)
    gbpoints = list()
    for x in np.nditer(xxs):
        if x==1:
            # mark
            a = int(k / xxs.shape[1])   # row
            b = k % xxs.shape[1]    # col
            gbpoints.append([b, a])
        k=k+1
    # 垃圾的坐标从 1 开始
    pts = np.array(gbpoints) + [1,1]
    return pts

# 这就是生活， 有能力才可以结婚，生娃娃，否则被淘汰
def life(peoples):
    scores = list()
    for i in range(len(peoples)):
        # 每个人要工作100次，取平均值
        #print('people epoch=%d ix=%d' %(epoch_num, i))
        if i % 10==0:
            print('.', end=(''))
        a = list()
        for k in range(10):
            lines, score = people_work(peoples[i], room())
            a.append(score)
        score = np.average(a)
        scores.append(score)
    scores = np.array(scores)
    print()
    print('这一代人的平均工作能力 %.3f' % np.average(scores))
    args = np.argsort(scores)  # 按工作能力排序

    # 随机寻找优秀的人去组合，结婚生子
    p = [0.1, 0.2, 0.3, 0.4]
    childrens = list()
    argss = np.split(args,4)    #人群拆分为4部分
    for i in range(100):
        k = int(np.random.choice([0,1,2,3], 1, replace=True, p=p))
        spouse = np.random.choice(argss[k], 2, replace=True)
        man = peoples[spouse[0]]
        woman = peoples[spouse[1]]
        c1, c2 = makelove(man, woman)
        childrens.append(c1)
        childrens.append(c2)
    return childrens



    # -*- coding: utf-8 -*-
"""
Created on Tue Mar  9 15:15:42 2021

@author: huwp001
"""


import numpy as np
import matplotlib.pyplot as plt

from func import life

# define

#基因长度
GENE_LEN = 243

#  初始化，女娲造人200个，基因随机
peoples = list()
for i in range(200):
    peoples.append(np.random.randint(0, 7, GENE_LEN))


# 暂定繁衍1000代
for epoch_num in range(1000):
    print('开始繁衍第%d代' % epoch_num )
    np.savetxt('peoples%d.txt' % epoch_num ,np.array(peoples),fmt='%d')
    peoples = life(peoples)
    epoch_num = epoch_num +1



*/