//-------------------------------------------------------------------
// Timer.cpp
// Class to convert simulation time - implementation
// David Hegardt 2016-10-28
//-------------------------------------------------------------------

#include "Timer.h"

Time::Time() {
	hours = 0;
	minutes = 0;
}

void Time::setHour(int pHours) {

	hours = pHours;
}

void Time::setMinutes(int pMinutes) {

	minutes = pMinutes;
}

void Time::fromMinutes(int minuter) {

	hours = minuter / 60;
	minutes = minuter % 60;

}

Time &Time::operator+(const Time &time) {
	int sek = (minutes * 60) + (hours * 60 * 60);
	int sekTime = (time.minutes * 60) + (time.hours * 60 * 60);

	int diff = sek + sekTime;

	hours = diff / 3600;
	minutes = diff / 60;
	minutes %= 60;

	return *this;
}

Time &Time::operator-(const Time &time) {

	int sek = (minutes * 60) + (hours * 60 * 60);
	int sekTime = (time.minutes * 60) + (time.hours * 60 * 60);

	int diff = sek - sekTime;

	hours = diff / 3600;
	minutes = diff / 60;
	minutes %= 60;

	return *this;
}

int Time::toMinutes() {
	
	return (hours * 60 + minutes);
}

bool Time::operator==(const Time &time) const
{
	return hours == time.hours && minutes == time.minutes;
}

bool Time::operator<(const Time &time) const
{
	return (hours * 60 + minutes) < (time.hours * 60 + time.minutes);
}

// Överlagring av << operatorn för utskrift av Time-objekt
// ---------------------------------------------------------------------

ostream &operator<<(ostream &os, const Time &time)
{	
	if (time.getHour() >= 24) {
		os << (time.getHour() % 24) << ":";
	}
	else if (time.getHour() < 10) {
		os << "0" << (time.getHour()) << ":";
	}
	else if (time.getHour() >= 10) {
		os << (time.getHour()) << ":";
	}
	if (time.getMinutes() < 10) {
		os << "0" << (time.getMinutes());
	}
	else if (time.getMinutes() >= 10) {
		os << (time.getMinutes());
	}
	return os;

}