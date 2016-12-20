//-------------------------------------------------------------------
// Datafile.cpp
// Datafile class - implementation
// David Hegardt 2016-10-28
//-------------------------------------------------------------------

#include "Datafile.h"
#include "Trainbuilder.h"
#include <string>
#include <fstream>
#include <iterator>
#include <iostream>
#include <algorithm>

using namespace std;

void Datafile::readTrainMap() {

	ifstream inFile(trainmapFile);																							// Skapa ifstream till fil

	if (!inFile.is_open()) {																								// Check if file is open
		throw runtime_error("Cannot open file");																			//Throw runtime error if operation fails
	}
	inFile.exceptions(ifstream::eofbit | ifstream::failbit);

	string avg;
	string ank;
	int dist;
	try {																													// Try för att läsa från fil
		while (inFile >> avg >> ank >> dist) {
			theMap.createMap(avg, ank, dist);																				// Skapa ny trainmap

		}
	}
	catch (ifstream::failure& e) {																							// när end of file är nodd, print message
		if (inFile.rdstate() & ifstream::eofbit) {
			cout << "EOF reached." << endl;
		}
	}
}

void Datafile::readTrainStation() {

	ifstream inFile(trainstationFile);																						// Skapa ifstream till fil
	if (!inFile.is_open()) {
		throw runtime_error("Cannot open file");																			//Throw runtime error om misslyckad
	}

	string stName1, stName2, stName3, stName4, stName5, stName6;															// Deklarera station names
	char par1 = '(';																										// Deklarera separators
	char par2 = ')';
	int id = 0;

	int typ = 0;
	int param0 = 0;
	int param1 = 0;
	shared_ptr<Ralsfordon> rals;																							// Skapa pekare till fordon
	string tmpString;
	vector<string> statName;
	inFile >> stName1;																										// Läs in stationsnamn
	setStationName(stName1);																								// Sätt tillfälligt stationsnamn
	createStation();																										// Anropa funktion för att skapa station
	while (inFile.peek() != '\n') {																							// Läs tills innan EOL
		inFile >> rals;																										// Anropa överlagring för att läsa in fordon
		addToStation(rals);																									// Skicka pekare till fordon till station
	}
	inFile >> stName1;																										// Upprepa ovan för alla stationer
	setStationName(stName1);
	createStation();
	while (inFile.peek() != '\n') {
		inFile >> rals;
		addToStation(rals);
	}
	inFile >> stName1;
	setStationName(stName1);
	createStation();
	while (inFile.peek() != '\n') {
		inFile >> rals;
		addToStation(rals);
	}
	inFile >> stName1;
	setStationName(stName1);
	createStation();
	while (inFile.peek() != '\n') {
		inFile >> rals;
		addToStation(rals);
	}
	inFile >> stName1;
	setStationName(stName1);
	createStation();
	while (inFile.peek() != '\n') {
		inFile >> rals;
		addToStation(rals);
	}
	inFile >> stName1;
	setStationName(stName1);
	createStation();
	while (inFile.peek() != '\n') {
		inFile >> rals;
		addToStation(rals);
	}
	inFile >> stName1;
	setStationName(stName1);
	createStation();
	while (inFile.peek() != '\n') {
		inFile >> rals;
		addToStation(rals);
	}
	inFile >> stName1;
	setStationName(stName1);
	createStation();
	for (int i = 0; i < 104; i++) {
		inFile >> rals;
		addToStation(rals);
	}




	inFile.close();
}

void Datafile::createStation() {
	string statName = getStationName();																		// Hämta stationsnamn
	
	statVec.push_back(shared_ptr<Station>(new Station(statName)));											// Skapa ny station samt pekare och lägg in i vector
}

void Datafile::readTrains() {

	ifstream inFile(trainsFile);																			// Skapa ifstream till fil

	if (!inFile.is_open()) {
		throw runtime_error("Cannot open file");															//Throw runtime error om misslyckad
	}
	inFile.exceptions(ifstream::eofbit | ifstream::failbit);
	shared_ptr<Tag> train;																					// Skapa pekare till tåg
	try {
		while (inFile) {																					// Försök att läsa in tåg
			inFile >> train;																				// Anropa överlagring för inläsning av tåg
			tagVec.push_back(train);																		// Lägg till tågvector när tåg skapat
		}
	}
	catch (ifstream::failure& e) {
		if (inFile.rdstate() & ifstream::eofbit) {
			cout << "EOF reached." << endl;
		}
	}

	inFile.close();
}

