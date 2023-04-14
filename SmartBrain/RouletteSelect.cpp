#include "RouletteSelect.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
int RouletteSelect::main(int scale)
{
	if (scale > 100)
		scale = 100;

	initPool(scale,true);
	calc_fitness(scale);
	select(scale);
	
	return 0;
}
//初始化选择池，值代表得分
void RouletteSelect::initPool(int scale,bool random)
{
	for (int i = 0;i < scale;i++)
	{
		if (random)
		{
			pool[i] = rand() % scale;
			if (rand() % 10 == 1)
				pool[i] *= -1;
		}
		else
		{
			pool[i] = i;
		}
	}
}
//计算（累计）选择概率
void RouletteSelect::calc_fitness(int scale)
{
	int min = 0;
	for (int i = 0;i < scale;i++)
	{
		if (min > pool[i])
			min = pool[i];
	}
	int total = 0;
	for (int i = 0;i < scale;i++)
	{
		if (min < 0)
		{
			total += min * -1;
		}
		total += pool[i];
	}
	if (total == 0)
		total = 1;
	for (int i = 0;i < scale;i++)
	{
		int val = pool[i];
		if (min < 0)
			val -= min;
		
		fitness[i] = val / (double)total;
		if (i>0)
		{
			fitness[i] += fitness[i - 1];
		}
		
	}
}
int hehe(int n)
{
	if (n == 1)
		return 1;
	else
		return n + hehe(n - 1);
}
//做scale次选择
void RouletteSelect::select(int scale)
{
	for (int i = 0;i < scale;i++)
		selected[i] = 0;
	for (int i = 0;i < hehe(scale);i++)
	{
		double r = rand() / (double)(RAND_MAX + 1);
		int index = 0;
		while (fitness[index] < r)
			index++;
		selected[index] ++;
	}
	for (int i = 0;i < scale;i++)
	{
		printf("%3d [%3d fit %3f]:%3d\n\r",i,pool[i], fitness[i], selected[i]);
	}
}

