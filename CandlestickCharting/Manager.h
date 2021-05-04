#pragma once
#include "Chart.h"

class Manager {
private: 

	Charting execute;

public:

	void initialize();
	void candlestick();
	void bargraph();
	void movingAverage();
	void relativeStrengthIndex();
	void finalize();
	void replaceCout(ostream& os);
};