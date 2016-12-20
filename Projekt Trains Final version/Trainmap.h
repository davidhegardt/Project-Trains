//-------------------------------------------------------------------
// Trainmap.h
// Class for handeling trainmap file
// David Hegardt 2016-10-28
//-------------------------------------------------------------------

#ifndef TrainmapH
#define TrainmapH
#include "Tag.h"
#include <map>

using namespace std;

class Trainmap {
public:
	Trainmap() {};

	void createMap(string avg,string ank, int dist);
	/* Create map - create new multimap for storing inparameters
	   inparam : start station, destination station, distance between
	   pre : files read
	*/

	void printMap();
	/* Printmap - print information stored in multimap
	   pre : map created
	*/

	int findDistance(string avg,string ank);
	/* Find distance - based on multimap, enables search based on start station and end station
	   return distance
	   pre : map created
	*/

	double calcAvgSpeed(string avg, string ank, Time avgT, Time ankT);
	/* Calc average speed - calculate average speed based on time and start/destination
	
	*/


private:

	multimap<vector<string>,int> stationMap;
};




#endif // !TrainmapH
