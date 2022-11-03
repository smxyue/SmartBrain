#include "LetsMakeaDeal.h"
#include <stdlib.h>
#include <stdio.h>
void LetsMakeaDeal::initBox()
{

    for (int i = 0;i < 3;i++)
    {
        box[i] = 0;
    }
    box[rand() % 3] = 1;
}
void LetsMakeaDeal::printBox()
{
    for (int i = 0;i < 3;i++)
        printf("%d", box[i]);
    printf("\n\r");
}
void LetsMakeaDeal::guess()
{
    int s = rand() % 3;
    int open;
    for (int i = 0;i < 3;i++)
    {
        if (i != s && box[i] == 0)
            open = i;
    }
    int last;
    for (int i = 0;i < 3;i++)
    {
        if (i != s && i != open)
            last = i;
    }
    if (box[last] == 1)
        succed++;
    else
        failed++;
}
void LetsMakeaDeal::guess1()
{
    int s = rand() % 3;
    int open;
    for (int i = 0;i < 3;i++)
    {
        if (i != s && box[i] == 0)
            open = i;
    }
    int last;
    for (int i = 0;i < 3;i++)
    {
        if (i != s && i != open)
            last = i;
    }
    if (rand() % 2 == 1)
    {
        last = s;
    }
    if (box[last] == 1)
        succed++;
    else
        failed++;
}
void LetsMakeaDeal::guess2()
{
    int s = rand() % 3;
    int open;
    for (int i = 0;i < 3;i++)
    {
        if (i != s && box[i] == 0)
            open = i;
    }

    if (box[s] == 1)
        succed++;
    else
        failed++;
}
void LetsMakeaDeal::test(int nCount)
{
    failed = 0;
    succed = 0;
    for (int i = 0;i < nCount;i++)
    {
        initBox();
        //printBox();
        guess();
    }
    printf("%d times tried£º succed£º%d  failed:%d\n\r", nCount, succed, failed);
}