//-------------------------------------------------------------------
// Personvagn.h
// Derived classes for person carriages
// David Hegardt 2016-10-28
//-------------------------------------------------------------------

#ifndef PersonvagnH
#define PersonvagnH

#include "Ralsfordon.h"

using namespace std;
/* Derived new baseclass from Ralsfordon class*/

class Personvagn : public Ralsfordon {
public:
	Personvagn(int pId, string pTyp) :Ralsfordon(pId,pTyp) {}

	virtual ~Personvagn() {}

	virtual int getSeats() = 0;																				// Pure virtual, return number of seats

	virtual bool getInternet() = 0;																			// Pure virtual, return if internet

	virtual int getBeds() = 0;																				// Pure virtual, return number of beds
};

/*Derived class from Personvagn */

class Sittvagn : public Personvagn {
public:
	Sittvagn(int pId, string pTyp, int pSeats, bool pInternet) :Personvagn(pId,"Sittvagn"),seats(pSeats), internet(pInternet) {}

	
	virtual ~Sittvagn() {}

	virtual int getSeats() { return seats; }																// Get number of seats

	virtual bool getInternet() { return internet; }

	virtual int getBeds() { return 0; }																		// Not used for this class

protected:
	int seats;
	bool internet;
};

/*Derived class from Personvagn */

class Sovvagn : public Personvagn {
public:
	Sovvagn(int id, string ptyp, int pBeds) :Personvagn(id, "Sovvagn"), beds(pBeds) {}


	virtual ~Sovvagn() {}

	virtual int getSeats() { return 0; }																	// Not used for this class
	virtual bool getInternet() { return false; }

	int getBeds() { return beds; }
protected:
	int beds;
};






#endif // !PersonvagnH