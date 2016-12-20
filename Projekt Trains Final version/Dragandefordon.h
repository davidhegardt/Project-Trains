//-------------------------------------------------------------------
// Dragandefordon.h
// Derived classes for locomotives
// David Hegardt 2016-10-28
//-------------------------------------------------------------------

#ifndef DragandefordonH
#define DragandefordonH

#include "Ralsfordon.h"
#include <string>

using namespace std;
/* Derived new baseclass from Ralsfordon class*/

class Dragandefordon : public Ralsfordon {
public:
	Dragandefordon(int pId, string pTyp, double pMaxSpeed) :
		Ralsfordon(pId,pTyp), maxSpeed(pMaxSpeed) {}

	virtual ~Dragandefordon() {}

	virtual double getMaxSpeed() = 0;														// Pure virtual, Maxspeed for locomotive

	virtual double getEffekt() = 0;															// Pure virtual, effect for locomotive

	virtual double getForbrukning() = 0;													// Pure virtual, energy consumption

protected:
	double maxSpeed;
};

/*Derived class from Dragandefordon */

class Ellok : public Dragandefordon {
public:
	Ellok(int pId, string pTyp, double pMaxSpeed, double pEffekt) :
		Dragandefordon(pId,"El-lok",pMaxSpeed), effekt(pEffekt) {}

	virtual ~Ellok() {}

	virtual double getMaxSpeed() { return maxSpeed; }										// Implement function and return maxspeed

	virtual double getEffekt() { return effekt; }											// Implement function and return effect

	virtual double getForbrukning() { return 0; }											// Not used for this class

protected:
	double effekt;
};

/*Derived class from Dragandefordon */

class Diesellok : public Dragandefordon {
public:
	Diesellok(int pId, string pTyp, double pMaxSpeed, double pForbrukning) :
		Dragandefordon(pId,"Diesel-lok",pMaxSpeed), forbrukning(pForbrukning) {}

	virtual ~Diesellok() {}

	virtual double getMaxSpeed() { return maxSpeed; }										// Implement and return maxspeed
	virtual double getEffekt() { return 0; }												// Not used for this class
	virtual double getForbrukning() { return forbrukning; }									// Implement and return effect

protected:
	double forbrukning;
};












#endif
