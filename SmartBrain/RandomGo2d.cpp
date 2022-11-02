#include "RandomGo2d.h"
#include <stdio.h>
#include <math.h>


void RandomGo2d::go()
{
	goCount++;
	if (rand() % 2 == 0)
		current++;
	else
		current--;
	if (current == 0)
	{
		homeTime++;
	}
}
void RandomGo2d::test(int GoTimes)
{
	printf("\n\rRandom go in 2d is loading...\n\r");
	for (int i = 0;i < GoTimes;i++)
		go();
	printf("trid:%d distance:%d, backHome:%d  expect:%f", GoTimes, current, homeTime, sqrt(GoTimes));
}
