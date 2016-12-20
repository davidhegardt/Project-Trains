//-------------------------------------------------------------------
// Timer.h
// Class to convert simulation time and print correctly
// David Hegardt 2016-10-28
//-------------------------------------------------------------------

#ifndef timerH
#define timerH
#include <iostream>
#include <string>

using namespace std;

class Time {
private:
	int hours;
	int minutes;
public:
	Time();
	Time(int pHours, int pMinutes) : hours(pHours), minutes(pMinutes) {}

	//Setfunktioner
	void setHour(int pHours);
	void setMinutes(int pMinutes);
	

	//Getfunktioner
	int getHour() const { return hours; }
	int getMinutes() const { return minutes; }

	int toMinutes();
	/* Convert current Time object to minutes of int
	*/
	void fromMinutes(int minuter);
	/* Convert to Time objekt time from minutes in int
	*/

	//Overloading of operatorer
	Time &operator+(const Time &time);
	Time &operator-(const Time &time);
	bool operator<(const Time &time) const;
	bool operator==(const Time &time) const;

};

ostream &operator<<(ostream &os, const Time &time);


#endif