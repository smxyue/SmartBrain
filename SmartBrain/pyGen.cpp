#include "pyGen.h"
#include <stdlib.h>
#include <corecrt_math.h>

void pyGen::initPool()
{
	int sum = 0;
	for (int i = 0; i < 50; i++)
	{
		pool[i] = rand()%1000;
		sum += pool[i];
	}
	wanted = sum / 10;
}
void pyGen::getGroup(int* group)
{
	int groupIndex[10];
	int index = 9;
	while (index >= 0)
	{
		int r = rand() % 50;
		bool is_used = false;
		for (int j = 0; j < 9 - index; j++)
		{
			if (groupIndex[j] == r)
				is_used = true;
		}
		if (!is_used)
		{
			groupIndex[9 - index] = r;
			index--;
		}
	}
	for (int i = 0; i < 10; i++)
	{
		group[i] = pool[groupIndex[i]];
	}
}

void pyGen::initPopulation()
{
	for (int i = 0; i < POP_SIZE; i++)
	{
		getGroup(population[i]);
	}
}

double* pyGen::getFitness()
{
	double fitness[POP_SIZE];
	for (int i = 0; i < POP_SIZE; i++)
	{
		int sum = 0;
		for (int j = 0; j < 10; j++)
			sum += population[i][j];
		int value = fabs(wanted - sum);
		if (value == 0)
			fitness[i] = 10;
		else
			fitness[i] = 1 / value;
	}
	return fitness;		
}
void pyGen::select()
{
	int newPopulation[POP_SIZE][10];
	double * fitness = getFitness();
	double totalfitness = 0;
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

			for i in range(len(old_answer)//2): #繁衍次数
				temp = []#保存每次抽取的父本和母本
				for j in range(2) :#抽取父本和母本
					rand = random.uniform(0, 1)
					for k in range(len(error_one)) :
						if k == 0 :
							if rand < error_one[k] :
								temp.append(old_answer[k])
							else :
								if rand >= error_one[k - 1] and rand < error_one[k] :
									temp.append(old_answer[k])
									rand = random.randint(0, 6) # 选择连续的3个数作为交换，一共10个数
									temp_1 = temp[0][:rand] + temp[1][rand:rand + 3] + temp[0][rand + 3:]
									temp_2 = temp[1][:rand] + temp[0][rand:rand + 3] + temp[1][rand + 3:]
									if len(set(temp_1)) == 10 and len(set(temp_2)) == 10: # 判断子代是否具有相同元素，若有则保存父本和母本
										result.append(temp_1)
										result.append(temp_2)
									else:
	result.append(temp[0])
		result.append(temp[1])
		return result
}