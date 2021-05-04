#include "Chart.h"
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>
#include <cmath>


void Charting::readFile() {

	cout << endl << endl;
	cout << "Charting Program!" << endl;
	cout << "Author: Maksymilian Cwirzen" << endl;
	cout << "SID: 8907921" << endl << endl;

	ifstream inputFileStream;
	string FileName;
	cin >> FileName;
	inputFileStream.open(FileName);

	if (inputFileStream.good()) {
		cout << "Input file is good, start processing..." << endl << endl;

		string line;
		getline(inputFileStream, line);
		getline(inputFileStream, line);

		while (!inputFileStream.fail()) { // check for failbit
			stringstream ss(line);
			string date1;
			string date2;
			getline(ss, date1, ',');
			if (inputFileStream.fail()) {
				cout << "Error in input file." << endl;
				break;
			}
			getline(ss, date2, ',');
			if (inputFileStream.fail()) {
				cout << "Error in input file." << endl;
				break;
			}

			stringstream sss(date1);
			string stringday;
			int day;
			getline(sss, stringday, ' '); // extracts the day number, later used for X axis
			if (inputFileStream.fail()) {
				cout << "Error in input file." << endl;
				break;
			}
			sss >> day;

			vector<double> vdata; // vector that helps in collecting data needed to insert into one line of data vector
			for (int i = 0; i < 6; i++) { // loops through the remaining 6 columns
				string field;
				double fieldData;
				getline(ss, field, ',');
				if (inputFileStream.fail()) {
					cout << "Error in input file." << endl;
					break;
				}
				stringstream fss(field);
				fss >> fieldData;

				vdata.push_back(fieldData);

				if (i == 1 && fieldData > candleMax) {
					candleMax = fieldData;
				}
				if (i == 2 && fieldData < candleMin) {
					candleMin = fieldData;
				}
				if (i == 4 && fieldData > barMax) {
					barMax = fieldData;
				}
				if (i == 4 && fieldData < barMin) {
					barMin = fieldData;
				}
			}
			data.push_back(Data(day, vdata[0], vdata[1], vdata[2], vdata[3], vdata[4], vdata[5])); // insert all the values into a vector of objects

			getline(inputFileStream, line); // read next line
		}
		if (!inputFileStream.eof()) {
			cout << "Error in input file." << endl;
		}
	}

	else {
		cout << "Error opening input file, ";
		cout << "check 'BTChist.csv' exists in correct directory." << endl;
		exit(1);
	}

	inputFileStream.close();
}


bool Charting::candleIsBullish(double close, double open) {
	return open < close;
}


double Charting::candleFindFixedValue(double fixedValue) {

	for (int l = 0; l < candleYaxis.size(); l++) {
		if (abs(fixedValue - candleYaxis[l]) <= candleRange / 2) { // an equation on finding the closest Yaxis value
			fixedValue = candleYaxis[l];
			break;
		}
	}
	return fixedValue;
}


void Charting::candlePutFixedToChart(int i) { 

	if (i == 0) { // fixing values for high/low
		for (int columns = fixedValue.size() - 1; columns >= 0; columns--) {
			for (int rows = candleYaxis.size() - 1; rows >= 0; rows--) {
				if (fixedValue[columns] == candleYaxis[rows]) {
					candleChart[rows][columns] = char(179);
					break;
				}
			}
		}
	}
	if (i == 1) { // fixing values for open/close
		for (int columns = fixedValue.size() - 2; columns >= 0; columns -= 2) {
			for (int rows = candleYaxis.size() - 1; rows >= 0; rows--) {
				int x = columns + 1; // an int created in order to avoid arithmetical overflow warning
				if (fixedValue[x] == candleYaxis[rows]) {
					if (!(candleIsBullish(fixedValue[x], fixedValue[columns]))) { // bearish, goes down
						candleChart[rows][columns / 2] = char(178);
					}
					if (candleIsBullish(fixedValue[x], fixedValue[columns])) { // bullish, goes up
						candleChart[rows][columns / 2] = char(177);
					}
				}
				if (fixedValue[columns] == candleYaxis[rows]) {
					if (!(candleIsBullish(fixedValue[x], fixedValue[columns]))) { // bearish, goes down
						candleChart[rows][columns / 2] = char(178);
					}
					if (candleIsBullish(fixedValue[x], fixedValue[columns])) { // bullish, goes up
						candleChart[rows][columns / 2] = char(177);
					}
				}
			}
		}
	}
	fixedValue.clear();
}


