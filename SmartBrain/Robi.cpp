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
	score = 0;
	currentX = 0;
	currentY = 0;
}
void Robi::printRobi()
{
	printf("\n\rJar counts:%d", jarCount);
}
int Robi::goStep(int ch)
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
				return -5;
			}
			break;
		case 1:
			if (currentY < 9)
			{
				currentY++;
			}
			else
			{
				return -5;
			}
			break;
		case 2:
			if (currentX < 9)
			{
				currentX++;
			}
			else
			{
				return -5;
			}
			break;
		case 3:
			if (currentX > 0)
			{
				currentX--;
			}
			else
			{
				return  -5;
			}
			break;
		case 4:
			break;
		case 5:
			if (matrix[currentX][currentY] == 1)
			{
				matrix[currentX][currentY] = 0;
				return 10;
			}
			else
			{
				return -1;
			}
			break;
		case 6:
			return goStep(rand() % 6);
			break;
			
	}
	return 0;
}
void numGen(char* Gen, char* Tar)
{
	for (int i = 0;i < STEPMAX;i++)
		Tar[i] = Gen[i] - '0';
}
int Robi::runG()
{
	char gen[STEPMAX];
	numGen(G, gen);
	return run(gen);
}

int Robi::runM()
{
	char gen[STEPMAX];
	numGen(M, gen);
	return run(gen);

}
int Robi::run(char* gen)
{
	int score = 0;
	init();
	for (int i = 0;i < STEPMAX;i++)
	{
		score +=goStep(gen[i]);
	}
	return score;
}
void Robi::test(int nTimes)
{
	char myGen[300];
	int len = genWonderGen(myGen);
	int mTotal = 0, gTotal = 0,yTotal = 0;
	//printf("\n\rRobo method test...\n\r");
	for (int i = 0;i < nTimes;i++)
	{
		init();
		mTotal += runM();
		gTotal += runG();
		yTotal += run(myGen);
	}
	printf("\n\rM total:%d    G total:%d	Y total %d", mTotal / nTimes, gTotal / nTimes,yTotal / nTimes);
	printf("\n\r");
	for (int i = 0;i < len;i++)
		printf("%d", myGen[i]);
	printf("\n\r len%d", len);

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
int Robi::genWonderGen(char *myGen)
{
	for (int i = 0;i < 200;i++)
		myGen[i] = 0;
	int index = 0;
	for (int i = 0;i < 5;i++)
	{
		for (int j = 0;j < 10;j++)
		{
			myGen[index++] = 2;
			myGen[index++] = 5;
		}
		myGen[index++] = 1;
		myGen[index++] = 5;
		for (int j = 0;j < 10;j++)
		{
			myGen[index++] = 3;
			myGen[index++] = 5;
		}
		if (i < 4)
		{
			myGen[index++] = 2;
			myGen[index++] = 5;
		}
	}
	return index;
}


void Robi::initGenLibs()
{
	for (int i = 0;i < 200;i++)
	{
		for (int j = 0;j < STEPMAX;j++)
			genLibs[i][j] = 0;
	}
}


void Robi::randomGen(char* gen)
{
	for (int i = 0;i < STEPMAX;i++)
	{
		gen[i] = rand() % 6;
	}
}

void Robi::newGeneration()
{
	for (int j = 2;j < 200;j += 2)
	{
			int p = rand() % STEPMAX;
			for (int k = 0;k < p;k++)
			{
				genLibs[j][k] = genLibs[1][k];
				genLibs[j + 1][k] = genLibs[0][k];
			}
			for (int k = p;k < STEPMAX;k++)
			{
				genLibs[j][k] = genLibs[0][k];
				genLibs[j + 1][k] = genLibs[1][k];
			}
			//变异
			for (int k = 0;k < 1;k++)
			{
				p = rand() % STEPMAX;
				int old = genLibs[j][p];
				int change = rand() % 7;
				while (change == old)
				{
					change = rand() % 7;
				}
				genLibs[j][p] = change;
				p = rand() % STEPMAX;
				old = genLibs[j + 1][p];
				change = rand() % 7;
				while (change == old)
				{
					change = rand() % 7;
				}
				genLibs[j + 1][p] = change;
			}
	}
}
void Robi::getBetter()
{
	int score[200];
	for (int i = 0;i < 200;i++)
	{
		score[i] = run(genLibs[i]);
	}
	int a0 = score[0];
	int ai = 0;
	int b0 = score[1];
	int bi = 1;
	//确保选出最好时不会挤掉次好
	if (a0 < b0)
	{
		for (int i = 0;i < STEPMAX;i++)
		{
			int tmp = genLibs[0][i];
			genLibs[0][i] = genLibs[1][i];
			genLibs[1][i] = tmp;
		}
		a0 = b0;
		b0 = score[1];
		int tmp = score[0];
		score[0] = score[1];
		score[1] = tmp;

	}
	for (int i = 1;i < 200;i++)
	{
		if (score[i] > a0)
		{
			a0 = score[i];
			ai = i;
		}
	}
	if (ai != 0)
	{
		for (int i = 0;i < STEPMAX;i++)
		{
			genLibs[0][i] = genLibs[ai][i];
		}
	}
	for (int i = 2;i < 200;i++)
	{
		if (score[i] > b0)
			if (i != ai)
			{
				b0 = score[i];
				bi = i;
			}
	}
	if (bi != 1)
	{
		for (int i = 0;i < STEPMAX;i++)
		{
			genLibs[1][i] = genLibs[bi][i];
		}
	}
}
void Robi::revlution(int nTimes)
{
	initGenLibs();
	randomGen(genLibs[0]);
	randomGen(genLibs[1]);
	for (int i = 0;i < nTimes;i++)
	{
		newGeneration();
		getBetter();
	}
}
void Robi::printGenLibItem(int p)
{
	for (int i = 0;i < STEPMAX;i++)
		printf("%d", genLibs[p][i]);

	printf("\n\r");
}
void Robi::printGenLib()
{
	for (int i = 0;i < 200;i++)
	{
		printf("%d\n\r", i);
		printGenLibItem(i);
	}
}
void Robi::check()
{
	initGenLibs();
	randomGen(genLibs[0]);
	randomGen(genLibs[1]);
	int a0 = run(genLibs[0]);
	int b0 = run(genLibs[1]);
	printf("a0 %d  b0 %d \n\r", a0, b0);

	for (int i = 0;i < 1000;i++)
	{
		newGeneration();
		getBetter();
	}
	a0 = run(genLibs[0]);
	b0 = run(genLibs[1]);
	printf("a0 %d  b0 %d \n\r", a0, b0);
}