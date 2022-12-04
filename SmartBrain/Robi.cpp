#define _CRT_SECURE_NO_WARNINGS
#include "Robi.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

void Robi::init()
{
	jarCount = 0;
	for (int i = 0;i < 10;i++)
	{
		for (int j = 0;j < 10;j++)
		{
			matrix[i][j] = 0;
		}
	}
	while (jarCount < 50)
	{
		int x = rand() % 10;
		int y = rand() % 10;
		if (matrix[x][y] == 0)
		{
			matrix[x][y] = 1;
			jarCount++;
		}
	}
	currentX = 0;
	currentY = 0;
}
void Robi::printRobi()
{
	printf("\n\rJar counts:%d", jarCount);
}
void Robi::printMatrix()
{
	for (int i = 0; i < 10; i++)
	{
		printf("\n\r");
		for (int j = 0; j < 10; j++)
		{
			printf(" %1d ", matrix[i][j]);
		}
	}
}
int Robi::goStep(int ch)
{
	//0=向北移动，1=向南移动，2=向东移动，3=向西移动，4=不动，5=捡拾罐子，6=随机移动
	//格子中有罐子并且收集，10分。收集罐子而格子中又没有，-1分。如果撞到了墙，会被罚5分，并弹回原来的格子。
	//0=向上，1=向下，2=向左，3=向右，4=不动，5=捡拾罐子，6=随机移动

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
			if (currentX > 0)
			{
				currentX--;
			}
			else
			{
				return -5;
			}
			break;
		case 3:
			if (currentX < 9)
			{
				currentX++;
			}
			else
			{
				return  -5;
			}
			break;
		case 4:
			break;
		case 5:
			if (matrix[currentY][currentX] == 1)
			{
				matrix[currentY][currentX] = 0;
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
	currentX = 0;
	currentY = 0;
	//init();
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
	int index = 0;
	for (int i = 0;i < 5;i++)
	{
		for (int j = 0;j < 10;j++)
		{
			myGen[index++] = 5;
			myGen[index++] = 2;
		}
		index--;
		if (i < 4)
		{
			myGen[index++] = 1;
		}

		for (int j = 0;j < 10;j++)
		{
			myGen[index++] = 5;
			myGen[index++] = 3;
		}
		index--;
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
		for (int j = 0; j < STEPMAX; j++)
			genLibs[i][j] = rand() % 6 ;
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
		gen[i] = rand() % 7;
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
	for (int j = 1; j < GenLibPool-1; j += 2)
	{
		//顺次从开始到结尾组合两段基因
		for (int k = 0; k < j; k++)
		{
			genLibs[j+1][k] = genLibs[1][k];
			genLibs[j + 2][k] = genLibs[0][k];
		}
		for (int k = j; k < STEPMAX; k++)
		{
			genLibs[j+1][k] = genLibs[0][k];
			genLibs[j + 2][k] = genLibs[1][k];
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
			genLibs[j+1][p] = change;

			p = rand() % STEPMAX;
			old = genLibs[j + 2][p];
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
		score[i] = runStrategy(genLibs[i]);
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
	printf("A0 %d A1 %d\n\r", a0, b0);
}
void Robi::revlution(int nTimes)
{
	initGenLibs();
	randomGen(genLibs[0]);
	randomGen(genLibs[1]);
	for (int i = 0;i < nTimes;i++)
	{
		init();
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

void Robi::sortAndSelect()
{
	int score[GenLibPool];
	int p[GenLibPool];
	//计算所有方法的得分
	for (int i = 0; i < GenLibPool; i++)
	{
		score[i] = runStrategy(genLibs[i]);
		p[i] = score[i];
		//printf("%3d:[%5d] ",i, score[i]);
	}
	//printf("\n\r\n\rorder\n\r");
	//在P中排序
	for (int i = 0; i < GenLibPool; i++)
	{
		int index = i;
		int s = p[i];
		for (int j = i + 1; j < GenLibPool; j++)
		{
			if (s < p[j])
			{
				index = j;
				s = p[j];
			}
		}
		if (index != i)
		{
			int tmp = p[i];
			p[i] = s;
			p[index] = tmp;
		}
		//printf("%3d:[%5d] ",i, p[i]);
	}
	//printf("\n\r\n\rSelect\n\r");
	//在p中形成选择顺序队列
	for (int i = 0; i < GenLibPool; i++)
	{
		for (int j = 0; j < GenLibPool; j++)
		{
			if (p[i] == score[j])
			{
				p[i] = j;
				score[j] = -9999;
				break;
			}
		}
		//printf("%3d:[%5d] ",i,p[i]);
	}
	int x = ((rand() % 10000) / 10000.0) * (rand() % GenLibPool);
	int y = ((rand() % 10000) / 10000.0) * (rand() % GenLibPool);
	while (x == y)
	{
		y = ((rand() % 10000) / 10000.0) * (rand() % GenLibPool);
	}
	exchangeGenLibItem(0, p[x]);
	exchangeGenLibItem(1, p[y]);
}
void Robi::exchangeGenLibItem(int to, int from)
{
	if ((to == from) || (to < 0) || (to >= GenLibPool) || (from < 0) || (from >= GenLibPool))
		return;
	for (int i = 0; i < STEPMAX; i++)
	{
		int tmp = genLibs[to][i];
		genLibs[to][i] = genLibs[from][i];
		genLibs[from][i] = tmp;
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
		init();
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
		printf("\n\rcan not open file %s\n\r", DATAFILE);
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
void Robi::test1(int nTimes)
{
	char myGen[STEPMAX+100];
	genWonderGen(myGen);
	if (getData() == 0)
	{
		initGenLibs();
	}
	for (int i = 0; i < STEPMAX; i++)
	{
		genLibs[0][i] = myGen[i];
		genLibs[1][i] = myGen[i];
	}
	int total = 0;
	int maxx = 0;
	for(int j=0;j<nTimes;j++)
	{
		for (int i = 0; i < 1000; i++)
		{
			init();
			nextEra();
			//printGenLib();
			sortAndSelect();
			//actionRate(genLibs[0],4);
		}
		int a0 = run(genLibs[0]);
		int b0 = run(genLibs[1]);
		printf("%5d   %5d\n\r", a0,b0);
		total += a0;
		if (maxx < a0)
			maxx = a0;
		//printGen(genLibs[0]);
		//printGen(genLibs[1]);
		saveData();
	}
	printf("%d times tryed, average score:%d max %d\n\r", nTimes, total / nTimes,maxx);

}
void Robi::test2(int nTimes)
{
	//initGenLibs();
	if (getData() == 0)
	{
		randomGen(genLibs[0]);
		randomGen(genLibs[1]);
	}
	//astart();
	init();
	for (int n = 0; n < nTimes; n++)
	{
		for (int i = 0; i < 100; i++)
		{
			nextEra();
			sortAndSelect();
		}
		int a0 = runStrategy(genLibs[0]);
		int b0 = runStrategy(genLibs[1]);
		printf("a0 %d  b0 %d  ", a0, b0);
		printf("\n\r");
	}
	saveData();
}
void Robi::testG(int nTimes)
{
	init();
	char numGen[244];
	for (int i = 0; i < 244; i++)
		numGen[i] = G[i] - '0';
	for (int i = 0; i < nTimes; i++)
	{
		init();
		printf("\n\r%d", runStrategy(numGen));
	}
}
int Robi::getStateNo(int row,int col)
{
	char grid[4] = { 0,0,0,0 };//上下左右中
	if (row == 0)
	{
		grid[0] = 2;
	}
	else
	{
		grid[0] = matrix[row - 1][col];
	}
	if (row == 9)
	{
		grid[1] = 2;
	}
	else
	{
		grid[1] = matrix[row + 1][col];
	}
	if (col == 0)
	{
		grid[2] = 2;
	}
	else
	{
		grid[2] = matrix[row][col - 1];
	}
	if (col == 9)
	{
		grid[3] = 2;
	}
	else
	{
		grid[3] = matrix[row][col + 1];
	}
	int index = grid[0] * 3 * 3 * 3 * 3 + grid[1] * 3 * 3 * 3 + grid[2] * 3 * 3 + grid[3] * 3 + matrix[row][col];
	return index;
}
int Robi::runStrategy(char* gen)
{
	int score = 0;
	currentX = 0;
	currentY = 0;
	for (int i = 0; i < STEPMAX; i++)
	{
		char action = gen[getStateNo(currentY, currentX)];
		//printf("(%d，%d):%d ", currentY, currentX, action);
		score += goStep(action);
	}
	return score;
}
void Robi::test3()
{
	printf("%c", G[183]);
}
