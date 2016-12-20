//-------------------------------------------------------------------
// Station.h
// Station class
// David Hegardt 2016-10-28
//-------------------------------------------------------------------

#ifndef StationH
#define StationH
#include "Tag.h"

using namespace std;

class Station {
public:
	Station(string pStationname) : stationName(pStationname) {}

	~Station();

	list<shared_ptr<Ralsfordon>> getStationPool();
	/* Get Station pool
	   return list of shared pointers of vehicles
	*/

	void addToPool(shared_ptr<Ralsfordon> &newFordon);
	/* add to Pool
	   add vehicle to station pool
	*/

	bool checkAvail(string aTyp);
	/* Check Availible
	   check if current vehicle is availible in the station pool
	*/
	shared_ptr<Ralsfordon> getFordon(string aTyp);
	/* Get fordon
	   retrieve pointer to fordon from the station pool
	*/

	void addTrain(shared_ptr<Tag> aTrain);
	/* Add train to the vector of current trains
	   retrieve pointer to train 
	*/

	bool remTrain(int tagNr);
	/* Remove train
	   retrieve tågnummer and remove train from station pool
	*/

	void printInfo(int logglevel);
	/* Print info
	   print information about station
	*/

	string getStationName() { return stationName; }
	/* Get station name
	   return name of station
	*/

	void updatePool(list<shared_ptr<Ralsfordon>> newPool) { stationPool = newPool; }
private:
	string stationName;
	list<shared_ptr<Ralsfordon>> stationPool;
	list<shared_ptr<Tag>> trainList;
};


#endif // !StationH

