#include "pyGen.h"
#include <stdlib.h>
#include <corecrt_math.h>
#include <stdio.h>

//初始化数据集，随机选择0-1000之间的50个数组成选择集，期望值为wanted，即总和的1/10
void pyGen::initPool()
{
	printf("数据集[");
	int sum = 0;
	for (int i = 0; i < 50; i++)
	{
		pool[i] = rand()%1000;
		sum += pool[i];
		printf(" %d ", pool[i]);
	}
	wanted = (double)sum / 10;
	printf("]\n\r期望值:%f\n\r",wanted);
}
//从选择集中随机选择10个不同的数组成一个组合
void pyGen::getGroup(int* group)
{
	int groupIndex[10];
	int count = 9;
	while (count >= 0)
	{
		int r = rand() % 50;
		bool is_used = false;
		//查看已经生成的是否重复
		for (int j = 0; j < 9 - count; j++)
		{
			if (groupIndex[j] == r)
				is_used = true;
		}
		if (!is_used)
		{
			groupIndex[9 - count] = r;
			count--;
		}
	}
	for (int i = 0; i < 10; i++)
	{
		group[i] = pool[groupIndex[i]];
	}
}

//初始化种群
void pyGen::initPopulation()
{
	for (int i = 0; i < POP_SIZE; i++)
	{
		getGroup(population[i]);
	}
}
//计算适应度
void pyGen::getFitness()
{
	for (int i = 0; i < POP_SIZE; i++)
	{
		int sum = 0;
		for (int j = 0; j < 10; j++)
			sum += population[i][j];
		int value = fabs(wanted - sum);
		if (value == 0)
			fitness[i] = 10;
		else
			fitness[i] = 1 / (double)value;
		//printf("%7f ", fitness[i]);
	}
	//printf("\n\r___________________________________________________________\n\r");
}

