#pragma once
#define GenLibPool 243
#define RunStep 200
#define variationCount 10
class Robi2
{
	private:
		char M[244] = "65635365625235325265635365615135315125235325215135315165635365625235325265635365605035305025235325205035305015135315125235325215135315105035305025235325205035305065635356252353252656353656151353151252353252151353151656353656252353252656353454";
		char G[244] = "254355153256235251056355461151336154151034156110550150052030256256132252350325112052333054055231255051336154150665264150266506012264453605631520256431054354632404350334153250253251352352045150130156213436252353223135051260513356201524514343432";
		char genLibs[GenLibPool][GenLibPool];
		 
		int currentRow = 0;
		int currentCol = 0;

	public:
		char matrix[10][10];
		int score[GenLibPool];
		void init();
		double rand_zo();
		int goStep(int ch);
		void initGenLibs();
		void nextEra();
		void exchangeGenLibItem(int, int);
		void sortAndSelect();
		void variation();
		int getStateNo(int x, int y);
		void getBetter2();
		int runStrategy(char*);
		void printGen(char*);
		void test(int);
		void ga_sele();
		void ga_cross();
		void ga_muta();
		int get_score();
		void test2();
		void test3();
		int RVS();
		int roulette_select();
		void showStrategy(int index);
			
};

