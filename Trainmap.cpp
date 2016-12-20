//-------------------------------------------------------------------
// Trainmap.cpp
// Class for handeling trainmap file - implementation
// David Hegardt 2016-10-28
//-------------------------------------------------------------------

#include "Trainmap.h"
#include <algorithm>

void Trainmap::createMap(string avg,string ank, int dist) {
	vector<string> tempVec;																							// Skapa temp vektor för avgång/destination
	tempVec.push_back(avg);
	tempVec.push_back(ank);
	stationMap.insert(make_pair(tempVec, dist));																	// Lägg till multimap med key = distans
		
}

void Trainmap::printMap() {
	for (multimap<vector<string>, int>::iterator it = stationMap.begin();
		it != stationMap.end(); ++it) {
		cout << it->first.at(0) << " - " << it->first.at(1) << " " << it->second << endl; 
	}
}

int Trainmap::findDistance(string ank, string avg) {
	for (multimap<vector<string>, int>::iterator it = stationMap.begin();											// Iterera genom multimap
		it != stationMap.end(); ++it) {
		if (it->first.at(0) == avg && it->first.at(1) == ank) { return it->second; }								// om vector[0] = start och vector[1] = destination, returnera nyckel
		else if (it->first.at(0) == ank && it->first.at(1) == avg) { return it->second; }							// om vector[1] = start och vector[0] = destination, returnera nyckel
	}
}

double Trainmap::calcAvgSpeed(string avg, string ank, Time avgT, Time ankT) {

	double distance = findDistance(avg, ank);																		// Anropa funktion för att ta reda på distans
	
	Time restid = ankT - avgT;																						// Räkna ut restid
	cout << "Restid calcspeed : " << restid << endl;
	double minut = restid.toMinutes();

	double avgSpeed = distance / minut * 60;																		// restid v = s/t 
	return avgSpeed;
}