void Charting::candleCreateAxes() {

	if (data.size() < 60) { // set the chart's size
		chartSizeDefault = data.size();
	}
	else {
		chartSizeDefault = 60;
	}
	candleChart.resize(chartSizeDefault);
	maChart.resize(chartSizeDefault);
	for (int i = 0; i < candleChart.size(); i++) {
		candleChart[i].resize(chartSizeDefault);
		maChart[i].resize(chartSizeDefault);
	}
	candleRange = (candleMax - candleMin) / (chartSizeDefault - 1);
	
	for (double i = candleMax; i >= candleMin; i -= candleRange) { // creating the Y axis
		int round = nearbyint(i);
		candleYaxis.push_back(round);
	}
	if (candleYaxis.size() < chartSizeDefault) { // bug fixing code on not including minimum value in the Y axis
		candleYaxis.push_back(nearbyint(candleMin));
	}
	else if (candleYaxis.size() > chartSizeDefault) {
		candleYaxis.pop_back();
	}
	
	dayspercolumn = ceil(data.size() / chartSizeDefault);
	for (int i = data.size() - 1; i >= 0; i -= 3 * dayspercolumn) { // creating the X axis
		dayXaxis.push_back(data[i].readDate());
	}
}


void Charting::candlePlot() {

	/*for (int i = 0; i < chartSizeDefault; i++) { // filling the chart with empty spaces
		for (int j = 0; j < chartSizeDefault; j++) {
			candleChart[i][j] = ' ';
			maChart[i][j] = ' ';
		}
	}*/ // activate this loop if there is an error in printing a chart


	for (int j = data.size() - 1; j >= 0; j -= dayspercolumn) { // fixing high values on the Y axis, values are ready to be printed out
		double highestValue = data[j].readHigh();
			for (int k = 1; k < dayspercolumn; k++) {
				int x = j - k; // an int created in order to avoid arithmetical overflow warning
				if (x >= 0 && data[x].readHigh() > highestValue) {
					highestValue = data[x].readHigh();
				}
		}
		highestValue = candleFindFixedValue(highestValue);
		fixedValue.push_back(highestValue);
	}
	candlePutFixedToChart(0);
	
	for (int j = data.size() - 1; j >= 0; j -= dayspercolumn) { // fixing low values on the Y axis, values are ready to be printed out
		double lowestValue = data[j].readLow();
			for (int k = 1; k < dayspercolumn; k++) {
				int x = j - k; // an int created in order to avoid arithmetical overflow warning
				if (x >= 0 && data[x].readLow() < lowestValue) {
					lowestValue = data[x].readLow();
				}
			}
		lowestValue = candleFindFixedValue(lowestValue);
		fixedValue.push_back(lowestValue);
	}
	candlePutFixedToChart(0);

	for (int j = data.size() - 1; j >= 0; j -= dayspercolumn) { // fixing open/close values on the Y axis, values are ready to be printed out
		double openValue = data[j].readOpen();
		openValue = candleFindFixedValue(openValue);
		fixedValue.push_back(openValue);

		int x = j + 1 - dayspercolumn;
		if (x >= 0) {
			double closeValue = data[x].readClose();
			closeValue = candleFindFixedValue(closeValue);
			fixedValue.push_back(closeValue);
		}
		else {
			double closeValue = data[0].readClose();
			closeValue = candleFindFixedValue(closeValue);
			fixedValue.push_back(closeValue);
		}
	}
	candlePutFixedToChart(1);


	for (int columns = 0; columns < chartSizeDefault; columns++) { // loop responsible for filling up the candlesticks
		char filler = ' ';
		int i = 0;
		for (int rows = 0; rows < chartSizeDefault; rows++) {
			if ((candleChart[rows][columns] == char(178) || candleChart[rows][columns] == char(177)) && filler == char(179)) {
				for (int j = 1; j < i; j++) {
					int x = rows - j; // an int created in order to avoid arithmetical overflow warning
					candleChart[x][columns] = char(179);
				}
				filler = ' ';
				i = 0;
			}
			if (candleChart[rows][columns] == char(178) && filler == char(178)) {
				for (int j = 1; j < i; j++) {
					int x = rows - j;
					candleChart[x][columns] = char(178);
				}
				filler = ' ';
				i = 0;
			}
			if (candleChart[rows][columns] == char(177) && filler == char(177)) {
				for (int j = 1; j < i; j++) {
					int x = rows - j;
					candleChart[x][columns] = char(177);
				}
				filler = ' ';
				i = 0;
			}
			if (candleChart[rows][columns] == char(179) && (filler == char(178) || filler == char(177))) {
				for (int j = 1; j < i; j++) {
					int x = rows - j;
					candleChart[x][columns] = char(179);
				}
				filler = ' ';
				i = 0;
			}
			if (candleChart[rows][columns] == char(179)) {
				filler = char(179);
			}
			if (candleChart[rows][columns] == char(178)) {
				filler = char(178);
			}
			if (candleChart[rows][columns] == char(177)) {
				filler = char(177);
			}
			if (filler != ' ') {
				i++;
			}
		}
	}

	int period = nearbyint(data.size() / 30);
	cout << endl << setw(45) << "Candlestick Chart showing the last " << period << " months' data:" << endl;
	cout << setw(12) << "- Price -" << endl;
	for (int rows = 0; rows < candleYaxis.size(); rows++) { // printing the Y axis and the chart
		cout << setw(10) << candleYaxis[rows] << " " << char(180);
		for (int columns = 0; columns < chartSizeDefault; columns++) {
			cout << candleChart[rows][columns];
		}
		cout << endl;
	}
	cout << "           " << char(192);
	for (int i = 0; i < dayXaxis.size(); i++) {
		cout << char(194) << char(196) << char(196);
	}
	cout << endl << "  - Days >";
	for (int i = 0; i < dayXaxis.size(); i++) { // printing the X axis
		cout << " " << setw(2) << dayXaxis[i];
	}
	cout << endl << endl << endl;
}


