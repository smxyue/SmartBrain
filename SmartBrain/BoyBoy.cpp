#include "BoyBoy.h"
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
void BoyBoy::test(int nCount)
{
    srand(time(0));
    int total = 0;
    int boy = 0;
    int girl = 0;
    int childen[2];
    for (int i = 0;i < nCount;i++)
    {
        for (int j = 0;j < 2;j++)
        {
            childen[j] = rand() % 2;
        }
        if (childen[0] == 0 || childen[1] == 0)
        {
            total++;
            if (childen[0] == childen[1])
                boy++;
            else
                girl++;

        }
    }
    printf("%d tried boy:%d rate:%1.2f  girl:%d rate:%1.2f\n\r", total, boy, (float)boy / total, girl, (float)girl / total);
}