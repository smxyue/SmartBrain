#include "Prisoner100.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//һ��ʵ�飬�ɹ�����1������0
int Prisoner100::tryOne()
{
    // printf("}\n\r");
    //ÿ���˳����ҵ��Լ��ĺ���
    for (int i = 0;i < 100;i++)
    {
        int last = i;//���Լ��ĺ��뿪ʼ
        int limit = 50;
        //printf("\n\r%d    ",i);
        while (limit > 0)
        {
            if (box[last] == i)
            {
                break;
            }
            else
            {
                limit--;
                last = box[last];
            }
            //printf("[%d]:%d ",last,box[last]);
        }
        if (box[last] != i)
            return 0;

    }
    return 1;
}
//�ҵ�һ��Ԫ�صĲ���
int Prisoner100::findStep(int target)
{
    int count = 0;
    int last = target;
    while (box[last] != target)
    {
        last = box[last];
        count++;
        if (count > 100)
        {
            printf("\n\r%d can not find!", target);
            break;
        }
    }
    return count;
}
//ͳ�Ƹ���Ԫ���ҵ��ĳ���
void Prisoner100::checkLen()
{
    int below = 0;
    int up = 0;
    int len[100];
    for (int i = 0;i < 100;i++)
        len[i] = 0;
    for (int i = 0;i < 100;i++)
    {
        int step = findStep(i);
        len[i] = step;
        if (step <= 50)
        {

            below++;
        }
        else
        {
            up++;
        }
    }
    printf("\n\rfind step table\n\r");
    for (int i = 0;i < 100;i++)
        printf("%d : %d\n\r", i, len[i]);
    printf("below:%d  up:%d", below, up);
}
void Prisoner100::printBox()
{
    //��ʾ����
    printf("\n\rTable\n\r{");
    for (int i = 0;i < 100;i++)
    {
        printf("%d ", box[i]);
    }
}
void Prisoner100::initBox()
{
    //���Ҷ���
    for (int i = 0;i < 100;i++)
    {
        box[i] = 0;
    }
    for (int i = 0;i < 100;i++)
    {
        int next = rand() % 100;
        for (int j = 0;j < i;j++)
        {
            if (box[j] == next)
            {
                next = rand() % 100;
                j = -1;
            }
        }
        box[i] = next;
    }
}
void Prisoner100::printSeq()
{
    printf("\n\r");
    for (int i = 0;i < 100;i++)
    {
        for (int j = 0;j < 100;j++)
        {
            if (box[j] == i)
            {
                printf("[%d]:%d ", j, i);
            }
        }
    }
}
//������ɵĶ����Ƿ�����©
void Prisoner100::checkfill()
{
    int missed = 0;
    for (int i = 0;i < 100;i++)
    {
        int found = 0;
        for (int j = 0;j < 100;j++)
        {
            if (box[j] == i)
            {
                found = 1;
                break;
            }
        }
        if (found == 0)
        {
            printf("\n\r%d not found!", i);
        }
    }
}
//������ɵĶ����Ƿ���Ԫ���ظ�
void Prisoner100::checkdup()
{
    int dup = 0;
    for (int i = 0;i < 100;i++)
    {
        int found = 0;
        dup = 0;
        for (int j = 0;j < 100;j++)
        {
            if (box[j] == i)
            {
                if (found == 1)
                {
                    dup++;
                    printf("\n\r %d found again %d", i, dup);
                }
                else
                {
                    found = 1;
                }

            }
        }
        if (found == 0)
        {
            printf("\n\r%d not found!", i);
        }
    }
}
void Prisoner100::test(int nCount)
{
    srand(time(0));
    succed = 0;
    for (int i = 0;i < nCount;i++)
    {
        initBox();
        //aset();
        //printBox();
        //checkLen();
        //printSeq();
        //checkdup();
          if (tryOne() ==1)
          {
               succed ++;
          }
    }
    printf("\n\r%d tried: succed:%d, rate: %1.2f", nCount, succed, (float)succed / nCount);
}