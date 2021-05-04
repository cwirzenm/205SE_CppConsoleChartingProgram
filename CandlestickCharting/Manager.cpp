#include "Manager.h"

#include <fstream>
#include <sstream>

void Manager::initialize() {
	execute.readFile();
}

void Manager::candlestick() {
	execute.candleCreateAxes();
	execute.candlePlot();
}

void Manager::bargraph() {
	execute.barCreateYaxis();
	execute.barPlot();
}

void Manager::movingAverage() {
	execute.maFindAverage();
	execute.maPlot();
}

void Manager::relativeStrengthIndex() {
	execute.rsiCalculate();
	execute.rsiCreateYaxis();
	execute.rsiPlot();
}

void Manager::replaceCout(ostream& os) {}
void Manager::finalize() {
	replaceCout(cout);
	bool userInput;
	cout << "Would you like to save the output to a text file? [0/1]" << endl;
	cin >> userInput;
	cout << endl;

	if (userInput == 1) {
		string userInputFileName;
		cout << "How would you like to call your file?" << endl;
		cin >> userInputFileName;
		ofstream writeToFile;
		writeToFile.open(userInputFileName);
		execute.saveToFile(writeToFile);
		writeToFile.close();
	}
}
