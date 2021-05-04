#pragma once
#include <iostream>

using namespace std;

class Data {
private:
	int date;
	double open;
	double high;
	double low;
	double close;
	double volume;
	double marketCap;

public:
	Data(int date, double open, double high, double low, double close, double volume, double marketCap);
	~Data();
	double readDate();
	double readOpen();
	double readHigh();
	double readLow();
	double readClose();
	double readVolume();
};