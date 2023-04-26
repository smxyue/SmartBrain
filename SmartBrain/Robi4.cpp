#include "Robi4.h"
#include <vector>
#include <algorithm>

std::vector<int>* Robi4::stateSet()
{
	static std::vector<int>* indexSet = new std::vector<int>;
	indexSet->clear();
	for (int i = 0; i < 3; i++)
		for(int j=0;j<3;j++)
			for (int l = 0; l < 3; l++)
				for(int m=0;m<3;m++)
					for (int n = 0; n < 2; n++)
					{
						if ((i != 2 && j != 2) && (l != 2 && m != 2))
						{
							int index = i * 81 + j * 27 + l * 9 + m * 3 + n;
							indexSet->push_back(index);
						}
					}
	return indexSet;
}
void Robi4::printStateSet(std::vector<int>* set)
{
	if (set == NULL)
		return;
	printf("\n\r{");
	for (int val:*set)
	{
		printf(" %d ", val);
	}
	printf("}\n\r");
}
void Robi4::printStrategy(int index, char action)
{
	struct RobotState {
		char up, down, left, right, curr;
	};
	RobotState state;
	state.curr = index % 3;
	state.up = index / 81;
	state.down = (index % 81) / 27;
	state.left = (index % 27) / 9;
	state.right = (index % 9) / 3;
	//0=向上，1=向下，2=向左，3=向右，4=不动，5=捡拾罐子，6=随机移动

	char goAction;
	switch (action)
	{
	case 0:
		goAction = 'A';
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
	printf("%d%d%d  %C \n\r", state.left, state.curr, state.right, goAction);
	printf(" %d\n\r", state.down);
}
char* Robi4::deIndex(int index)
{
	static char state[5];
	state[4] = index % 3;
	state[0] = index / 81;
	state[1] = (index % 81) / 27;
	state[2] = (index % 27) / 9;
	state[3] = (index % 9) / 3;
	return state;
}
int Robi4::randAction(int index)
{
	std::vector<int> list = { 0,1,2,3,4,5,6 };
	char* bits = deIndex(index);
	if (bits[0] == 2)
	{
		auto it = std::remove(list.begin(), list.end(), 0);
		list.erase(it, list.end());
	}
	if (bits[1] == 2)
	{
		auto it = std::remove(list.begin(), list.end(), 1);
		list.erase(it, list.end());
	}
	if (bits[2] == 2)
	{
		auto it = std::remove(list.begin(), list.end(), 2);
		list.erase(it, list.end());
	}
	if (bits[3] == 2)
	{
		auto it = std::remove(list.begin(), list.end(), 3);
		list.erase(it, list.end());
	}
	return list.at(rand() % list.size());
}
/* 生成一个随机的基因（即策略表） */
char* Robi4::generate_random_gene() {
	static char gene[GENE_SIZE];
	for (int i = 0; i < GENE_SIZE; i++) {
		gene[i] = randAction(i);
	}
	return gene;
}

/* 生成初始种群 */
char** Robi4::generate_initial_population()
{
	static char** population = (char**)malloc(POP_SIZE * sizeof(char*));
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
void Robi4::initCells()
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
int Robi4::getStateNo(char workCell[][10])
{
	char grid[4] = { 0,0,0,0 };//上下左右中
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

int Robi4::goStep(char ch, char workCell[][10])
{
	//格子中有罐子并且收集，10分。收集罐子而格子中又没有，-1分。如果撞到了墙，会被罚5分，并弹回原来的格子。
	//0=向上，1=向下，2=向左，3=向右，4=不动，5=捡拾罐子，6=随机移动
	if (ch == 6)
	{
		ch = rand() % 4;
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
int Robi4::evaluate_fitness(char* gene) {
	// 初始化机器人状态
	int score = 0;
	for (int k = 0;k < 10;k++)
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
			int fen = goStep(action, cells);
			score += fen;
		}
	}
	return score / 10;
}

void Robi4::Normalize(int* score, double* nor) {
	int min_val = INT_MAX;
	int max_val = -1 * INT_MAX;
	for (int i = 0; i < POP_SIZE; i++) {
		if (score[i] < min_val) {
			min_val = score[i];
		}
		if (score[i] > max_val)
		{
			max_val = score[i];
		}
	}
	if (max_val == min_val)
		max_val = min_val + 1;
	for (int i = 0; i < POP_SIZE; i++) {
		nor[i] = (double)(score[i] - min_val) / (max_val - min_val);
	}
}
void Robi4::printFittness(double* fitness)
{
	printf("\n\r\tfittness\n\r");
	vector<pair<double, int>> v;
	for (int i = 0; i < POP_SIZE; i++)
	{
		v.push_back(make_pair(fitness[i], i));
	}
	sort(v.begin(), v.end());
	for (int i = 0; i < POP_SIZE; i++)
	{
		printf("%3d:%.5f",v[i].second, v[i].first);
	}
	printf("\n\r");
}
/* 选择操作：根据归一化的适应度函数值选择优秀个体 */
void Robi4::selection(char** population, int* fitness) {
	static int new_population[POP_SIZE][GENE_SIZE];
	double cumulative_fitness[POP_SIZE];

	//fixFitness(fitness);
	Normalize(fitness, cumulative_fitness);
	vector<pair<double, int>> v;
	for (int i = 0; i < POP_SIZE; i++)
	{
		v.push_back(make_pair(fitness[i], i));
	}
	sort(v.begin(), v.end());
	printFittness(cumulative_fitness);
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
	//选择
	for (int i = 0;i < POP_SIZE;i++)
	{
		int selected = 0;
		double rnd = static_cast<double>(rand()) / double(RAND_MAX + 1);
		while (rnd > cumulative_fitness[selected] && selected < POP_SIZE-1)
			selected++;
		printf("%d:%d ", selected,v[selected].second);
		for (int j = 0; j < GENE_SIZE; j++)
		{
				new_population[i][j] = population[selected][j];
		}
	}
	// 将新种群复制回原来的数组
	for (int i = 0; i < POP_SIZE; i++) {
		for (int j = 0; j < GENE_SIZE; j++) {
			population[i][j] = new_population[i][j];
		}
	}
}
/* 交叉操作：对两个父代基因进行随机交叉，生成两个新的子代基因 */
void Robi4::crossover(char* parent1, char* parent2) 
{

	double crossover_random = (double)rand() / RAND_MAX;
	if (crossover_random <= CROSSOVER_RATE) 
	{
		char child[2][GENE_SIZE];
		int crossover_point = rand() % (GENE_SIZE - 1) + 1;
		for (int i = 0; i < crossover_point; i++) {
			child[0][i] = parent1[i];
			child[1][i] = parent2[i];
		}
		for (int i = crossover_point; i < GENE_SIZE; i++) {
			child[0][i] = parent2[i];
			child[1][i] = parent1[i];
		}
		for (int i = 0;i < GENE_SIZE;i++)
		{
			parent1[i] = child[0][i];
			parent2[i] = child[1][i];
		}
	}
}
void Robi4::mutation(char* gene) {
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
		int newAction = randAction(index);
		//while (gene[index] == newAction || newAction == 4 || newAction == 6)
		while (gene[index] == newAction)
		{
			newAction = randAction(index);
		}
		gene[index] = newAction;
	}
}

void Robi4::testG()
{
	char M[244] = "65635365625235325265635365615135315125235325215135315165635365625235325265635365605035305025235325205035305015135315125235325215135315105035305025235325205035305065635356252353252656353656151353151252353252151353151656353656252353252656353454";
	char G[244] = "254355153256235251056355461151336154151034156110550150052030256256132252350325112052333054055231255051336154150665264150266506012264453605631520256431054354632404350334153250253251352352045150130156213436252353223135051260513356201524514343434";
	col = 0;
	row = 0;
	int score = 0;
	initCells();
	for (int i = 0; i < 200; i++)
	{
		int stateNo = getStateNo(cells);
		char ch = M[stateNo] - '0';
		score += goStep(ch, cells);
	}
	printf("G mehtod socre:%d\n\r", score);
}

void Robi4::printPopulation(char** p)
{
	for (int i = 0;i < POP_SIZE;i++)
	{
		printf("\n\r[");
		for (int j = 0;j < GENE_SIZE;j++)
			printf("%d", p[i][j]);
		printf("]\n\r");
	}
}
float genLikes(char* p1, char* p2)
{
	int count = 0;
	for (int i = 0;i < GENE_SIZE;i++)
	{
		if (p1[i] == p2[i])
			count++;
	}
	return (float)count / GENE_SIZE;
}
int Robi4::main()
{
	char** population = generate_initial_population();
	// 开始迭代
	int iteration = 0;
	initCells();
	while (iteration < MAX_ITERATION)
	{
		// 计算适应度函数值
		int fitness[POP_SIZE];
		for (int i = 0; i < POP_SIZE; i++) {
			fitness[i] = evaluate_fitness(population[i]);
		}
		// 选择操作
		selection(population, fitness);

		// 交叉和变异操作
		for (int i = 0; i < POP_SIZE - 2; i += 2) {
			crossover(population[i], population[i+1]);
			mutation(population[i]);
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

	int s = evaluate_fitness(population[0]);
	printf("\n\rScore:%d\n\r\n\r\n\r", s);

	return 0;
}
void Robi4::test()
{
	char** population = generate_initial_population();

	int fitness[POP_SIZE];
	for (int a = 0;a < 1;a++)
	{
		for (int i = 0; i < POP_SIZE; i++) {
			fitness[i] = evaluate_fitness(population[i]);
		}
		// 选择操作
		selection(population, fitness);
		for (int i = 0; i < POP_SIZE - 2; i += 2) {
			//crossover(population[i], population[i + 1]);
			//mutation(population[i]);
			//mutation(population[i + 1]);
		}

	}
	for (int i = 0;i < POP_SIZE;i++)
	{
		//printf("%4d: %.5f\n\r",i,genLikes(population[0],population[i]));
	}
}