//选择并交换
void pyGen::select()
{
	int newPopulation[POP_SIZE][10];
	getFitness();
	double totalfitness = 0;
	//计算累计比率
	double cumulative_fitness[POP_SIZE];
	for (int i = 0; i < POP_SIZE; i++)
		totalfitness += fitness[i];
	for (int i = 0; i < POP_SIZE; i++)
	{
		cumulative_fitness[i] = fitness[i] / totalfitness;
		if (i > 0)
		{
			cumulative_fitness[i] += cumulative_fitness[i - 1];
		}
	}
	//每次选择两个，并交叉
	int newIndex = 0;
	for (int i = 0;i < POP_SIZE / 2;i++)
	{
		int temp[2][10];
		for (int j = 0;j < 2;j++)
		{
			double rnd = static_cast<double>(rand()) / double(RAND_MAX + 1);
			for (int k = 0;k < POP_SIZE;k++)
			{
				if (k == 0)
				{
					if (rnd < cumulative_fitness[0])
					{
						for (int m = 0;m < 10;m++)
						{
							temp[j][m] = population[0][m];
						}
					}
				}
				else if (rnd >= cumulative_fitness[k - 1] && rnd < cumulative_fitness[k])
				{
					for (int m = 0;m < 10;m++)
					{
						temp[j][m] = population[k][m];
					}
				}
			}
		}
		//交叉
		int crossed[2][10];
		double crossRnd = static_cast<double>(rand()) / (double)(RAND_MAX + 1);
		int rnd = rand() % 9 +1;
		if (crossRnd < CROSSOVER_RATE)
		{
			for (int j = 0;j < rnd;j++)
			{
				crossed[0][j] = temp[0][j];
				crossed[1][j] = temp[1][j];
			}
			for (int j = rnd;j < 10;j++)
			{
				crossed[0][j] = temp[1][j];
				crossed[1][j] = temp[0][j];

			}
		}
		else
		{
			for (int j = 0;j < 10;j++)
			{
				crossed[0][j] = temp[0][j];
				crossed[1][j] = temp[1][j];
			}
		}
		/*
		for (int j = 0;j <= rnd;j++)
		{
			crossed[0][j] = temp[0][j];
			crossed[1][j] = temp[1][j];
		}
		for (int j = 1;j <= 3;j++)
		{
			crossed[0][rnd + j] = temp[1][rnd + j];
			crossed[1][rnd + j] = temp[0][rnd + j];
		}
		for (int j = rnd+4;j < 10;j++)
		{
			crossed[0][j] = temp[0][j];
			crossed[1][j] = temp[1][j];
		}*/
		//加入新种群
		for (int j = 0;j < 10;j++)
		{
			newPopulation[newIndex][j] = crossed[0][j];
			newPopulation[newIndex + 1][j] = crossed[1][j];
		}
		newIndex += 2;	
	}
	//复制新种群
	for(int i=0;i<POP_SIZE;i++)
		for (int j = 0;j < 10;j++)
		{
			population[i][j] = newPopulation[i][j];
		}
}
/*
在您的选择交叉函数中，我发现了一些可能的问题。我将它们列出如下，并给出了相应的建议：

随机数生成问题：在选择染色体和计算交叉概率时，您使用了rand()函数。然而，这样的用法并不是正确的，因为它将随机数限制在了一个非常小的范围内。请使用以下方法替换这些行：

int rnd = rand() / double(RAND_MAX + 1);
```
替换为：
````cpp
double rnd = static_cast<double>(rand()) / (RAND_MAX + 1);
```

````cpp
double crossRnd = rand() / (double)(RAND_MAX + 1);
```
替换为：
````cpp
double crossRnd = static_cast<double>(rand()) / (RAND_MAX + 1);
```

选择过程中的概率分布问题：在您的选择过程中，您使用了累积适应度值。然而，这可能会导致某些染色体过于频繁地被选中。为了解决这个问题，您可以使用轮盘赌选择方法。在这种方法中，每个染色体的选择概率与其相对适应度成正比。您可以按照以下方式修改选择过程：

在select()函数内，在for (int i = 0; i < POP_SIZE; i++)循环之后，添加以下代码：

// Roulette wheel selection
std::vector<int> selected_indices;
for (int i = 0; i < POP_SIZE; ++i) {
	double rnd = static_cast<double>(rand()) / (RAND_MAX + 1);
	for (int j = 0; j < POP_SIZE; ++j) {
		if (rnd < cumulative_fitness[j]) {
			selected_indices.push_back(j);
			break;
		}
	}
}
```

然后，将选择过程中的`for (int j = 0; j < 2; j++)`循环替换为以下代码：

````cpp
for (int j = 0; j < 2; j++) {
	int index = selected_indices[2 * i + j];
	for (int m = 0; m < 10; m++) {
		temp[j][m] = population[index][m];
	}
}
```

交叉点的随机选择问题：在您的交叉过程中，您使用了固定的交叉点。为了增加多样性，您可以随机选择交叉点。将以下代码：

int rnd = rand() % 6;
```
替换为：
````cpp
int rnd = rand() % 9 + 1; // Random crossover point between 1 and 9
```
对您的代码进行这些更改后，您应该能够看到改善的进化效果。如果您需要进一步的帮助，请随时告诉我。
*/
//变异
void pyGen::variation()
{
	for (int i = 0;i < POP_SIZE;i++)
	{
		double rnd = rand() / (double)(RAND_MAX + 1);
		if (rnd < MUTATION_RATE)
		{
			int index = rand() % 10;
			int newGuy = rand() % 50;
			population[i][index] = pool[newGuy];
		}
	}
}
int pyGen::main()
{
	initPool();
	initPopulation();
	for (int i = 0;i < MAX_ITERATION;i++)
	{
		select();
		variation();
	}
	getFitness();
	int best = 0;
	for (int i = 1;i < POP_SIZE;i++)
	{
		if (fitness[best] < fitness[i])
			best = i;
	}
	int sum = 0;
	printf("\n\r最优选择集%d[",best);
	for (int i = 0;i < 10;i++)
	{
		sum += population[best][i];
		printf(" %d ", population[best][i]);
	}

	printf("\n\r进化值:%d\n\r", sum);
	randTry();
	return 0;
}
void pyGen::randTry()
{
	int best[10];
	int tmp[10];

	//initPool();
	int distance = wanted;
	for (int i = 0;i < MAX_ITERATION;i++)
	{
		for (int j = 0;j < 10;j++)
			tmp[j] = -1;
		for (int j = 0;j < 10;j++ )
		{
			bool is_usd;
			int rnd;
			do
			{
				is_usd = false;
				rnd= rand() % 50;
				for (int k = 0;k < j;k++)
				{
					if (tmp[k] == rnd)
						is_usd = true;
				}
			} while (is_usd);
			tmp[j] = rnd;
		}
		int sum = 0;
		for (int j = 0;j < 10;j++)
		{
			sum += pool[tmp[j]];
		}
		if (fabs(wanted - sum) < distance)
		{
			for (int j = 0;j < 10;j++)
			{
				best[j] = tmp[j];
			}
			distance = fabs(wanted - sum);
		}
	}
	int sum = 0;
	for (int i = 0;i < 10;i++)
	{
		printf(" %d ", pool[best[i]]);
		sum += pool[best[i]];
	}
	printf("\n\rSum:%d distance:%d\n\r",sum, distance);
}