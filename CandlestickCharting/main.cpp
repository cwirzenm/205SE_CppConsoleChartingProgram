#include "Manager.h"

int main() {

	Manager manager;

	manager.initialize();
	manager.candlestick();
	manager.bargraph();
	manager.movingAverage();
	manager.relativeStrengthIndex();
	manager.finalize();

	bool userInput;
	cout << endl << endl << "Would you like to run a program again? [0/1]" << endl;
	cin >> userInput;
	cout << endl << endl;
	if (userInput == 1) {
		return main();
	}

	return 0;
}