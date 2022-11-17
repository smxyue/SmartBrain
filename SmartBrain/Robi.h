#pragma once
#define STEPMAX 200
class Robi
{
private:
	char M[244] = "65635365625235325265635365615135315125235325215135315165635365625235325265635365605035305025235325205035305015135315125235325215135315105035305025235325205035305065635356252353252656353656151353151252353252151353151656353656252353252656353454";
	char G[244] = "254355153256235251056355461151336154151034156110550150052030256256132252350325112052333054055231255051336154150665264150266506012264453605631520256431054354632404350334153250253251352352045150130156213436252353223135051260513356201524514343432";
	int currentX = 0;
	int currentY = 0;
public:
	int jarCount = 0;
	char matrix[10][10];
	int score = 0;
	void goStep(int ch);
	void init();
	void printRobi();
	int runM();
	int runG();
	int myGo();
	void test(int nTime);
};

