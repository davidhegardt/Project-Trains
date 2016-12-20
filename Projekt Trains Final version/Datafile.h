//-------------------------------------------------------------------
// Datafile.h
// Datafile class - for reading data from files
// David Hegardt 2016-10-28
//-------------------------------------------------------------------

#ifndef DatafileH
#define DatafileH
#include "Tag.h"
#include "Station.h"
#include "Trainmap.h"

using namespace std;

class Datafile {
	
public:
	Datafile(string aTrainmap,string aTrainstation,string aTrains) : trainmapFile(aTrainmap),
		trainstationFile(aTrainstation), trainsFile(aTrains) {}

	void readTrainMap();
	/* Function to read train map file

	*/

	void readTrainStation();
	/* Function to read train station file

	*/

	void readTrains();
	/* Function to read Trains file */

	void createStation();
	/* Function to create stations
	   
	*/

	void addToStation(shared_ptr<Ralsfordon> &fordon);
	/* Function to add vehicle to station
	   pre : files read, vehicle created
	*/
	void printStatInfo();
	/* Function to print info about station
	   pre : station created
	*/
	void printTrainInfo();
	/* Function to print info about station
	pre : trains created
	*/

	void setStationName(string pName) { stationName = pName; };
	/* Set current station name while reading trainsfile*/

	string getStationName() { return stationName; };
	/* Get current station name while adding vehicle to station*/

	vector<shared_ptr<Station>> getStationVec() { return statVec; }
	/*Get vector of created station objects*/

	vector<shared_ptr<Tag>> getTagVec() { return tagVec; }
	/*Get vector of created train objects*/

	Trainmap getTrainMap() { return theMap; }
	/*Retrieve trainmap of locations and distance*/

private:
	string trainmapFile;
	string trainstationFile;
	string trainsFile;
	string stationName;
	vector<shared_ptr<Station>> statVec;
	vector<shared_ptr<Tag>> tagVec;
	Trainmap theMap;
};

/*Overloeaded operator to read vehicles */
std::ifstream &operator >> (std::ifstream & is, shared_ptr<Ralsfordon> &v);							

/*Overloaded operator to read trains*/
std::ifstream &operator >> (std::ifstream & is, shared_ptr<Tag> &v);

#endif // !DatafileH


