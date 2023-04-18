#include "Test.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
int** Test::getit()
{
	static int** myarr = (int**)malloc(10 * sizeof(int*));
	for (int i = 0;i <= 9;i++)
	{
		myarr[i] = (int*)malloc(10 * sizeof(int));
		for (int j = 0;j <= 9;j++)
		{
			myarr[i][j] = i * j;
		}
	}
	return myarr;
}
void Test::showit(int** arr)
{
	for (int i = 0;i <= 9;i++)
	{
		for (int j = 0;j <= 9;j++)
		{
			printf("%d\t",arr[i][j]);
		}
		printf("\n\r");
	}
}
void Test::main()
{
	srand(time(NULL));
	int** arr = getit();
	showit(arr);
}