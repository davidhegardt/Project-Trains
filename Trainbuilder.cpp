//-------------------------------------------------------------------
// Trainbuilder.cpp
// Static class for creation of objects - implementation
// David Hegardt 2016-10-28
//-------------------------------------------------------------------

#include "Trainbuilder.h"


shared_ptr<Ralsfordon> Trainbuilder::createFordon(int pId, int pTyp, int pPar1, int pPar2) {
	shared_ptr<Ralsfordon> nyttFordon;																		// Skapa ny pekare för fordon

	if (pTyp == 0) {																						// Om typ = 0, skapa ett sittvagn objekt
		nyttFordon = make_shared<Sittvagn>(pId, "Sittvagn", pPar1, pPar2);									// Använd make shared för shared_ptr
		return nyttFordon;																					// Returnera pekare för fordonsobjekt
		
	}
	else if (pTyp == 1) {																					// Om typ = 1, skapa ett sovvagn objekt
		nyttFordon = make_shared<Sovvagn>(pId, "Sovvagn", pPar1);
		return nyttFordon;
	}
	else if (pTyp == 2) {
		nyttFordon = make_shared<Opengodsvagn>(pId, "Open Godsvagn", pPar1, pPar2);							// Om typ = 2, skapa ett öppen godsvagn objekt
		return nyttFordon;
	}
	else if (pTyp == 3) {
		nyttFordon = make_shared<Closedgodsvagn>(pId, "Closed Godsvagn", pPar1);							// Om typ = 3, skapa ett stängd godsvagn objekt
		return nyttFordon;
	}
	else if (pTyp == 4) {
		nyttFordon = make_shared<Ellok>(pId, "El-Lok", pPar1, pPar2);										// Om typ = 4, skapa ett ellok objekt
		return nyttFordon;
	}
	else if (pTyp == 5) {
		nyttFordon = make_shared<Diesellok>(pId, "Diesel-lok", pPar1, pPar2);								// Om typ = 5, skapa ett diesel lok objekt
		return nyttFordon;
	}
	//cout << "Nytt fordon skapat";
	
}

void Trainbuilder::printASCII(string typ) {
	if (typ == "Diesel-lok") {
		cout << "  Oo  ____    " << endl;
		cout << " _\\/__|_|_  " << endl;
		cout << "(=DIESEL_|-- " << endl;
		cout << "//o--=OOO- " << endl;
	}
	if (typ == "El-lok") {
		cout << "     ____    " << endl;
		cout << " __/__|_|_  " << endl;
		cout << "(=ELLOK_|-- " << endl;
		cout << "//o--=OOO- " << endl;
	}
	if (typ == "Open Godsvagn") {
		cout << " _,o+-o,_" << endl;
		cout << "|_______|--" << endl;
		cout << "`O`   `O`" << endl;
	}
	if (typ == "Closed Godsvagn") {
		cout << " _______" << endl;
		cout << "|_______|--" << endl;
		cout << "`O`   `O`" << endl;
	}
	if (typ == "Sittvagn") {
		cout << "_-------_" << endl;
		cout << "||_| |_| |" << endl;
		cout << "|________|--" << endl;
		cout << "`O`   `O`" << endl;
	}
	if (typ == "Sovvagn") {
		cout << "__________" << endl;
		cout << "| SOVVAGN |" << endl;
		cout << "|_________|--" << endl;
		cout << "`O`   `O`" << endl;
	}
}


