#pragma once
#define POOLSIZE 100
//ÂÖÅÌ¶ÄÑ¡ÔñËã·¨Ä£Äâ
class RouletteSelect
{
	int pool[POOLSIZE];
	double fitness[POOLSIZE];
	int selected[POOLSIZE];
	void initPool(int scale,bool rand);
	void calc_fitness(int scale);
	void select(int scale);
public:
	int main(int scale);

};

