#include "pyGen.h"
#include <stdlib.h>
#include <corecrt_math.h>
#include <stdio.h>

//��ʼ�����ݼ������ѡ��0-1000֮���50�������ѡ�񼯣�����ֵΪwanted�����ܺ͵�1/10
void pyGen::initPool()
{
	printf("���ݼ�[");
	int sum = 0;
	for (int i = 0; i < 50; i++)
	{
		pool[i] = rand()%1000;
		sum += pool[i];
		printf(" %d ", pool[i]);
	}
	wanted = (double)sum / 10;
	printf("]\n\r����ֵ:%f\n\r",wanted);
}
//��ѡ�������ѡ��10����ͬ�������һ�����
void pyGen::getGroup(int* group)
{
	int groupIndex[10];
	int count = 9;
	while (count >= 0)
	{
		int r = rand() % 50;
		bool is_used = false;
		//�鿴�Ѿ����ɵ��Ƿ��ظ�
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

//��ʼ����Ⱥ
void pyGen::initPopulation()
{
	for (int i = 0; i < POP_SIZE; i++)
	{
		getGroup(population[i]);
	}
}
//������Ӧ��
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

//ѡ�񲢽���
void pyGen::select()
{
	int newPopulation[POP_SIZE][10];
	getFitness();
	double totalfitness = 0;
	//�����ۼƱ���
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
	//ÿ��ѡ��������������
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
		//����
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
		//��������Ⱥ
		for (int j = 0;j < 10;j++)
		{
			newPopulation[newIndex][j] = crossed[0][j];
			newPopulation[newIndex + 1][j] = crossed[1][j];
		}
		newIndex += 2;	
	}
	//��������Ⱥ
	for(int i=0;i<POP_SIZE;i++)
		for (int j = 0;j < 10;j++)
		{
			population[i][j] = newPopulation[i][j];
		}
}
/*
������ѡ�񽻲溯���У��ҷ�����һЩ���ܵ����⡣�ҽ������г����£�����������Ӧ�Ľ��飺

������������⣺��ѡ��Ⱦɫ��ͼ��㽻�����ʱ����ʹ����rand()������Ȼ�����������÷���������ȷ�ģ���Ϊ�����������������һ���ǳ�С�ķ�Χ�ڡ���ʹ�����·����滻��Щ�У�

int rnd = rand() / double(RAND_MAX + 1);
```
�滻Ϊ��
````cpp
double rnd = static_cast<double>(rand()) / (RAND_MAX + 1);
```

````cpp
double crossRnd = rand() / (double)(RAND_MAX + 1);
```
�滻Ϊ��
````cpp
double crossRnd = static_cast<double>(rand()) / (RAND_MAX + 1);
```

ѡ������еĸ��ʷֲ����⣺������ѡ������У���ʹ�����ۻ���Ӧ��ֵ��Ȼ��������ܻᵼ��ĳЩȾɫ�����Ƶ���ر�ѡ�С�Ϊ�˽��������⣬������ʹ�����̶�ѡ�񷽷��������ַ����У�ÿ��Ⱦɫ���ѡ��������������Ӧ�ȳ����ȡ������԰������·�ʽ�޸�ѡ����̣�

��select()�����ڣ���for (int i = 0; i < POP_SIZE; i++)ѭ��֮��������´��룺

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

Ȼ�󣬽�ѡ������е�`for (int j = 0; j < 2; j++)`ѭ���滻Ϊ���´��룺

````cpp
for (int j = 0; j < 2; j++) {
	int index = selected_indices[2 * i + j];
	for (int m = 0; m < 10; m++) {
		temp[j][m] = population[index][m];
	}
}
```

���������ѡ�����⣺�����Ľ�������У���ʹ���˹̶��Ľ���㡣Ϊ�����Ӷ����ԣ����������ѡ�񽻲�㡣�����´��룺

int rnd = rand() % 6;
```
�滻Ϊ��
````cpp
int rnd = rand() % 9 + 1; // Random crossover point between 1 and 9
```
�����Ĵ��������Щ���ĺ���Ӧ���ܹ��������ƵĽ���Ч�����������Ҫ��һ���İ���������ʱ�����ҡ�
*/
//����
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
	printf("\n\r����ѡ��%d[",best);
	for (int i = 0;i < 10;i++)
	{
		sum += population[best][i];
		printf(" %d ", population[best][i]);
	}

	printf("\n\r����ֵ:%d\n\r", sum);
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