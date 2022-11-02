#pragma once
#define TAR  16

class JingGame
{
	private:
		int checkDup(int index, int a, int b, int c);
	public:
		int count = 0;//满足的组合数
		int table[3][100];
		void test();
		void numUsed();
};