void Trainbuilder::printFordonlist(list<shared_ptr<Ralsfordon>>currList, int logglevel) {
	//cout << "\n*** Ingående Fordon i tåg : " << getTagnummer() << " ***" << endl;	
	list<shared_ptr<Ralsfordon>>::iterator begin = currList.begin(), end = currList.end(), it;				// Iterator för lista av fordon
	for (it = begin; it != end; it++) {
		if ((*it)->getTyp() == "Sittvagn") {																// Om typ matchar
			cout << "id: " << (*it)->getId() << endl;														// Skriv ut allmäna parametrar
			cout << "typ: " << (*it)->getTyp() << endl;
			if (logglevel == 2) {
				cout << "Nuvaranande plats: " << (*it)->getLocation() << endl;
				cout << "Antal platser: " << static_pointer_cast<Sittvagn>(*it)->getSeats() << endl;		// Använd static pointer cast för att casta om till deriverat klass objekt
				cout << "Internet: " << static_pointer_cast<Sittvagn>(*it)->getInternet() << endl;			// Används för att komma åt medlemsfunktioner till deriverad klass
				printASCII((*it)->getTyp());																// Skriv ut grafik
			}
			cout << "-----------------" << endl;
		}
		if ((*it)->getTyp() == "Sovvagn") {
			cout << "id: " << (*it)->getId() << endl;
			cout << "typ: " << (*it)->getTyp() << endl;
			if (logglevel == 2) {
				cout << "Nuvaranande plats: " << (*it)->getLocation() << endl;
				cout << "Antal sängar: " << static_pointer_cast<Sovvagn>(*it)->getBeds() << endl;
				printASCII((*it)->getTyp());
			}
			cout << "-----------------" << endl;
		}
		if ((*it)->getTyp() == "Open Godsvagn") {
			cout << "id: " << (*it)->getId() << endl;
			cout << "typ: " << (*it)->getTyp() << endl;
			if (logglevel == 2) {
				cout << "Nuvaranande plats: " << (*it)->getLocation() << endl;
				cout << "Lastkapacitet : " << static_pointer_cast<Opengodsvagn>(*it)->getLoadCap() << " ton" << endl;
				cout << "Lastyta : " << static_pointer_cast<Opengodsvagn>(*it)->getLastYta() << " kvm" << endl;
				printASCII((*it)->getTyp());
			}
			cout << "-----------------" << endl;

		}
		if ((*it)->getTyp() == "Closed Godsvagn") {
			cout << "id: " << (*it)->getId() << endl;
			cout << "typ: " << (*it)->getTyp() << endl;
			if (logglevel == 2) {
				cout << "Nuvaranande plats: " << (*it)->getLocation() << endl;
				cout << "Lastvolym : " << static_pointer_cast<Closedgodsvagn>(*it)->getLoadVol() << " m3" << endl;
				printASCII((*it)->getTyp());
			}
			cout << "-----------------" << endl;
		}
		if ((*it)->getTyp() == "El-lok") {
			cout << "id: " << (*it)->getId() << endl;
			cout << "typ: " << (*it)->getTyp() << endl;
			if (logglevel == 2) {
				cout << "Nuvaranande plats: " << (*it)->getLocation() << endl;
				cout << "Maxhastighet : " << static_pointer_cast<Ellok>(*it)->getMaxSpeed() << " km/h" << endl;
				cout << "Effekt : " << static_pointer_cast<Ellok>(*it)->getEffekt() << " kW" << endl;
				printASCII((*it)->getTyp());
			}
			cout << "-----------------" << endl;
		}
		if ((*it)->getTyp() == "Diesel-lok") {
			cout << "id: " << (*it)->getId() << endl;
			cout << "typ: " << (*it)->getTyp() << endl;
			if (logglevel == 2) {
				cout << "Nuvaranande plats: " << (*it)->getLocation() << endl;
				cout << "Maxhastighet : " << static_pointer_cast<Diesellok>(*it)->getMaxSpeed() << " km/h" << endl;
				cout << "Förbrukning : " << static_pointer_cast<Diesellok>(*it)->getForbrukning() << " liter/timme" << endl;
				printASCII((*it)->getTyp());
			}
			cout << "-----------------" << endl;


		}



	}
}


shared_ptr<Tag> Trainbuilder::createTag(int pTagnummer, string pUrsprung, string pDest, Time pUrsprungtid, Time pDesttid, int pMaxSpeed, Time pDelay, STATUS pTillstand, list<int> pLogiclist) {
	
	list<string> trainList;
	list<int>::iterator begin = pLogiclist.begin(), end = pLogiclist.end(), it;															//Skapa iterator utifrån tåglista
	for (it = begin; it != end; it++) {																									//Baserat på parameter, omvandla till rätt typ
		if (*it == 0) {
			trainList.push_back("Sittvagn");
		}
		if (*it == 1) {
			trainList.push_back("Sovvagn");
		}
		if (*it == 2) {
			trainList.push_back("Open Godsvagn");
		}
		if (*it == 3) {
			trainList.push_back("Closed Godsvagn");
		}
		if (*it == 4) {
			trainList.push_back("El-lok");
		}
		if (*it == 5) {
			trainList.push_back("Diesel-lok");
		}
	}

	shared_ptr<Tag> newTrain(new Tag(pTagnummer, pUrsprung, pDest, pUrsprungtid, pDesttid, pMaxSpeed, pDelay, pTillstand, trainList));		// Skapa nytt tåg objekt och shared_ptr till tåg objekt

	return newTrain;
}