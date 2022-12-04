#include "Robi2.h"
#include <stdlib.h>
#include <stdio.h>
#include <corecrt_memory.h>
#include <utility>


void Robi2::init()
{
	int jarCount = 0;
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
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
	currentRow = 0;
	currentCol = 0;
}
int Robi2::goStep(int ch)
{
	//�������й��Ӳ����ռ���10�֡��ռ����Ӷ���������û�У�-1�֡����ײ����ǽ���ᱻ��5�֣�������ԭ���ĸ��ӡ�
	//0=���ϣ�1=���£�2=����3=���ң�4=������5=��ʰ���ӣ�6=����ƶ�

	switch (ch)
	{
	case 0:
		if (currentRow > 0)
		{
			currentRow--;
		}
		else
		{
			return -5;
		}
		break;
	case 1:
		if (currentRow < 9)
		{
			currentRow++;
		}
		else
		{
			return -5;
		}
		break;
	case 2:
		if (currentCol > 0)
		{
			currentCol--;
		}
		else
		{
			return -5;
		}
		break;
	case 3:
		if (currentCol < 9)
		{
			currentCol++;
		}
		else
		{
			return  -5;
		}
		break;
	case 4:
		break;
	case 5:
		if (matrix[currentRow][currentCol] == 1)
		{
			matrix[currentRow][currentCol] = 0;
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
void Robi2::initGenLibs()
{
	for (int i = 0; i < GenLibPool; i++)
	{
		for (int j = 0; j < GenLibPool; j++)
			genLibs[i][j] = rand() % 7;
	}
}
void Robi2::nextEra()
{
	//printGen(genLibs[0]);
	//printGen(genLibs[1]);
	for (int j = 1; j < GenLibPool - 2; j += 2)
	{
		//˳�δӿ�ʼ����β������λ���
		for (int k = 0; k < j; k++)
		{
			genLibs[j + 1][k] = genLibs[0][k];
			genLibs[j + 2][k] = genLibs[1][k];
		}
		for (int k = j; k < GenLibPool; k++)
		{
			genLibs[j + 1][k] = genLibs[1][k];
			genLibs[j + 2][k] = genLibs[0][k];
		}
	}
	//����
	int muta = 0.005 * GenLibPool * GenLibPool;
	for (int i = 0; i < muta; i++)
	{
			genLibs[rand() %GenLibPool][rand() % GenLibPool] = rand() % 7;
	}
}
void Robi2::exchangeGenLibItem(int to, int from)
{
	if ((to == from) || (to < 0) || (to >= GenLibPool) || (from < 0) || (from >= GenLibPool))
		return;
	for (int i = 0; i < GenLibPool; i++)
	{
		int tmp = genLibs[to][i];
		genLibs[to][i] = genLibs[from][i];
		genLibs[from][i] = tmp;
	}
}
void Robi2::sortAndSelect()
{
	int score[GenLibPool];
	int p[GenLibPool];
	//�������з����ĵ÷�
	for (int i = 0; i < GenLibPool; i++)
	{
		score[i] = runStrategy(genLibs[i]);
		p[i] = score[i];
		//printf("%3d:[%5d] ",i, score[i]);
	}
	//printf("\n\r\n\rorder\n\r");
	//��P������
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
	//��p���γ�ѡ��˳�����
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
void Robi2::variation()
{
	for (int i = 2; i < GenLibPool; i++)
	{
		exchangeGenLibItem((i % 2 ? 0: 1), i);
		for (int i = 0; i < variationCount; i++)
		{
			int p = rand() % GenLibPool;
			int ch = rand() % 7;
			while (ch == genLibs[i][p])
			{
				ch = rand() % 7;
			}
		}
	}
}
void Robi2::getBetter2()
{
	int score[GenLibPool];
	int p[GenLibPool];
	//�������з����ĵ÷�
	for (int i = 0; i < GenLibPool; i++)
	{
		score[i] = runStrategy(genLibs[i]);
		p[i] = score[i];
		//printf("%3d:[%5d] ",i, score[i]);
	}
	//printf("\n\r\n\rorder\n\r");
	//��P������
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
	//��p���γ�ѡ��˳�����
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
int Robi2::getStateNo(int row, int col)
{
	char grid[4] = { 0,0,0,0 };//����������
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
int Robi2::runStrategy(char* gen)
{
	int score = 0;
	currentRow = 0;
	currentCol = 0;
	init();
	for (int i = 0; i < RunStep; i++)
	{
		char action = gen[getStateNo(currentRow, currentCol)];
		//printf("(%d��%d):%d ", currentY, currentX, action);
		score += goStep(action);
	}
	return score;
}
void Robi2::printGen(char* gen)
{
	printf("\n\r{");
	for (int i = 0; i < GenLibPool; i++)
	{
		printf("%d", gen[i]);
	}
	printf("}");
}
int Robi2::roulette_select()
{
	float pr[GenLibPool];
	get_score();
	int sum = 0;
	for (int i = 0; i < GenLibPool; i++)
	{
		sum += score[i];
	}
	for (int i = 0; i < GenLibPool; i++)
	{
		if (sum == 0)
		{
			pr[i] = 1.0 / GenLibPool;
		}
		else
			pr[i] = 1.0 * score[i] / sum;
		
	}
	for (int i = 1; i < GenLibPool; i++)
	{
		pr[i] = pr[i - 1] + pr[i];
	}
	float r0 = (rand() % 10000) / 10000.0;
	int a0=-1 , b0=-1;
	for (int i = 0; i < GenLibPool; i++)
	{
		if (r0 < pr[i])
		{
			return i;
		}
	}
	return 0;
}
double Robi2::rand_zo()
{
	return (rand() % 10000) / 10000.0;
}
void Robi2::ga_sele()
{
	char  new_popu[GenLibPool][GenLibPool];  //�����µ���Ⱥ������200������
	double  score_pr[GenLibPool];  //��score[200] ���ϵ���0,1���ı�������
	double cum_pr[GenLibPool];    //�����ۼ����ϵ� 0~1 ��ֱ����
	double score_sum = 0;

	for(int i = 0; i < GenLibPool; i++)
			score_sum += score[i];   //�����ۼƵ÷�
	if (score_sum <= 0)
	{
		for (int i = 0; i < GenLibPool; i++)
		{
			score[i] = 1;
		}
		score_sum = GenLibPool;
	}

	for (int i = 0; i < GenLibPool; i++)
		score_pr[i] = score[i] / score_sum;    //������ʣ����ϵ���0,1��������

	cum_pr[0] = score_pr[0];    //�ۼӱ��ʣ����ϵ�0~1��ֱ����

	for (int i = 1; i < GenLibPool; i++)
		cum_pr[i] = cum_pr[i - 1] + score_pr[i];

	//----           
	//���̶ĸ���ѡ��

	for (int i = 0; i < GenLibPool; i++)
	{
		double rand_0 = rand_zo();  //����һ����0,1���������
		for (int j = 0; j < GenLibPool - 1; j++)
		{
			if (rand_0 < cum_pr[0])
			{
				memcpy(new_popu[i], genLibs[0], GenLibPool);
				//������������cum_pr[0]�����䣬��old_popu[0]���屣�����µ�Ⱥ��

				break;
			}
			else
				if (cum_pr[j] <= rand_0 <= cum_pr[j + 1])
				{
					memcpy(new_popu[i], genLibs[j + 1], GenLibPool);

					//������������cum_pr[j] �� cum_pr[j+1] ֮������䣬��old_popu[j+1]���屣�����µ�Ⱥ��

					break;
				}
		}
	}

	for (int i = 0; i < GenLibPool; i++)
		memcpy(genLibs[i], new_popu[i], GenLibPool);   //����ѡ��ĸ������·Żش�����������

}

void Robi2::ga_cross()
{
	//ѡ����Ҫ����ĸ���
	int count = 0;
	int need_cr[GenLibPool];
	int i, j;
	int temp[GenLibPool];
	int cross_lno, cross_lno_1;
	int cross_rate = 0.85;
	for (i = 0; i < GenLibPool; i++)
		if (rand_zo() < cross_rate)
		{
			need_cr[count] = i;
			count++;
		}
	if (count % 2 != 0)
		count++;
	//�����ȡһ����Ϊ0�Ľ����
	int cr_point = 0;
	while (cr_point == 0)
		cr_point = (rand_zo() * GenLibPool);
	//���н���
	for (i = 0; i < count; i++)
	{
		cross_lno = need_cr[i];
		cross_lno_1 = need_cr[i + 1];
		for (j = cr_point; j < GenLibPool; j++)
		{
			temp[j] = genLibs[cross_lno][j];
			genLibs[cross_lno][j] = genLibs[cross_lno_1][j];
			genLibs[cross_lno_1][j] = temp[j];
		}
	}
}
void Robi2::ga_muta()
{
	int sum = GenLibPool * GenLibPool;
	int muta_rate = 0.005;
	int i, j, k;
	for (i = 0; i < sum; i++)
	{
		if (rand_zo() < muta_rate)
		{
			//��λ�˻������ڵĸ��壨Ⱦɫ�壩
			int chr_loc;
			chr_loc = i / GenLibPool;

			//�ٶ�λ�˻������ڵĸ��壨Ⱦɫ�壩�ϵĻ���λ
			int gen_loc;
			gen_loc = i % GenLibPool;

			//���б���
			genLibs[chr_loc][gen_loc] = (rand_zo() * 6);
		}
	}
}

int Robi2::get_score()
{
	int maxScore = -99999;
	int min = 0;
	for (int i = 0; i < GenLibPool; i++)
	{
		score[i] = runStrategy(genLibs[i]);
		if (min > score[i])
		{
			min = score[i];
		}
		if (maxScore < score[i])
		{
			maxScore = score[i];
		}		
	}
	if (min < 0)
	{
		min = -1 * min;
		for (int i = 0; i < GenLibPool; i++)
		{
			score[i] += min;
		}
	}
	return maxScore;
}
void Robi2::test(int nTimes)
{
	char newGen[GenLibPool][GenLibPool];
	initGenLibs();

	for (int i = 0; i < nTimes; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			ga_cross();
			for (int k = 0; k < GenLibPool; k++)
			{
				int s = roulette_select();
				for (int m = 0; m < GenLibPool; m++)
				{
					newGen[k][m] = genLibs[s][m];
				}
			}
		}
		for(int j=0;j<GenLibPool;j++)
			for (int k = 0; k < GenLibPool; k++)
			{
				genLibs[j][k] = newGen[j][k];
			}
		int a0 = runStrategy(genLibs[0]);
		printf("\n\r%d", a0);
	}
	printGen(genLibs[0]);
}
int Robi2::RVS()
{
	float r[100];
	float pr[100];
	float sum = 0;
	for (int i = 0; i < 100; i++)
	{
		r[i] = 100-i;
		sum += r[i];
	}
	if (sum == 0)
	{
		for (int i = 0; i < 100; i++)
			r[i] = 1;
		sum = 100;
	}
	for (int i = 0; i < 100; i++)
	{
		pr[i] = r[i] / sum;
	}
	for (int i = 1; i < 100; i++)
	{
		pr[i] = pr[i - 1] + pr[i];
	}
	float rr = (rand() % 10000) / 10000.0;
	for (int i = 0; i < 100; i++)
	{
		if (rr < pr[i])
			return i;
	}
	return 0;
}
void Robi2::test3()
{
	char genM[GenLibPool];
	char genG[GenLibPool];

	for (int i = 0; i < GenLibPool; i++)
	{
		genG[i] = G[i] - '0';
		genM[i] = M[i] - '0';
	}
	init();
	int sg = runStrategy(genG);
	int sm = runStrategy(genM);
	printf("\n\rG score:%d    M score:%d", sg, sm);
}
void Robi2::test2()
{

	//score[i]�ǲ���prop[i]�õ������֣����ճ�ʼ��������һ��[ -250,500 ]֮������֡�

	init();   //��ʼ�����������200������
	initGenLibs();
	printGen(genLibs[0]);
	printf("\n������...\n");
	for (int k = 0; k < 10; k++)
	{
		for (int i = 0; i < 100; i++)  //�Ŵ��㷨������1000��
		{
			get_score();//ʹ��Ⱥ��ÿ��������Ϊ���Դ�ɨһ�֣��õ����и��������score[200];

			ga_sele();   //�������ʹ�����̶��㷨����ʤ��̭���õ��µ�Ⱥ��

			ga_cross(); // ��������    

			ga_muta();  //���� 
		}

		//����1000���󣬽�����ϣ�������յ÷�

		int maxscore = get_score();

		printf("������ߵ÷�Ϊ��%d \n", maxscore);
	}
	printGen(genLibs[0]);
}
void Robi2::showStrategy(int index)
{
	int old = index;
	char grid[5] = { 0,0,0,0,0 };
	for (int i = 4; i >=0 ; i--)
	{

		grid[i] = index % 3;
		index = index / 3;
	}
	printf(" %d \n\r%d%d%d\n\r %d", grid[0], grid[2], grid[4], grid[3], grid[1]);
	printf("\n\r %c\n\r\n\r", G[old]);
}

