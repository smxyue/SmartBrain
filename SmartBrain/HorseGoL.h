#pragma once
#define N  8 // ���̴�С
#include <vector>

using namespace std;

class HorseGoL
{
	
	vector<pair<int, int>> moves = {{1, 2}, {2, 1}, {2, -1}, {1, -2}, {-1, -2}, {-2, -1}, {-2, 1}, {-1, 2}}; // ��ѡ���ƶ���ʽ
	bool visited[N][N]; // ��¼ÿ��λ���Ƿ񱻷��ʹ�

	bool isValid(int x, int y);
	bool randomWalk(int x, int y);
public:
	int main();
};

