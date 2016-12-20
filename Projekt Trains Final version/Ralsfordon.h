//-------------------------------------------------------------------
// Ralsfordon.h
// Base class for all vehicles
// David Hegardt 2016-10-28
//-------------------------------------------------------------------

#ifndef RalsfordonH
#define RalsfordonH

#include <iostream>
#include <vector>
#include <string>

using namespace std;
/* Ralsfordon - base class for all derived fordons classes

*/
class Ralsfordon {
public:
	Ralsfordon(int pId, string pTyp) : id(pId), typ(pTyp) {}

	virtual ~Ralsfordon() {}

	int getId() { return id; }
	string getTyp() { return typ; }

	void setLocation(string pLocation) { location = pLocation; }
	string getLocation() { return location; }

protected:
	int id;
	string typ;
	string location;
};










#endif // !RalsfordonH