void Charting::barCreateYaxis() {

	barSize = 20;
	barChart.resize(barSize);
	for (int i = 0; i < barChart.size(); i++) {
		barChart[i].resize(chartSizeDefault);
	}

	if (barMin / 1000000000 >= 1) {
		magnitudeValue = 1000000000;
		magnitudeText = "Bil";
	}
	else if (barMin / 1000000 >= 1) {
		magnitudeValue = 1000000;
		magnitudeText = "Mil";
	}
	else {
		magnitudeValue = 1000;
		magnitudeText = "   ";
	}

	barRange = (barMax - barMin) / (barSize - 1);

	for (double i = barMax; i >= barMin; i -= barRange) { // creating the Y axis for a bargraph
		int round = i / magnitudeValue;
		barYaxis.push_back(round);
	}
	if (barYaxis.size() < barSize) { // bug fixing code on not including minimum value in the Y axis
		int round = barMin / magnitudeValue;
		barYaxis.push_back(round);
	}
	else if (barYaxis.size() > barSize) {
		barYaxis.pop_back();
	}
}


void Charting::barPlot() {

	/*for (int i = 0; i < barSize; i++) { // filling the chart with empty spaces
		for (int j = 0; j < chartSizeDefault; j++) {
			barChart[i][j] = ' ';
		}
	}*/ // activate this loop if there is an error in printing a chart

	vector<bool> BGisBullish;
	for (int i = data.size() - 1; i >= 0; i -= dayspercolumn) { // fixing volume values on the Y axis, values are ready to be printed out
		double meanValue = data[i].readVolume() / magnitudeValue;
		for (int j = 1; j < dayspercolumn; j++) {
			int x = i - j; // an int created in order to avoid arithmetical overflow warning
			if (x >= 0) {
				meanValue += data[x].readVolume() / magnitudeValue;
			}
		}
		meanValue /= dayspercolumn;
		for (int k = 0; k < barYaxis.size(); k++) {
			if (abs(meanValue - barYaxis[k]) <= ceil((barRange / magnitudeValue) / 2)) { // an equation on finding the closest Yaxis value (modified for low values)
				meanValue = barYaxis[k];
				break;
			}
		}
		int x = i - dayspercolumn;
		if (i - dayspercolumn >= 0) {
			BGisBullish.push_back(candleIsBullish(data[i].readClose(), data[x].readOpen()));
			fixedValue.push_back(meanValue);
		}
	}
	for (int columns = fixedValue.size() - 1; columns >= 0; columns--) { // marking all the needed places
		for (int rows = barYaxis.size() - 1; rows >= 0; rows--) {
			if (fixedValue[columns] == barYaxis[rows] && !BGisBullish[columns]) {
				barChart[rows][columns] = char(178);
			}
			if (fixedValue[columns] == barYaxis[rows] && BGisBullish[columns]) {
				barChart[rows][columns] = char(177);
			}
		}
	}
	fixedValue.clear();

	for (int rows = 1; rows < barChart.size(); rows++) { // filling up the bargraph
		int x = rows - 1; // an int created in order to avoid arithmetical overflow warning
		for (int columns = 0; columns < chartSizeDefault; columns++) {
			if (barChart[x][columns] == char(178)) {
				barChart[rows][columns] = char(178);
			}
			else if (barChart[x][columns] == char(177)) {
				barChart[rows][columns] = char(177);
			}
		}
	}

	cout << endl << setw(55) << "    Bargraph showing trading volume:    " << endl;
	cout << setw(12) << "- Price -" << endl;
	for (int rows = 0; rows < barYaxis.size(); rows++) { // printing the Y axis and the chart
		cout << setw(6) << barYaxis[rows] << " " << magnitudeText << " " << char(180);
		for (int columns = 0; columns < barChart[rows].size(); columns++) {
			cout << barChart[rows][columns];
		}
		cout << endl;
	}
	cout << "           " << char(192);
	for (int i = 0; i < dayXaxis.size(); i++) {
		cout << char(194) << char(196) << char(196);
	}
	cout << endl << "  - Days >";
	for (int i = 0; i < dayXaxis.size(); i++) { // printing the X axis
		cout << " " << setw(2) << dayXaxis[i];
	}
	cout << endl << endl << endl;
}


