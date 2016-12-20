//-------------------------------------------------------------------
// Godsvagn.h
// Derived classes for goodswagons
// David Hegardt 2016-10-28
//-------------------------------------------------------------------


#ifndef GodsvagnH
#define GodsvagnH

#include "Ralsfordon.h"

using namespace std;
/* Derived new baseclass from Ralsfordon class*/

class Godsvagn : public Ralsfordon {
public:
	Godsvagn(int pId, string pTyp) : Ralsfordon(pId,pTyp) {}

	virtual ~Godsvagn() {}
	
	virtual int getLoadCap() = 0;																					// Pure virtual, return load capacity

	virtual int getLastYta() = 0;																					// Pure virtual, return Last yta

	virtual int getLoadVol() = 0;																					// Pure virtual, return load volume

};

/*Derived class from Godsvagn */

class Opengodsvagn : public Godsvagn {
public:
	Opengodsvagn(int pId, string pTyp, int pLoadCap, int pLastYta) : 
		Godsvagn(pId,"Open Godsvagn"), loadCap(pLoadCap), lastYta(pLastYta) {}

	virtual ~Opengodsvagn() {}

	//virtual double getMaxSpeed() { return maxSpeed; }

	virtual int getLoadCap() { return loadCap; }																	// Return load capacity
	virtual int getLastYta() { return lastYta; }																	// Return last yta

	virtual int getLoadVol() { return 0; }																			// Not used for this class

protected:
	int loadCap;
	int lastYta;
};

/*Derived class from Godsvagn */

class Closedgodsvagn : public Godsvagn {
public:
	Closedgodsvagn(int pId, string pTyp, int pLoadVol) : Godsvagn(pId,"Closed Godsvagn"),
		loadVol(pLoadVol) {}

	virtual ~Closedgodsvagn() {}

	virtual int getLoadCap() { return 0; }																			// Not used for this class																	
	virtual int getLastYta() { return 0; }																			// Not used for this class

	virtual int getLoadVol() { return loadVol; }																	//return load volume

protected:
	int loadVol;
};










#endif // GodsvagnH

