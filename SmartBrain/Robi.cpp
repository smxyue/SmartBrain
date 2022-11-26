#define _CRT_SECURE_NO_WARNINGS
#include "Robi.h"
#include <stdlib.h>
#include <stdio.h>

int Robi::init()
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
	currentX = 0;
	currentY = 0;
	return jarCount;
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
	//changeMatrix(20);
	for (int i = 0; i < STEPMAX; i++)
	{
		score += goStep(gen[i]);
	}
	return score;
}
int Robi::runBatch(char* gen,int nTimes = 3)
{
	int score = 0;
	for (int k = 0; k < nTimes; k++)
	{
		//init();
		for (int i = 0; i < STEPMAX; i++)
		{
			score += goStep(gen[i]);
		}
	}
	return (score/nTimes);
}
int Robi::actionRate(char * gen,int ch)
{
	int chCount = 0;
	for (int i = 0; i < STEPMAX; i++)
	{
		if (gen[i] == ch)
		{
			chCount++;
		}
	}
	printf("%d ", chCount);
	return chCount;
}
void Robi::genActionRate(char* gen)
{
	int ch[7];
	for (int i = 0; i < 7; i++)
		ch[i] = 0;
	for (int i = 0; i < STEPMAX; i++)
	{
		ch[gen[i]] ++;
	}
	printf("gen action rate\n\r");
	for (int i = 0; i < 7; i++)
	{
		printf("%d:%d  ", i, ch[i]);
	}
	printf("\n\r");
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
	return myScore;
}
int Robi::genWonderGen(char *myGen)
{
	for (int i = 0;i < STEPMAX;i++)
		myGen[i] = 0;
	int index = 0;
	for (int i = 0;i < 5;i++)
	{
		for (int j = 0;j < 10;j++)
		{
			myGen[index++] = 5;
			myGen[index++] = 2;
		}
		if (i < 4)
		{
			myGen[index++] = 1;
		}

		for (int j = 0;j < 10;j++)
		{
			myGen[index++] = 5;
			myGen[index++] = 3;
		}
		if (i < 4)
		{
			myGen[index++] = 1;
		}
	}
	return index;
}


void Robi::initGenLibs()
{
	for (int i = 0;i < GenLibPool;i++)
	{
		for (int j = 0;j < STEPMAX;j++)
			genLibs[i][j] = 0;
	}
}

