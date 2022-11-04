#pragma once
#include <list>
#define GAMESCALE 5
#define GAMEBLOCKS GAMESCALE *GAMESCALE

class Tetravex
{
	public:
		int matrix[4 * GAMEBLOCKS * GAMEBLOCKS] = {};
		int p[GAMEBLOCKS * GAMEBLOCKS] = { -1 };
		int firstRoom = 0;
		char tryIndex = 0;
		char tryOrder[GAMEBLOCKS]; //用过的块号
		std::list<BackPoint> backList;
		//char matrix[36]={'2','8','5','9','6','3','8','2','2','8','1','1','4','4','8','6','4','9','8','4','1','8','8','9','1','1','8','2','9','9','3','4','0','8','7','1'};;
		//char matrix[36]=  {'3','4','2','1','3','2','4','1','5','4','6','7','1','2','2','3','1','2','3','4','7','3','8','6','3','1','4','0','4','4','5','9','6','5','8','8'};

		int jiecheng(int n);
		void inputMatrix();
		void printMatrix();
		void ascMatrix();
		int neighborCloset(int closet);
		void makeGame();
		void choasMatrix();
		bool checkItem(int* pp, int grid, int index);
		bool preCheck(int* pp, int len);
		bool checkp();
		void qpl(int* chs, int* p, int size);
		void qpl_old(int* chs, int* p, int size);
		void CloneChs(int* old, int* clone, int index, int size);
		void CloneP(int* p, int* newp);
		bool tryBlock(char room, char block);
		char tryEmpty();
		void initTryOrder();
		void printTryOrder();
		bool isUsed(char block);
		void printBackList();
		void printP(bool block = false);
		void test();
};
struct BackPoint
{
	char room; //位置
	char dir;
	char neighbor; //适配块
};