void Charting::maFindAverage() {

	for (int i = data.size() - 1; i >= 7; i -= dayspercolumn) { // for loop, looking for a past week average for every day, starting from day 8
		double average = 0;
		for (int j = 0; j < 8; j++) {
			int x = i - j; // an int created in order to avoid arithmetical overflow warning
			average += (data[x].readHigh() + data[x].readLow()) / 2;
		}
		average /= 7;
		if (weekAvg.size() < ceil(data.size() / dayspercolumn) - floor(7 / dayspercolumn) + 1) {
			weekAvg.push_back(average);
		}
	}
	if (weekAvg.size() > ceil(data.size() / dayspercolumn) - floor(7 / dayspercolumn) + 1) { // bug fixing
		weekAvg.pop_back();
	}

	for (int i = data.size() - 1; i >= 14; i -= dayspercolumn) { // for loop, looking for a past week average for every day, starting from day 15
		double average = 0;
		for (int j = 0; j < 15; j++) {
			int x = i - j;
			average += (data[x].readHigh() + data[x].readLow()) / 2;
		}
		average /= 14;
		if (twoWeekAvg.size() < ceil(data.size() / dayspercolumn) - floor(14 / dayspercolumn) + 1) {
			twoWeekAvg.push_back(average);
		}
	}
	if (twoWeekAvg.size() > ceil(data.size() / dayspercolumn) - floor(14 / dayspercolumn) + 1) { // bug fixing
		twoWeekAvg.pop_back();
	}
}


void Charting::maPlot() {

	for (int i = 0; i < weekAvg.size(); i++) { // fixing volume values on the Y axis, values are ready to be printed out
		int average = candleFindFixedValue(weekAvg[i]);
		fixedValue.push_back(average);
	}
	for (int columns = fixedValue.size() - 1; columns >= 0; columns--) {
		for (int rows = candleYaxis.size() - 1; rows >= 0; rows--) {
			int columnsum = columns + floor(7 / dayspercolumn); // shifts the chart by a week, also an int created in order to avoid arithmetical overflow warning
			if (fixedValue[columns] == candleYaxis[rows]) {
				maChart[rows][columnsum] = 'a';
				break;
			}
		}
	}
	fixedValue.clear();

	for (int i = 0; i < twoWeekAvg.size(); i++) { // fixing volume values on the Y axis, values are ready to be printed out
		int average = candleFindFixedValue(twoWeekAvg[i]);
		fixedValue.push_back(average);
	}
	for (int columns = fixedValue.size() - 1; columns >= 0; columns--) {
		for (int rows = candleYaxis.size() - 1; rows >= 0; rows--) {
			int columnsum = columns + floor(14 / dayspercolumn); // shifts the chart by two weeks, also an int created in order to avoid arithmetical overflow warning
			if (fixedValue[columns] == candleYaxis[rows]) {
				if (maChart[rows][columnsum] == 'a') {
					maChart[rows][columnsum] = '@';
					break;
				}
				else {
					maChart[rows][columnsum] = 'o';
					break;
				}
			}
		}
	}
	fixedValue.clear();


	cout << endl << setw(55) << "    Period-7 and period-14 MA chart:    " << endl;
	cout << setw(12) << "- Price -" << endl;
	for (int rows = 0; rows < candleYaxis.size(); rows++) { // printing the Y axis and the chart
		cout << setw(10) << candleYaxis[rows] << " " << char(180);
		for (int columns = 0; columns < chartSizeDefault; columns++) {
			cout << maChart[rows][columns];
		}
		cout << endl;
	}
	cout << "           " << char(192);
	for (int i = 0; i < dayXaxis.size(); i++) {
		cout << char(194) << char(196) << char(196);
	}
	cout << endl << "  - Days >";
	for (int i = 0; i < dayXaxis.size(); i++) { // printing the X axis
		cout << " " << setw(2) << dayXaxis[i];
	}
	cout << endl << endl << endl;
}


