#pragma once
#define STEPMAX 200
#define GenLibPool 200
#define DATAFILE "genLibs.dat"
class Robi
{
private:
	char M[244] = "65635365625235325265635365615135315125235325215135315165635365625235325265635365605035305025235325205035305015135315125235325215135315105035305025235325205035305065635356252353252656353656151353151252353252151353151656353656252353252656353454";
	char G[244] = "254355153256235251056355461151336154151034156110550150052030256256132252350325112052333054055231255051336154150665264150266506012264453605631520256431054354632404350334153250253251352352045150130156213436252353223135051260513356201524514343432";
	char genLibs[GenLibPool][STEPMAX];
	int currentX = 0;
	int currentY = 0;
public:
	int jarCount = 0;
	char matrix[10][10];
	int goStep(int ch);
	int init();
	void printRobi();
	int runM();
	int runG();
	int run(char*);
	int runBatch(char*,int);
	int myGo();
	void test(int nTime);
	int genWonderGen(char* myGen);


	void initGenLibs();
	void astart();
	int actionRate(char* gen, int ch);
	void genActionRate(char* );
	void randomGen(char* gen);
	void revlution(int nTimes);
	void newGeneration();
	void nextEra();
	void getBetter();
	void printGenLibItem(int p);
	void printGenLib();
	void check();
	void test1();
	void test2(int);

	void saveData();
	int getData();
	void printGen(char*);
	void changeMatrix(int);
};

