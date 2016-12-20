//-------------------------------------------------------------------
// Trainstatus.h
// Class for handeling trains - implementation
// David Hegardt 2016-10-28
//-------------------------------------------------------------------


#include "Trainstatus.h"
#include "Trainbuilder.h"
#include <algorithm>

Trainstatus::~Trainstatus() {



}

Trainstatus::Trainstatus() {
	
}

void Trainstatus::readInfo() {
	Datafile newData("TrainMap.txt", "TrainStations.txt", "Trains.txt");									// Anropa konstruktor och sätt filnamn
	newData.readTrainStation();																				// Läs in tågstationer
	newData.readTrains();																					// Läs in tåg
	newData.readTrainMap();																					// Läs in tågkarta
	newMap = newData.getTrainMap();																			// Hämta tågkarta

	statVec = newData.getStationVec();																		// Hämta vektor med stationer
	tagVec = newData.getTagVec();																			// Hämta vektor med tåg

	for (auto i : statVec) {																				// Loopa samtliga stationer innan hopsättning
		list<shared_ptr<Ralsfordon>> allFordon;
		allFordon = (i)->getStationPool();																	// Hämta stationspool
		list<shared_ptr<Ralsfordon>>::iterator begin = allFordon.begin(), end = allFordon.end(), it;					// Använd iterator och loopa fordonspool
		for (it = begin; it != end; it++) {
			makeHist(*it);																					// Anropa historik-funktion
		}
	}
}


bool Trainstatus::tryAssemble(shared_ptr<Tag> currTrain) {
	bool assembled = true;
	int tagId = currTrain->getTagnummer();														// Hämta tågnummer
	string avg = currTrain->getStart();															// Hämta avgångsstation
	shared_ptr<Ralsfordon> v;
	list<string> ingFordon = currTrain->getLocigList();											// Hämta lista på ingående fordon i tåg

	list<string>::iterator begin = ingFordon.begin(), end = ingFordon.end(), listit;			// Skapa iterator för ing fordon
	for (listit = begin; listit != end; listit++) {												// Loopa lista på ingående fordon
		for (auto i : statVec) {																// Loopa stationsvector
			if ((i)->getStationName() == avg) {													// Om station == avgångstation
				
				if ((i)->checkAvail(*listit)) {													// Kolla att ledigt fordon finns
					v = (i)->getFordon(*listit);												// Anropa funktion för att hämta fordon från station
					v->setLocation("Tåg " + to_string(currTrain->getTagnummer()));				// uppdatera location
					makeHist(v);
					currTrain->addFordon(v);													// Lägg till fordon till tåget
					currTrain->remList(*listit);
					(i)->addTrain(currTrain);													// Lägg till tåg till stationen
				}
				else {
																								// "Påkoppling misslyckades!"
					currTrain->setTillstand(INCOMPLETE);										// Om rätt fordon inte finns på stationen, ändra tillstånd
																								// ändra till const
					assembled = false;															// om påkoppling misslyckas, sätt till false
					
				}
			}
		}
	
	}
	
	
	return assembled;																			// returnera om lyckade eller ej
	
}


void Trainstatus::ready(shared_ptr<Tag> currTrain) {
	
	currTrain->setTillstand(READY);
}

void Trainstatus::running(shared_ptr<Tag> currTrain) {
	
	double speed = newMap.calcAvgSpeed(currTrain->getStart(), currTrain->getDest(), currTrain->getStartTime(), currTrain->getDestTime());
	
	currTrain->setMedelH(speed);
	currTrain->setTillstand(RUNNING);
}

void Trainstatus::arrived(shared_ptr<Tag> currTrain) {

	currTrain->setMedelH(0.0);
	currTrain->setTillstand(ARRIVED);
}

void Trainstatus::finishUp(shared_ptr<Tag> currTrain) {
	string dest = currTrain->getDest();																	// Ta fram destination för tåget
	
	for (auto i : statVec) {
		if ((i)->getStationName() == dest) {															// Kolla att stationsnamn = destination
			(i)->addTrain(currTrain);																	// Lägg till tåg till station
			list<shared_ptr<Ralsfordon>> fordonList;																// Skapa pekare till fordonslista
			fordonList = currTrain->getFordonList();													// Ta fram fordonslista för tåget

			list<shared_ptr<Ralsfordon>>::iterator begin = fordonList.begin(), end = fordonList.end(), listit;		// Skapa iterator för fordonslistan
			for (listit = begin; listit != end; listit++) {
				(*listit)->setLocation(currTrain->getDest());											// Ändra location för alla fordon
				makeHist(*listit);
				(i)->addToPool(*listit);																// Lägg till fordon till stationspool
			}
			currTrain->clearList();																		// Rensa ingående fordon
			currTrain->setTillstand(FINISHED);															// Ändra tillstånd
			
		}
	}
}


void Trainstatus::findFordon(int pFordonid, int pLoglvl) {
	int loglevel = pLoglvl;																												// Bestäm informationsnivå
	int fordonId = pFordonid;														
	bool found = false;																													// Om fordon hittat
	shared_ptr<Ralsfordon> foundFordon;																									// Skapa pekare för hittat fordon
	list<shared_ptr<Ralsfordon>> statList;																								// Skapa och loopa lista på stationer
	for (auto i : statVec) {

		statList = (i)->getStationPool();																								// För varje station, hämta fordonspool
		auto it = find_if(statList.begin(), statList.end(), [=](shared_ptr<Ralsfordon> m) {return m->getId() == fordonId;});			// Leta upp fordonsid
		if (it != statList.end()) {
			foundFordon = (*it);																										// Om fordon hittat, tilldela fordon
			found = true;																												// sätt hittat = true
		}
	}

	list<shared_ptr<Ralsfordon>> tagList;																								// Skapa och loopa lista på tag
	for (auto i : tagVec) {																												// Loopa igenom lista med tåg

		tagList = (i)->getFordonList();
		auto it = find_if(tagList.begin(), tagList.end(), [=](shared_ptr<Ralsfordon> m) {return m->getId() == fordonId;});				// om fordon hittat, tilldela fordon till pekare
		if (it != tagList.end()) {
			foundFordon = (*it);
			found = true;
		}
	}
	
	if (found == true) {																												// Om fordon hittat, skriv ut information
		cout << "Fordon id: " << foundFordon->getId() << endl;
		cout << "Historik :";
		readHist(foundFordon->getId());																									// Läs map fil med historik över platser
		cout << "\nBefinner sig på : " << foundFordon->getLocation() << endl;
		list<shared_ptr<Ralsfordon>>tmpList;
		tmpList.push_back(foundFordon);
		Trainbuilder::printFordonlist(tmpList, loglevel);																				// Skicka till funktion för detaljerad utskrift
	}
	else if (found == false) {
		cout << fordonId << " Kan inte hittas!" << endl;
	}
}



void Trainstatus::makeHist(shared_ptr<Ralsfordon> currTrain) {
	vector<string> tempVec;																												// Skapa ny vektor för plats och typ
	tempVec.push_back(currTrain->getLocation());
	tempVec.push_back(currTrain->getTyp());
	histMap.insert(make_pair(currTrain->getId(),tempVec ));																				// Skapa multimap baserat på id
}


void Trainstatus::readHist(int id) {

	typedef multimap<int, vector<string>>::const_iterator mapIT;																		// Create multimap iterator
	pair<mapIT, mapIT> range = histMap.equal_range(id);																					// find information based on id

	for (auto it = range.first; it != range.second; ++it) {																				// Print map info in vector
		cout << it->second.at(0) << " -> ";																								// second.at(0) is vector[0]
	}
}