#include "DucksInPool.h"
#include <stdlib.h>
#include <math.h>
#include <stdio.h>


void DucksInPool::printDD()
{
	printf("\n\r(");
	for (int i = 0;i < SIZE;i++)
	{
		printf(" %f ", dd[0][i]);
	}
	printf(")\n\r");
	printf("total:%f  pre:%f\n\r", totalAngle(), dd[0][SIZE - 1] - dd[0][0]);

}
void DucksInPool::genDD()
{
	int count = 0;
	while (count < SIZE)
	{
		//��������
		float x = rand()%1000 / 1000.0;
		float y = rand()%1000 / 1000.0;
		//�ڵ�λԲ�ڲ��Ҳ���Բ��
		if (x * x + y * y < 1.0 && (x > 0 && y > 0))
		{

			//x�������
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
		//��������
		float angle = (rand() % 1000) / 500 * 3.1415926;
		dd[0][count] = sin(angle) ;
		dd[1][count] = cos(angle) ;
		//printf("(%1.4f %1.4f) ", dd[0][count], dd[1][count]);
		count++;
	}
	//printf("\n\r");
}

void DucksInPool::sortDD()
{
	int minp;
	//˳��ð��
	//����x
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
	//����y
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

void DucksInPool::reSortDD()
{
	float amm = dd[0][SIZE - 1] - dd[0][0];
	if (amm > 180)
		amm -= 180;
	if (amm <= dd[0][0])
	{
		for (int i = 0;i < SIZE - 2;i++)
		{
			dd[0][i + 1] = dd[0][i] + amm;
		}
		dd[0][0] = amm;
	}

}
int DucksInPool::canInOneDD()
{
	// ���Ҷ���CosA = (c*c + b*b - a*a)/2bc
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


void DucksInPool::test(int TestCount)
{
	printf("\n\rDucks in pool is comming...\n\r");
	int binggo = 0;
	int ddd = 0;
	for (int i = 0;i < TestCount;i++)
	{
		genDD();
		sortDD();
		int ok = canInOneDD();
		if (ok)
		{
			binggo++;
		}


	}
	printf("\n\r%d tried, %d binggoed, rate %f ", TestCount, binggo, ((float)binggo / (float)TestCount));
}
float DucksInPool::totalAngle()
{
	//���ȣ��ٶ����һ��Ѽ�ӵ���һ��Ѽ�ӵļн���С
	int pre = SIZE - 1, next = 0; //preǰһ��λ�ã���ǰʱ�Ӵ���ǰ��next��һ��λ�ã�����ʱ�Ӵ�����
	float total = dd[0][SIZE - 1] - dd[0][0];
	if (total > 180)
		total -= 180;
	for (int i = 1;i < SIZE ;i++)
	{
		float angle = dd[0][i] - dd[0][i - 1];
		if (angle > 180)
			angle -= 180;
		if (angle < total)
		{
			//�ҵ���С�ļнǺ�λ��
			total = angle;
			pre = i - 1;
			next = i;
		}
	}
	//printf("min angle [%d %d]:%f\n\r", pre, next, total);

	for (int i = 0;i < SIZE - 2;i++)
	{
		int prepre;
		float preAngle;
		if (pre == 0)
		{
			prepre = SIZE - 1;
			preAngle = dd[0][SIZE - 1] - dd[0][0];
			/*if (preAngle > 180)
				preAngle -= 180;*/

		}
		else
		{
			prepre = pre - 1;
			preAngle = dd[0][pre] - dd[0][prepre];
		}

		int nextnext;
		float nextAngle;
		if (next == SIZE - 1)
		{
			nextnext = 0;
			nextAngle = dd[0][SIZE - 1] - dd[0][0];
			/*if (nextAngle > 180)
				nextAngle -= 180;*/
		}
		else
		{
			nextnext = next + 1;
			nextAngle = dd[0][nextnext] - dd[0][next];
		}

		
		
		if (preAngle < nextAngle)
		{
			total += preAngle;
			//printf("pre angle [%d %d]:%f\n\r", prepre, pre, preAngle);
			pre = prepre;
		}
		else
		{
			total += nextAngle;
			//printf("next angle [%d %d]:%f\n\r", next, nextnext, nextAngle);
			next = nextnext;
		}
	}
	return total;
}
void DucksInPool::test2(int TestCount)
{
	printf("\n\rDucks in pool is comming...\n\r");
	int binggo = 0;
	int ddd = 0;
	for (int i = 0;i < TestCount;i++)
	{
		//����һ��Ѽ��
		int count = 0;
		while (count < SIZE)
		{
			float angle = (rand() % 1000) / 1000.0 * 360;
			dd[0][count] = angle;
			dd[1][count] = 0;
			count++;
		}
		sortDD();
		//reSortDD();
		//printDD();
		//printf("\n\r");
		//�Ƿ���һ����Բ
		if (totalAngle() <= 180)
			ddd++;
		if (dd[0][SIZE - 1] - dd[0][0] <= 180)
		{
			binggo++;
		}
		else
		{
			int amm = dd[0][SIZE - 1] - dd[0][0];
			if (amm > 180)
				amm -= 180;
			if (amm + (dd[0][SIZE - 1] - dd[0][1]) < 180)
			{
				binggo++;
			}
		}
	}
	printf("\n\r%d tried, %d binggoed, rate %f ddd: %d", TestCount, binggo, ((float)binggo / (float)TestCount),ddd);
}
