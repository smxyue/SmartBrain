#include "Robi.h"
#include <stdlib.h>
#include <stdio.h>

void Robi::init()
{
	jarCount = 0;
	for (int i = 0;i < 10;i++)
	{
		for (int j = 0;j < 10;j++)
		{
			if (rand() % 2 == 0)
			{
				matrix[i][j] = 1;
				jarCount++;
			}
			else
			{
				matrix[i][j] = 0;
			}
		}
	}
}
void Robi::printRobi()
{
	printf("\n\rJar counts:%d", jarCount);
}
void Robi::goStep(int ch)
{
	//0=���ƶ���1=�����ƶ���2=���ƶ���3=�����ƶ���4=������5=��ʰ���ӣ�6=����ƶ�
	//�������й��Ӳ����ռ���10�֡��ռ����Ӷ���������û�У�-1�֡����ײ����ǽ���ᱻ��5�֣�������ԭ���ĸ��ӡ�
	switch(ch)
	{
		case 0:
			if (currentY > 0)
			{
				currentY--;
			}
			else
			{
				score -= 5;
			}
			break;
		case 1:
			if (currentY < 9)
			{
				currentY++;
			}
			else
			{
				score -= 5;
			}
			break;
		case 2:
			if (currentX < 9)
			{
				currentX++;
			}
			else
			{
				score -= 5;
			}
			break;
		case 3:
			if (currentX > 0)
			{
				currentX--;
			}
			else
			{
				score -= 5;
			}
			break;
		case 4:
			break;
		case 5:
			if (matrix[currentX][currentY] == 1)
			{
				//matrix[currentX][currentY] = 0;
				score += 10;
			}
			else
			{
				score -= 1;
			}
			break;
		case 6:
			goStep(rand() % 6);
			break;
			
	}
}
int Robi::runG()
{
	score = 0;
	for (int i = 0;i < 200;i++)
	{
		goStep(G[i]-'0');
	}
	return score;
}
int Robi::runM()
{
	score = 0;
	for (int i = 0;i < 200;i++)
	{
		goStep(M[i] - '0');
	}
	return score;
}
void Robi::test(int nTimes)
{
	int mTotal = 0, gTotal = 0;
	//printf("\n\rRobo method test...\n\r");
	for (int i = 0;i < nTimes;i++)
	{
		init();
		mTotal += runM();
		gTotal += runG();
	}
	printf("\n\rM total:%d    G total:%d", mTotal / nTimes, gTotal / nTimes);

}
int Robi::myGo()
{
	int myStep = 0,dir = 0,myScore = 0;
	//printf("\n\rRobo method test...\n\r");
	myScore = 0;
	init();
	printf("\n\rJar counts:%d", jarCount);
	currentX = 0;
	currentY = 0;
	dir = 0;
	//̽��stepmax��
	while (myStep < STEPMAX)
	{
		//�ȼ�ǰλ��
		if (matrix[currentX][currentY] == 1)
		{
			myScore += 10;
		}
		else
		{
			myScore -= 1;
		}
		//�ƶ�һ��
		//������ң����Ϻ��£���ǽ��һ�У�����
		if (dir == 0)
		{
			currentX++;
			if (currentX > 9)
			{
				if (currentY < 9)
				{
					currentY++;
					dir = -1;
					myScore -= 5;
					currentX = 9;
				}
				else
					break;
			}
		}
		else
		{
			currentX--;
			if (currentX < 0)
			{
				if (currentY < 9)
				{
					currentY++;
					dir = 0;
					currentX = 0;
					myScore -= 5;
				}
				else
					break;
			}
		}
		myStep++;
	}
	printf("\n\rX end:%d  Y end :%d   score:%d",currentX,currentY, myScore);
	return score;
}