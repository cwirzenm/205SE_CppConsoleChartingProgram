#include "Candlestick.h"

Data::Data
(int date, double open, double high, double low, double close, double volume, double marketCap) :
	date(date), open(open), high(high), low(low), close(close), volume(volume), marketCap(marketCap) {
}

Data::~Data() {
}

double Data::readDate() {
	return date;
}

double Data::readOpen() {
	return open;
}

double Data::readHigh() {
	return high;
}

double Data::readLow() {
	return low;
}

double Data::readClose() {
	return close;
}

double Data::readVolume() {
	return volume;
}