#include "JingGame.h"
#include <stdio.h>

int JingGame::checkDup(int index, int a, int b, int c)
{
    for (int i = 0;i < index;i++)
    {
        if (table[0][i] == a || table[1][i] == a || table[2][i] == a)
        {
            if (table[0][i] == b || table[1][i] == b || table[2][i] == b)
            {
                if (table[0][i] == c || table[1][i] == c || table[2][i] == c)
                {
                    return 1;
                }
            }
        }
    }
    return 0;
}
void JingGame::test()
{
    for (int a = 1;a < 10;a++)
    {
        for (int b = 1;b < 10;b++)
        {
            table[0][count] = a;
            if (!(a == b))
            {
                table[1][count] = b;
                int c = TAR - table[0][count] - table[1][count];
                if ((c > 0) && (c < 10) && (c != table[0][count]) && (c != table[1][count]))
                {
                    if (checkDup(count, a, b, c) == 0)
                    {
                        table[2][count] = c;
                        count++;
                    }
                }

            }

        }
    }

    for (int i = 0;i < count;i++)
    {
        printf("%d (%d %d %d)\n\r", i + 1, table[0][i], table[1][i], table[2][i]);
    }
}
void JingGame::numUsed()
{
    int num[10] = { 0,0,0,0,0,0,0,0,0,0 };
    for (int i = 0;i < count;i++)
    {
        for (int j = 0;j < 3;j++)
        {
            int n = table[j][i];
            num[n] ++;
        }
    }
    for (int i = 1;i < 10;i++)
    {
        printf("%d:%d\n\r", i, num[i]);
    }
}