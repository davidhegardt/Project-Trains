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
	Datafile newData("TrainMap.txt", "TrainStations.txt", "Trains.txt");									// Anropa konstruktor och s�tt filnamn
	newData.readTrainStation();																				// L�s in t�gstationer
	newData.readTrains();																					// L�s in t�g
	newData.readTrainMap();																					// L�s in t�gkarta
	newMap = newData.getTrainMap();																			// H�mta t�gkarta

	statVec = newData.getStationVec();																		// H�mta vektor med stationer
	tagVec = newData.getTagVec();																			// H�mta vektor med t�g

	for (auto i : statVec) {																				// Loopa samtliga stationer innan hops�ttning
		list<shared_ptr<Ralsfordon>> allFordon;
		allFordon = (i)->getStationPool();																	// H�mta stationspool
		list<shared_ptr<Ralsfordon>>::iterator begin = allFordon.begin(), end = allFordon.end(), it;					// Anv�nd iterator och loopa fordonspool
		for (it = begin; it != end; it++) {
			makeHist(*it);																					// Anropa historik-funktion
		}
	}
}


bool Trainstatus::tryAssemble(shared_ptr<Tag> currTrain) {
	bool assembled = true;
	int tagId = currTrain->getTagnummer();														// H�mta t�gnummer
	string avg = currTrain->getStart();															// H�mta avg�ngsstation
	shared_ptr<Ralsfordon> v;
	list<string> ingFordon = currTrain->getLocigList();											// H�mta lista p� ing�ende fordon i t�g

	list<string>::iterator begin = ingFordon.begin(), end = ingFordon.end(), listit;			// Skapa iterator f�r ing fordon
	for (listit = begin; listit != end; listit++) {												// Loopa lista p� ing�ende fordon
		for (auto i : statVec) {																// Loopa stationsvector
			if ((i)->getStationName() == avg) {													// Om station == avg�ngstation
				
				if ((i)->checkAvail(*listit)) {													// Kolla att ledigt fordon finns
					v = (i)->getFordon(*listit);												// Anropa funktion f�r att h�mta fordon fr�n station
					v->setLocation("T�g " + to_string(currTrain->getTagnummer()));				// uppdatera location
					makeHist(v);
					currTrain->addFordon(v);													// L�gg till fordon till t�get
					currTrain->remList(*listit);
					(i)->addTrain(currTrain);													// L�gg till t�g till stationen
				}
				else {
																								// "P�koppling misslyckades!"
					currTrain->setTillstand(INCOMPLETE);										// Om r�tt fordon inte finns p� stationen, �ndra tillst�nd
																								// �ndra till const
					assembled = false;															// om p�koppling misslyckas, s�tt till false
					
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
	string dest = currTrain->getDest();																	// Ta fram destination f�r t�get
	
	for (auto i : statVec) {
		if ((i)->getStationName() == dest) {															// Kolla att stationsnamn = destination
			(i)->addTrain(currTrain);																	// L�gg till t�g till station
			list<shared_ptr<Ralsfordon>> fordonList;																// Skapa pekare till fordonslista
			fordonList = currTrain->getFordonList();													// Ta fram fordonslista f�r t�get

			list<shared_ptr<Ralsfordon>>::iterator begin = fordonList.begin(), end = fordonList.end(), listit;		// Skapa iterator f�r fordonslistan
			for (listit = begin; listit != end; listit++) {
				(*listit)->setLocation(currTrain->getDest());											// �ndra location f�r alla fordon
				makeHist(*listit);
				(i)->addToPool(*listit);																// L�gg till fordon till stationspool
			}
			currTrain->clearList();																		// Rensa ing�ende fordon
			currTrain->setTillstand(FINISHED);															// �ndra tillst�nd
			
		}
	}
}


void Trainstatus::findFordon(int pFordonid, int pLoglvl) {
	int loglevel = pLoglvl;																												// Best�m informationsniv�
	int fordonId = pFordonid;														
	bool found = false;																													// Om fordon hittat
	shared_ptr<Ralsfordon> foundFordon;																									// Skapa pekare f�r hittat fordon
	list<shared_ptr<Ralsfordon>> statList;																								// Skapa och loopa lista p� stationer
	for (auto i : statVec) {

		statList = (i)->getStationPool();																								// F�r varje station, h�mta fordonspool
		auto it = find_if(statList.begin(), statList.end(), [=](shared_ptr<Ralsfordon> m) {return m->getId() == fordonId;});			// Leta upp fordonsid
		if (it != statList.end()) {
			foundFordon = (*it);																										// Om fordon hittat, tilldela fordon
			found = true;																												// s�tt hittat = true
		}
	}

	list<shared_ptr<Ralsfordon>> tagList;																								// Skapa och loopa lista p� tag
	for (auto i : tagVec) {																												// Loopa igenom lista med t�g

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
		readHist(foundFordon->getId());																									// L�s map fil med historik �ver platser
		cout << "\nBefinner sig p� : " << foundFordon->getLocation() << endl;
		list<shared_ptr<Ralsfordon>>tmpList;
		tmpList.push_back(foundFordon);
		Trainbuilder::printFordonlist(tmpList, loglevel);																				// Skicka till funktion f�r detaljerad utskrift
	}
	else if (found == false) {
		cout << fordonId << " Kan inte hittas!" << endl;
	}
}



void Trainstatus::makeHist(shared_ptr<Ralsfordon> currTrain) {
	vector<string> tempVec;																												// Skapa ny vektor f�r plats och typ
	tempVec.push_back(currTrain->getLocation());
	tempVec.push_back(currTrain->getTyp());
	histMap.insert(make_pair(currTrain->getId(),tempVec ));																				// Skapa multimap baserat p� id
}


void Trainstatus::readHist(int id) {

	typedef multimap<int, vector<string>>::const_iterator mapIT;																		// Create multimap iterator
	pair<mapIT, mapIT> range = histMap.equal_range(id);																					// find information based on id

	for (auto it = range.first; it != range.second; ++it) {																				// Print map info in vector
		cout << it->second.at(0) << " -> ";																								// second.at(0) is vector[0]
	}
}