#include "Robi4.h"
#include <vector>

std::vector<int>* Robi4::stateSet()
{
	static std::vector<int>* indexSet = new std::vector<int>;
	indexSet->clear();
	for (int i = 0; i < 3; i++)
		for(int j=0;j<3;j++)
			for (int l = 0; l < 3; l++)
				for(int m=0;m<3;m++)
					for (int n = 0; n < 2; n++)
					{
						if ((i != 2 && j != 2) && (l != 2 && m != 2))
						{
							int index = i * 81 + j * 27 + l * 9 + m * 3 + n;
							indexSet->push_back(index);
						}
					}
	return indexSet;
}
void Robi4::printStateSet(std::vector<int>* set)
{
	if (set == NULL)
		return;
	printf("\n\r{");
	for (int val:*set)
	{
		printf(" %d ", val);
	}
	printf("}\n\r");
}

int Robi4::main()
{
	std::vector<int>* indexSet = stateSet();
	printStateSet(indexSet);
	return 0;
}