std::ifstream &operator >> (std::ifstream & is, shared_ptr<Tag> &v) {
	int id = 0;
	string avg;
	string ank;
	char sep = ':';
	int avgH = 0;
	int avgMin = 0;
	int ankH = 0;
	int ankMin = 0;
	int maxSpeed = 0;
	int vagn1 = 0, vagn2 = 0, vagn3 = 0, vagn4 = 0, vagn5 = 9;																							// Skapa variabler för inläsning av tågfil

	is >> id >> avg >> ank >> avgH >> sep >> avgMin >> ankH >> sep >> ankMin >> maxSpeed >> vagn1 >> vagn2 >> vagn3 >> vagn4;							// Läs in från fil enligt schema

	if (is.peek() != '\n') {																															// Kolla ifall nästa tecken är endline, kolla sedan om ytterligare parameter finns
		is >> vagn5;																																	// Detta beror på att vissa tåg  har fler vagnar
	}

	is.get();
	Time avgTid, ankTid, delay;																															// Skapa time objekt, sätt delay till 0
	delay.setHour(0); delay.setMinutes(0);
	avgTid.setHour(avgH); avgTid.setMinutes(avgMin); ankTid.setHour(ankH); ankTid.setMinutes(ankMin);
	STATUS tillstand = NOT_ASSEMBLED;
	list<int> trainList; trainList.push_back(vagn1); trainList.push_back(vagn2); trainList.push_back(vagn3);trainList.push_back(vagn4);					// Skicka in vagnsinformation till tågets ingående lista
	if (vagn5 != 9) { trainList.push_back(vagn5); }																										// Om vagn5 inte finns, läs in dummy värde
	v = Trainbuilder::createTag(id, avg, ank, avgTid, ankTid, maxSpeed, delay, tillstand, trainList);													// Skicka till funktion för att bygga tåg
	
	return is;																																			// Returnera ifstream och referens till tågobjekt
}	

std::ifstream &operator >> (std::ifstream & is, shared_ptr<Ralsfordon> &v) {
	int id, type, par1, par2 = 0;																														// Skapa parametrar för Rälsfordonsobjekt
	while (is.peek() == ' ' || is.peek() == '(')																										// Kolla ifall nästa tecken är parantes eller mellanrum
		is.ignore();																																	// Kör loop, ignorera delimiter
	is >> id >> type >> par1;																															// Läs in enligt schema

	if (is.peek() != ')') {																																// Kolla ifall nästa tecken är slutparantes, annars läs in nästa param
		is >> par2;
	}

	is.get();
	v = Trainbuilder::createFordon(id, type, par1, par2);																								// Skicka objekt till funktion för att skapa fordon
	return is;
}

void Datafile::addToStation(shared_ptr<Ralsfordon> &fordon) {
	string whichStation = getStationName();																												// Hämta tillfälligt stationsnamn
	fordon->setLocation(whichStation);																													// Sätt plats för fordon
	vector<shared_ptr<Station>>::iterator begin = statVec.begin(), end = statVec.end(), it;																// Iterera stationsvector
	for (it = begin; it != end; it++) {
		if ((*it)->getStationName() == whichStation) {																									// Om stationsnamn matchar stationsnamn i vector
			(*it)->addToPool(fordon);																													// Lägg till fordon till stationspool

		}
	}
}

void Datafile::printStatInfo() {
	vector<shared_ptr<Station>>::iterator begin = statVec.begin(), end = statVec.end(), it;				
	for (it = begin; it != end; it++) {
		(*it)->printInfo(2);
	}
}

void Datafile::printTrainInfo() {
	vector<shared_ptr<Tag>>::iterator begin = tagVec.begin(), end = tagVec.end(), it;				
	for (it = begin; it != end; it++) {
		cout << (*it)->getTagnummer() << endl;
		cout << (*it)->getStart() << endl;
		cout << (*it)->getDest() << endl;
	}
}