void Robi::astart()
{
	char g0[] = { 4, 5, 4, 1, 5, 0, 4, 2, 2, 5, 2, 3, 1, 3, 5, 1, 1, 5, 5, 0, 5, 1, 2, 4, 2, 4, 5, 0, 5, 3, 5, 0, 5, 0, 1, 4, 5, 2, 0, 5, 1, 5, 2, 5, 0, 2, 4, 5, 1, 5, 4, 2, 4, 5, 2, 0, 2, 2, 5, 1, 3, 1, 1, 5, 1, 2, 4, 4, 5, 0, 5, 0, 3, 0, 5, 2, 0, 1, 5, 1, 5, 1, 3, 1, 5, 5, 4, 5, 1, 5, 3, 4, 0, 5, 4, 0, 5, 0, 3, 5, 5, 1, 5, 3, 5, 3, 1, 0, 1, 5, 0, 3, 1, 0, 1, 5, 3, 5, 4, 4, 1, 1, 2, 5, 1, 2, 1, 5, 2, 5, 1, 4, 5, 2, 0, 0, 2, 5, 1, 5, 2, 3, 3, 5, 4, 2, 0, 0, 3, 3, 5, 3, 5, 5, 1, 5, 2, 5, 3, 3, 5, 1, 5, 3, 2, 5, 5, 1, 5, 3, 5, 3, 5, 0, 4, 2, 4, 4, 6, 6, 2, 1, 4, 0, 0, 3, 5, 1, 2, 3, 3, 3, 5, 4, 6, 2, 5, 6, 6, 0 };
	char g1[] = { 4, 5, 4, 1, 5, 0, 4, 2, 2, 5, 2, 3, 1, 3, 5, 1, 1, 5, 5, 0, 5, 1, 2, 4, 2, 4, 5, 0, 5, 3, 5, 0, 5, 0, 1, 4, 5, 2, 0, 5, 1, 5, 2, 5, 0, 2, 4, 5, 1, 5, 4, 2, 4, 5, 2, 0, 2, 2, 5, 1, 3, 1, 1, 5, 1, 2, 4, 4, 5, 0, 5, 0, 3, 0, 5, 2, 0, 1, 5, 1, 5, 1, 3, 1, 5, 5, 4, 5, 1, 5, 3, 4, 0, 5, 4, 0, 5, 0, 3, 5, 5, 1, 5, 3, 5, 3, 1, 0, 1, 5, 0, 3, 1, 0, 1, 2, 3, 5, 0, 4, 1, 1, 2, 5, 1, 2, 1, 5, 2, 5, 1, 4, 5, 2, 0, 0, 2, 5, 1, 5, 2, 3, 3, 5, 4, 2, 0, 0, 3, 3, 5, 3, 5, 5, 1, 5, 2, 5, 3, 3, 5, 1, 5, 3, 2, 5, 5, 1, 5, 3, 5, 3, 5, 0, 4, 2, 4, 0, 6, 6, 2, 1, 4, 0, 0, 3, 5, 1, 2, 3, 3, 3, 5, 4, 6, 2, 5, 6, 6, 0 };
	for (int i = 0; i < STEPMAX; i++)
	{
		genLibs[0][i] = g0[i];
		genLibs[1][i] = g1[i];
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
	for (int j = 2; j < GenLibPool; j += 2)
	{
		int p = rand() % STEPMAX;
		for (int k = 0; k < p; k++)
		{
			genLibs[j][k] = genLibs[1][k];
			genLibs[j + 1][k] = genLibs[0][k];
		}
		for (int k = p; k < STEPMAX; k++)
		{
			genLibs[j][k] = genLibs[0][k];
			genLibs[j + 1][k] = genLibs[1][k];
		}
		//变异
		for (int k = 0; k < 1; k++)
		{
			p = rand() % STEPMAX;
			int old = genLibs[j][p];
			int change = rand() % 6;
			while (change == old)
			{
				change = rand() % 6;
			}
			genLibs[j][p] = change;
			p = rand() % STEPMAX;
			old = genLibs[j + 1][p];
			change = rand() % 6;
			while (change == old)
			{
				change = rand() % 6;
			}
			genLibs[j + 1][p] = change;
		}
	}
}
void Robi::nextEra()
{
	for (int j = 2; j < GenLibPool; j += 2)
	{
		//顺次从开始到结尾组合两段基因
		for (int k = 0; k < j; k++)
		{
			genLibs[j][k] = genLibs[1][k];
			genLibs[j + 1][k] = genLibs[0][k];
		}
		for (int k = j; k < STEPMAX; k++)
		{
			genLibs[j][k] = genLibs[0][k];
			genLibs[j + 1][k] = genLibs[1][k];
		}
		//变异
		int p;
		for (int k = 0; k < 1; k++)
		{
			p = rand() % STEPMAX;
			int old = genLibs[j][p];
			int change = rand() % 6;
			while (change == old || change ==4)
			{
				change = rand() % 6;
			}
			genLibs[j][p] = change;

			p = rand() % STEPMAX;
			old = genLibs[j + 1][p];
			change = rand() % 6;
			while (change == old || change == 4)
			{
				change = rand() % 6;
			}
			genLibs[j + 1][p] = change;
		}
	}
}
void Robi::getBetter()
{
	int score[GenLibPool];
	//计算所有方法的得分
	for (int i = 0;i < GenLibPool;i++)
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
	for (int i = 1;i < GenLibPool;i++)
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
	for (int i = 2;i < GenLibPool;i++)
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
	//printf("A0 %d A1 %d\n\r", a0, b0);
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
	printf("{");
	for (int i = 0;i < STEPMAX;i++)
		printf("%d,", genLibs[p][i]);

	printf("}\n\r");
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
	//initGenLibs();
	if (getData() == 0)
	{
		randomGen(genLibs[0]);
		randomGen(genLibs[1]);
	}
	else
	{
		//printGenLibItem(0);
		//printGenLibItem(1);
	}
	//astart();
	int a0 = runBatch(genLibs[0]);
	int b0 = runBatch(genLibs[1]);
	printf("a0 %d  b0 %d  ", a0, b0);
	actionRate(genLibs[0], 4);
	actionRate(genLibs[1], 4);
	printf("\n\r");
	for (int i = 0;i < 500;i++)
	{
		nextEra();
		//printGenLib();
		getBetter();
		//actionRate(genLibs[0],4);
	}
	a0 = runBatch(genLibs[0]);
	b0 = runBatch(genLibs[1]);
	printf("a0 %d  b0 %d  ", a0, b0);
	actionRate(genLibs[0], 4);
	actionRate(genLibs[1], 4);
	printf("\n\r");
	saveData();
}
void Robi::saveData()
{
	FILE* fp;
	if ((fp = fopen(DATAFILE, "w")) == NULL)
	{
		printf("\n\rcan not open file %s", DATAFILE);
		return;
	}
	for (int i = 0; i < STEPMAX; i++)
	{
		fputc(genLibs[0][i] + '0', fp);
		fputc(genLibs[1][i]+'0', fp);
	}
	fclose(fp);
}
int Robi::getData()
{
	FILE* fp;
	if ((fp = fopen(DATAFILE, "r")) == NULL)
	{
		printf("\n\rcan not open file %s", DATAFILE);
		return 0;
	}
	for (int i = 0; i < STEPMAX; i++)
	{
		char ch = fgetc(fp)-'0';
		genLibs[0][i] = ch;
		ch = fgetc(fp)-'0';
		genLibs[1][i] = ch;
	}
	fclose(fp);
	return 1;
}
void Robi::changeMatrix(int nCount)
{
	if (nCount > 30)
		nCount = 30;
	/*for (int i = 0; i < nCount; i++)
	{
		int x1 = rand() % 10;
		int y1 = rand() % 10;
		while (matrix[x1][y1] == 0)
		{
			x1 = rand() % 10;
			y1 = rand() % 10;
		}
		matrix[x1][y1] = 0;
		int x2 = rand() % 10;
		int y2 = rand() % 10;
		while (matrix[x2][y2] == 1)
		{
			x2 = rand() % 10;
			y2 = rand() % 10;
		}
		matrix[x1][y1] = 1;
	}*/
	int n0 = 0, n1 = 0;
	while (n0 < nCount && n1 < nCount)
	{
		int x = rand() % 10;
		int y = rand() % 10;
		if (n0 < nCount)
		{
			if (matrix[x][y] == 0)
			{
				matrix[x][y] = 1;
				n0++;
				continue;
			}
		}
		if (n1 < nCount)
		{
			if (matrix[x][y] == 1)
			{
				matrix[x][y] = 0;
				n1++;
				continue;
			}
		}
	}

}
void Robi::printGen(char* gen)
{
	printf("{");
	for (int i = 0; i < STEPMAX; i++)
	{
		printf("%d", gen[i]);
	}
	printf("}");
}
void Robi::test1()
{
	char myGen[STEPMAX+100];
	genWonderGen(myGen);
	if (getData() == 0)
	{
		randomGen(genLibs[0]);
		randomGen(genLibs[1]);
	}
	int jarCount = init();
	int starta0 = run(genLibs[0]);
	int startb0 = run(genLibs[1]);

	for(int j=0;j<100;j++)
	{
		int s1 = run(myGen);
		int s2 = runM();
		int s3 = runG();
		for (int i = 0; i < 1000; i++)
		{
			nextEra();
			//printGenLib();
			getBetter();
			//actionRate(genLibs[0],4);
		}
		int a0 = run(genLibs[0]);
		int b0 = run(genLibs[1]);
		printf("%5d\n\r", a0);
		//printf("Expect:%2d S1:%3d  S2:%3d S3:%3d     a0 %3d  b0 %3d   dlta0:%d   dltb0:%d\n\r", jarCount * 10 - (STEPMAX - jarCount), s1, s2, s3, a0, b0, a0 - starta0, b0 - startb0);
		//printGen(genLibs[0]);
		//printGen(genLibs[1]);
		saveData();

	}
}
void Robi::test2(int nTimes)
{
	for (int i = 0; i < nTimes; i++)
	{
		init();
		currentX = rand() % 10;
		currentY = rand() % 10;
		int ms = runM();
		int gs = runG();
		printf("M:%d   G:%d\n\r", ms, gs);
	}
}