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
		if ((*it)->getTyp() == aTyp) {																						// Om typ matchar sätt till true
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
		cout << (*it)->getTyp() << " nr " << (*it)->getId() << " påkopplad" << endl;																					
		reqFordon = *it;																									// Tilldela fordonspekare till hittat fordon
		stationPool.erase(it);																								// Ta bort fordon från stationspool
		return reqFordon;																									// Returnera fordon
	}
	else {
		cout << "Inget fordon av typen" << aTyp << " är tillgängligt";
	}
}
	
void Station::addTrain(shared_ptr<Tag> aTrain) {
	trainList.push_back(aTrain);																							// Lägg till tåg till tåglista
	//sort(trainList.begin(), trainList.end());
	trainList.erase(unique(trainList.begin(), trainList.end()), trainList.end());											// Rensa ut unika tåg
}

bool Station::remTrain(int tagNr) {
	bool removed = false;
	auto it = find_if(trainList.begin(), trainList.end(), [=](shared_ptr<Tag> m) {return m->getTagnummer() == tagNr;});		// Hitta tåg baserat på tågnummer
	if (it != trainList.end()) {
		trainList.erase(it);																								// Ta bort tåg från lista
		removed = true;																										// Returnera om lyckades ta bort från station
	}
	
	return removed;
}

void Station::printInfo(int plogglevel) {
	int logglevel = plogglevel;
	cout << "Tåg inne på station " << getStationName() << endl;
	cout << "-----------------------------------------" << endl;
	if (trainList.empty()) {
		cout << "-Inget tåg finns på stationen" << endl;
	}
	else {
		for (auto i : trainList) {
			cout << "Tågnummer " << i->getTagnummer() << endl;
			cout << "Tillstånd " << statusStrings[i->getTillstand()] << endl;
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


