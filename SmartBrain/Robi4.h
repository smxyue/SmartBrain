#pragma once
#define POP_SIZE 200
#define GENE_SIZE 243
#define MAX_ITERATION 100
#define CROSSOVER_RATE 0.8
#define MUTATION_RATE 0.01

#include <vector>
using namespace std;
class Robi4
{
	char cells[10][10];
	int row;
	int col;

	std::vector<int>* stateSet();
	void printStateSet(std::vector<int>* set);
	int randAction(int index);
	char* deIndex(int);
	char* generate_random_gene();
	char** generate_initial_population();
	void initCells();
	int getStateNo(char[][10]);
	int goStep(char ch, char[][10]);
	void Normalize(int* score, double* nor);
	int evaluate_fitness(char* gene);
	void selection(char** population, int* fitness);
	void crossover(char* parent1, char* parent2);
	void mutation(char* gene);
	void printStrategy(int index, char action);
	void printPopulation(char**popluation);

public:
	void test();
	void testG();
	int main();
};

