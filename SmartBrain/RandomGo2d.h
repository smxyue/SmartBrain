#pragma once
#include <stdlib.h>
class RandomGo2d
{
	public:
		int homeTime = 0;
		int Setp = 1;
		int goCount = 0;
		int current = 0;
		void go();
		void test(int n);
};