void Charting::rsiCalculate() {
	
	double readClose;
	double changeOfClose;
	double upwardMovm;
	double downwardMovm;
	double averageUpward = 0;
	double averageDownward = 0;
	double relativeStrength;

	readClose = data[data.size() - 1].readClose();
	for (int i = data.size() - 2; i >= data.size() - 15; i--) { // gathering values up to 14th day
		changeOfClose = data[i].readClose() - readClose;
		readClose = data[i].readClose();
		if (changeOfClose >= 0) {
			averageUpward += changeOfClose;
			averageDownward += 0;
		}
		else {
			averageUpward += 0;
			averageDownward += abs(changeOfClose);
		}
	}
	averageUpward /= 14;
	averageDownward /= 14;
	relativeStrength = averageUpward / averageDownward;
	relativeStrengthIndex.push_back(100 - (100 / (relativeStrength + 1)));
	for (int i = data.size() - 16; i >= 0; i--) { // finding RSI's starting from 15th day
		changeOfClose = data[i].readClose() - readClose;
		readClose = data[i].readClose();
		if (changeOfClose >= 0) {
			upwardMovm = changeOfClose;
			downwardMovm = 0;
		}
		else {
			upwardMovm = 0;
			downwardMovm = abs(changeOfClose);
		}
		averageUpward = (averageUpward * 13 + upwardMovm) / 14;
		averageDownward = (averageDownward * 13 + downwardMovm) / 14;
		relativeStrength = averageUpward / averageDownward;
		relativeStrengthIndex.push_back(100 - (100 / (relativeStrength + 1)));
	}
}


void Charting::rsiCreateYaxis() {

	rsiSize = 11;
	rsiChart.resize(rsiSize);
	for (int i = 0; i < rsiChart.size(); i++) {
		rsiChart[i].resize(chartSizeDefault);
	}

	for (int i = 100; i >= 0; i -= 10) {
		rsiYaxis.push_back(i);
	}
}


void Charting::rsiPlot() {

	/*for (int i = 0; i < rsiChart.size(); i++) { // filling the chart with empty spaces
		for (int j = 0; j < chartSizeDefault; j++) {
			rsiChart[i][j] = ' ';
		}
	}*/ // activate this loop if there is an error in printing a chart

	for (int i = 0; i < dayXaxis.size() * 3; i++) { // splitting the chart in 3 parts
		rsiChart[3][i] = '-';
		rsiChart[7][i] = '-';
	}

	for (int i = 0; i < relativeStrengthIndex.size(); i += dayspercolumn) {
		double average = relativeStrengthIndex[i];
		for (int j = 1; j < dayspercolumn; j++) {
			int x = i + j; // an int created in order to avoid arithmetical overflow warning
			if (x < relativeStrengthIndex.size()) {
				average += relativeStrengthIndex[x];
			}
		}
		average /= dayspercolumn;
		fixedValue.push_back(average);
	}

	for (int l = 0; l < fixedValue.size(); l++) { // fixing values on the Y axis
		for (int j = 100; j >= 0; j -= 10) {
			if (abs(fixedValue[l] - j) <= 5) { // an equation on finding the closest Yaxis value
				fixedValue[l] = j;
				break;
			}
		}
	}
	for (int columns = fixedValue.size() - 1; columns >= 0; columns--) { // putting values to a chart
		for (int rows = rsiYaxis.size() - 1; rows >= 0; rows--) {
			int columnsum = columns + floor(14 / dayspercolumn); // shifts the chart by a week, also an int created in order to avoid arithmetical overflow warning
			if (fixedValue[columns] == rsiYaxis[rows]) {
				rsiChart[rows][columnsum] = 'X';
				break;
			}
		}
	}
	fixedValue.clear();

	cout << endl << setw(55) << "Relative Strength Index indicator chart:" << endl;
	cout << setw(12) << "- Price -" << endl;
	for (int rows = 0; rows < rsiYaxis.size(); rows++) { // printing the Y axis and the chart
		cout << setw(10) << rsiYaxis[rows] << " " << char(180);
		for (int columns = 0; columns < chartSizeDefault; columns++) {
			cout << rsiChart[rows][columns];
		}
		cout << endl;
	}
	cout << "           " << char(192);
	for (int i = 0; i < dayXaxis.size(); i++) {
		cout << char(194) << char(196) << char(196);
	}
	cout << endl << "  - Days >";
	for (int i = 0; i < dayXaxis.size(); i++) { // printing the X axis
		cout << " " << setw(2) << dayXaxis[i];
	}
	cout << endl << endl << endl;
}


