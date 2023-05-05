#pragma once
#define N  8 // 棋盘大小
#include <vector>

using namespace std;

class HorseGoL
{
	
	vector<pair<int, int>> moves = {{1, 2}, {2, 1}, {2, -1}, {1, -2}, {-1, -2}, {-2, -1}, {-2, 1}, {-1, 2}}; // 可选的移动方式
	bool visited[N][N]; // 记录每个位置是否被访问过

	bool isValid(int x, int y);
	bool randomWalk(int x, int y);
public:
	int main();
};

