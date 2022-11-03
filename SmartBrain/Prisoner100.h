#pragma once
class Prisoner100
{
	public:

		int box[100];
		int succed = 0;

		int tryOne();
		int findStep(int target);
		void checkLen();
		void printBox();
		void initBox();
		void printSeq();
		void checkfill();
		void checkdup();
		void test(int nCount);
};

