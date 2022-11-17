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
	//0=向北移动，1=向南移动，2=向东移动，3=向西移动，4=不动，5=捡拾罐子，6=随机移动
	//格子中有罐子并且收集，10分。收集罐子而格子中又没有，-1分。如果撞到了墙，会被罚5分，并弹回原来的格子。
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
	//探索stepmax步
	while (myStep < STEPMAX)
	{
		//先捡当前位置
		if (matrix[currentX][currentY] == 1)
		{
			myScore += 10;
		}
		else
		{
			myScore -= 1;
		}
		//移动一步
		//先左后右，先上后下，遇墙下一行，倒退
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