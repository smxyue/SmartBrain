#include "DucksInPool.h"
#include <stdlib.h>
#include <math.h>
#include <stdio.h>

void DucksInPool::genDD()
{
	int count = 0;
	while (count < SIZE)
	{
		//生成坐标
		double x = (double)rand() / RAND_MAX;
		double y = (double)rand() / RAND_MAX;
		//在单位圆内并且不在圆心
		if (x * x + y * y < 1.0 && (x > 0 && y > 0))
		{

			//x随机符号
			int  zone = rand() % 4;
			switch (zone)
			{
			case 1:
				x = x * -1.0;
				break;
			case 2:
				x = x * -1.0;
				y = y * -1.0;
				break;
			case 3:
				y = y * -1.0;
				break;
			}
			/*if (rand() % 2)
			{
				x = x * -1.0;
			}
			if (rand() % 2)
			{
				y = y * -1.0;
			}*/
			dd[0][count] = x;
			dd[1][count] = y;
			count++;
			//printf("%4f %4f \t", x, y);
		}
	}
	//printf("\n\r");
}
void DucksInPool::genDD2()
{
	int count = 0;
	while (count < SIZE)
	{
		//printf("\n\r");
		//生成坐标
		double angle = (rand() % 3600) / 1800.0 * 3.1415926;
		dd[0][count] = sin(angle) * 0.8;
		dd[1][count] = cos(angle) * 0.8;
		//printf("(%1.4f %1.4f) ", dd[0][count], dd[1][count]);
		count++;
	}
	//printf("\n\r");
}
void DucksInPool::genDD3()
{
	int count = 0;
	while (count < SIZE)
	{
		//printf("\n\r");
		//生成坐标
		double angle = (rand() % 3600) / 1800.0 * 3.1415926;
		dd[0][count] = angle;
		//printf("(%1.4f %1.4f) ", dd[0][count], dd[1][count]);
		count++;
	}
	//printf("\n\r");
}
void DucksInPool::sortDD()
{
	int minp;
	//顺次冒泡
	//先排x
	for (int i = 0;i < SIZE - 1;i++)
	{
		minp = i;
		for (int j = i + 1;j < SIZE;j++)
		{
			if (dd[0][j] < dd[0][minp])
			{
				minp = j;
			}
		}
		if (minp != i)
		{
			double x = dd[0][i];
			double y = dd[1][i];
			dd[0][i] = dd[0][minp];
			dd[1][i] = dd[1][minp];
			dd[0][minp] = x;
			dd[1][minp] = y;
		}
	}
	//再排y
	for (int i = 0;i < SIZE - 1;i++)
	{
		minp = i;
		for (int j = i + 1;j < SIZE;j++)
		{
			if ((dd[0][j] == dd[0][minp]) && (dd[1][j] < dd[1][minp]))
			{
				minp = j;
			}
		}
		if (minp != i)
		{
			double x = dd[0][i];
			double y = dd[1][i];
			dd[0][i] = dd[0][minp];
			dd[1][i] = dd[1][minp];
			dd[0][minp] = x;
			dd[1][minp] = y;
		}
	}
}
void DucksInPool::sortDD3()
{
	int minp;
	//顺次冒泡
	//先排x
	for (int i = 0;i < SIZE - 1;i++)
	{
		minp = i;
		for (int j = i + 1;j < SIZE;j++)
		{
			if (dd[0][j] < dd[0][minp])
			{
				minp = j;
			}
		}
		if (minp != i)
		{
			double x = dd[0][i];
			double y = dd[1][i];
			dd[0][i] = dd[0][minp];
			dd[1][i] = dd[1][minp];
			dd[0][minp] = x;
			dd[1][minp] = y;
		}
	}
}
int DucksInPool::canInOneDD()
{
	// 余弦定理CosA = (c*c + b*b - a*a)/2bc
	double total = 0;
	for (int i = 0;i < SIZE - 1;i++)
	{
		double dltx = dd[0][i + 1] - dd[0][i];
		double dlty = dd[1][i + 1] - dd[1][i];
		double a = sqrt(dltx * dltx + dlty * dlty);
		double b = sqrt(dd[0][i + 1] * dd[0][i + 1] + dd[1][i + 1] * dd[1][i + 1]);
		double c = sqrt(dd[0][i] * dd[0][i] + dd[1][i] * dd[1][i]);
		double cosa = (c * c + b * b - a * a) / (2 * b * c);
		total += acos(cosa);
	}
	if (total <= 3.1415926)
	{
		return 1;
	}
	return 0;
}
int DucksInPool::canInOneDD3()
{
	// 余弦定理CosA = (c*c + b*b - a*a)/2bc
	double total = 0;
	for (int i = SIZE - 1;i>=1;i--)
	{
		
		total += (dd[0][i] - dd[0][i-1]);
	}
	if (total <= 3.1415926)
	{
		return 1;
	}
	return 0;
}
int DucksInPool::canInOneDDD()
{
	// 余弦定理CosA = (c*c + b*b - a*a)/2bc
	double total = 0;
	for (int i = SIZE -1;i >= 0 - 1;i--)
	{
		double dltx = dd[0][i] - dd[0][i-1];
		double dlty = dd[1][i] - dd[1][i-1];
		double a = sqrt(dltx * dltx + dlty * dlty);
		double b = sqrt(dd[0][i-1] * dd[0][i-1] + dd[1][i - 1] * dd[1][i - 1]);
		double c = sqrt(dd[0][i] * dd[0][i] + dd[1][i] * dd[1][i]);
		double cosa = (c * c + b * b - a * a) / (2 * b * c);
		total += acos(cosa);
	}
	if (total <= 3.1415926)
	{
		return 1;
	}
	return 0;
}
void DucksInPool::test(int TestCount)
{
	printf("\n\rDucks in pool is comming...\n\r");
	int binggo = 0;
	int ddd = 0;
	for (int i = 0;i < TestCount;i++)
	{
		genDD3();
		sortDD3();
		int ok = canInOneDD3();
		if (ok)
		{
			binggo++;
		}
		else
		{
			ddd += canInOneDDD();
		}

	}
	printf("\n\r%d tried, %d binggoed, rate %f  ddd %d", TestCount, binggo, ((float)binggo / (float)TestCount),ddd);
}
