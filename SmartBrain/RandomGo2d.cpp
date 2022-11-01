#include "RandomGo2d.h"


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
void RandomGo2d::gogo(int n)
{
	for (int i = 0;i < n;i++)
		go();
}