void Charting::saveToFile(ostream& os) {

	int period = nearbyint(data.size() / 30);
	os << endl << setw(45) << "Candlestick Chart showing the last " << period << " months' data:" << endl;
	os << setw(12) << "- Price -" << endl;
	for (int rows = 0; rows < candleYaxis.size(); rows++) {
		os << setw(10) << candleYaxis[rows] << " " << char(180);
		for (int columns = 0; columns < chartSizeDefault; columns++) {
			os << candleChart[rows][columns];
		}
		os << endl;
	}
	os << "           " << char(192);
	for (int i = 0; i < dayXaxis.size(); i++) {
		os << char(194) << char(196) << char(196);
	}
	os << endl << "  - Days >";
	for (int i = 0; i < dayXaxis.size(); i++) {
		os << " " << setw(2) << dayXaxis[i];
	}
	os << endl << endl << endl << endl << setw(55) << "    Bargraph showing trading volume:    " << endl;
	os << setw(12) << "- Price -" << endl;
	for (int rows = 0; rows < barYaxis.size(); rows++) {
		os << setw(6) << barYaxis[rows] << " " << magnitudeText << " " << char(180);
		for (int columns = 0; columns < barChart[rows].size(); columns++) {
			os << barChart[rows][columns];
		}
		os << endl;
	}
	os << "           " << char(192);
	for (int i = 0; i < dayXaxis.size(); i++) {
		os << char(194) << char(196) << char(196);
	}
	os << endl << "  - Days >";
	for (int i = 0; i < dayXaxis.size(); i++) {
		os << " " << setw(2) << dayXaxis[i];
	}
	os << endl << endl << endl << endl << setw(55) << "    Period-7 and period-14 MA chart:    " << endl;
	os << setw(12) << "- Price -" << endl;
	for (int rows = 0; rows < candleYaxis.size(); rows++) {
		os << setw(10) << candleYaxis[rows] << " " << char(180);
		for (int columns = 0; columns < chartSizeDefault; columns++) {
			os << maChart[rows][columns];
		}
		os << endl;
	}
	os << "           " << char(192);
	for (int i = 0; i < dayXaxis.size(); i++) {
		os << char(194) << char(196) << char(196);
	}
	os << endl << "  - Days >";
	for (int i = 0; i < dayXaxis.size(); i++) {
		os << " " << setw(2) << dayXaxis[i];
	}
	os << endl << endl << endl << endl << setw(55) << "Relative Strength Index indicator chart:" << endl;
	os << setw(12) << "- Price -" << endl;
	for (int rows = 0; rows < rsiYaxis.size(); rows++) {
		os << setw(10) << rsiYaxis[rows] << " " << char(180);
		for (int columns = 0; columns < chartSizeDefault; columns++) {
			os << rsiChart[rows][columns];
		}
		os << endl;
	}
	os << "           " << char(192);
	for (int i = 0; i < dayXaxis.size(); i++) {
		os << char(194) << char(196) << char(196);
	}
	os << endl << "  - Days >";
	for (int i = 0; i < dayXaxis.size(); i++) {
		os << " " << setw(2) << dayXaxis[i];
	}
	os << endl << endl << endl;

}
