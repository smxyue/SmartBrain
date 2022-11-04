#include "SafeBoxPwd.h"
#include <stdio.h>
int SafeBoxPwd::check()		//这个是检查当前填入的数字是否符合。
{
    for (int i = 0;i < 10;i++)
    {
        int count = 1;
        for (int j = 0;j < 10;j++)
        {
            if (s[j] == i)
                count++;
        }
        if (s[i] != count)
            return 0;
    }
    return 1;
}
void SafeBoxPwd::test()
{
    for (int a = 1;a < 10;a++)
        for (int b = 1;b < 10;b++)
            for (int c = 1;c < 10;c++)
                for (int d = 1;d < 10;d++)
                    for (int e = 1;e < 10;e++)
                        for (int f = 1;f < 10;f++)
                            for (int g = 1;g < 10;g++)
                                for (int h = 1;h < 10;h++)
                                    for (int i = 1;i < 10;i++)
                                        for (int j = 1;j < 10;j++)
                                        {
                                            s[0] = a;
                                            s[1] = b;
                                            s[2] = c;
                                            s[3] = d;
                                            s[4] = e;
                                            s[5] = f;
                                            s[6] = g;
                                            s[7] = h;
                                            s[8] = i;
                                            s[9] = j;//依次填入每个空
                                            if (check() == 1)//然后检验
                                            {
                                                printf("Got it:\n\r");
                                                for (int i = 0;i < 10;i++)
                                                {
                                                    printf("%5d", s[i]);
                                                }
                                            }
                                        }

}