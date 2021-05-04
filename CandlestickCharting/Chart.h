#pragma once
#include "Candlestick.h"
#include <limits>
#include <vector>

class Charting {
private:

	vector<Data> data;
	double chartSizeDefault;
	int dayspercolumn;
	vector<double> dayXaxis;
	vector<double> fixedValue;

	double candleMin = numeric_limits<double>::max(); // setting max at lowest possible value
	double candleMax = numeric_limits<double>::min(); // setting min at highest possible value
	double candleRange;
	vector<double> candleYaxis;
	vector<vector<char>> candleChart;

	double barSize;
	double barMin = numeric_limits<double>::max();
	double barMax = numeric_limits<double>::min();
	double barRange;
	string magnitudeText;
	double magnitudeValue;
	vector<double> barYaxis;
	vector<vector<char>> barChart;

	vector<double> weekAvg;
	vector<double> twoWeekAvg;
	vector<vector<char>> maChart;

	int rsiSize;
	vector<int> rsiYaxis;
	vector<double> relativeStrengthIndex;
	vector<vector<char>> rsiChart;

public:

	void readFile();

	void candleCreateAxes();
	bool candleIsBullish(double open, double close);
	double candleFindFixedValue(double fixedValue);
	void candlePutFixedToChart(int i);
	void candlePlot();

	void barCreateYaxis();
	void barPlot();

	void maFindAverage();
	void maPlot();

	void rsiCalculate();
	void rsiCreateYaxis();
	void rsiPlot();

	void saveToFile(ostream& os);
};
