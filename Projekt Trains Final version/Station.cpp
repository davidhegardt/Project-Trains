//-------------------------------------------------------------------
// Station.cpp
// Station class - implementation
// David Hegardt 2016-10-28
//-------------------------------------------------------------------

#include "Station.h"
#include "Trainbuilder.h"
#include <functional>
#include <algorithm>

Station::~Station() {

}

list<shared_ptr<Ralsfordon>> Station::getStationPool() {

	return stationPool;
}

void Station::addToPool(shared_ptr<Ralsfordon> &newFordon) {

	stationPool.push_back(newFordon);
}

bool Station::checkAvail(string aTyp) {
	bool availible = false;
	list<shared_ptr<Ralsfordon>>::iterator begin = stationPool.begin(), end = stationPool.end(), it;						// Iterera genom pekare av fordons objekt
	for (it = begin; it != end; it++) {																						
		if ((*it)->getTyp() == aTyp) {																						// Om typ matchar s�tt till true
			availible = true;
		}
	}
	return availible;																										// Returnea om fordon finns eller inte
}

shared_ptr<Ralsfordon> Station::getFordon(string aTyp) {
	shared_ptr<Ralsfordon> reqFordon;																																	// Sortera fordon efter id
	stationPool.sort([](shared_ptr<Ralsfordon>  lhs, shared_ptr<Ralsfordon>  rhs) {return (lhs->getTyp() == rhs->getTyp()) && (lhs->getId() < rhs->getId());});
	auto it = find_if(stationPool.begin(), stationPool.end(), [=](shared_ptr<Ralsfordon> m) {return m->getTyp() == aTyp;});												// Om typ matchar
	if (it != stationPool.end()) {
		cout << (*it)->getTyp() << " nr " << (*it)->getId() << " p�kopplad" << endl;																					
		reqFordon = *it;																									// Tilldela fordonspekare till hittat fordon
		stationPool.erase(it);																								// Ta bort fordon fr�n stationspool
		return reqFordon;																									// Returnera fordon
	}
	else {
		cout << "Inget fordon av typen" << aTyp << " �r tillg�ngligt";
	}
}
	
void Station::addTrain(shared_ptr<Tag> aTrain) {
	trainList.push_back(aTrain);																							// L�gg till t�g till t�glista
	//sort(trainList.begin(), trainList.end());
	trainList.erase(unique(trainList.begin(), trainList.end()), trainList.end());											// Rensa ut unika t�g
}

bool Station::remTrain(int tagNr) {
	bool removed = false;
	auto it = find_if(trainList.begin(), trainList.end(), [=](shared_ptr<Tag> m) {return m->getTagnummer() == tagNr;});		// Hitta t�g baserat p� t�gnummer
	if (it != trainList.end()) {
		trainList.erase(it);																								// Ta bort t�g fr�n lista
		removed = true;																										// Returnera om lyckades ta bort fr�n station
	}
	
	return removed;
}

void Station::printInfo(int plogglevel) {
	int logglevel = plogglevel;
	cout << "T�g inne p� station " << getStationName() << endl;
	cout << "-----------------------------------------" << endl;
	if (trainList.empty()) {
		cout << "-Inget t�g finns p� stationen" << endl;
	}
	else {
		for (auto i : trainList) {
			cout << "T�gnummer " << i->getTagnummer() << endl;
			cout << "Tillst�nd " << statusStrings[i->getTillstand()] << endl;
			continue;
		}
		
	}
	system("pause");
	cout << "Fordon i stationspoolen : " << stationPool.size() << endl;
	if (stationPool.empty()) {
		cout << "Inga fordon i poolen" << endl;
	}
	else {
		
		
		Trainbuilder::printFordonlist(stationPool, logglevel);
		
	}
}


