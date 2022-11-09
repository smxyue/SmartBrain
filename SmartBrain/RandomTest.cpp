#include "RandomTest.h"
#include <stdlib.h>
#include <stdio.h>
void RandomTest::test(int nTimes)
{
	for (int i = 0;i < TESTSCALE;i++)
		no[i] = 0;
	for (int i = 0;i < nTimes;i++)
	{
		int val = rand()% TESTSCALE;
		no[val] ++;
	}
	printf("Test result table\n\r runTimes:%d\n\rexpect:%8.1f\n\r",nTimes, (float)nTimes / TESTSCALE);
	for (int i = 0;i < TESTSCALE;i++)
		printf("%8d:%8d %8.3f\n\r", i, no[i], (float)no[i] / nTimes);
}