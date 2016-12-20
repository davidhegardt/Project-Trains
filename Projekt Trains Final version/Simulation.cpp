//-------------------------------------------------------------------
// Simulation.cpp
// Simulation class - implementation
// David Hegardt 2016-10-28
//-------------------------------------------------------------------


#include "Simulation.h"
#include "Event.h"
#include "SimConst.h"
#include "Interface.h"

Simulation::~Simulation() {

	delete newInt;
	
}

void Simulation::run(int mins, Interface *pThis) {
	int fortsatt = 0;																		// Sätt fortsätt till 0 initialt för att inte fortsätta
	int korTill = currentTime + mins;														// Sluttid för simulation = inparam
	currentTime = getSimStart();															// Sätt nuvarande tid till vald starttid, def = 0
	if (korTill >= SIM_TIME) {																// Om körtill är större än SIM_TIME, sätt kör till = SIM_TIME
		korTill = SIM_TIME;
	}

	while (!eventQueue.empty()) {
		if (fortsatt == 0) {																// Om fortsätt = 0, lägg till intervalltid
			currentTime += intervallTime;		
			system("CLS");
		}
		else if (fortsatt == 1) {
			currentTime = currentTime;														// om fortsätt är 1, behåll tiden
		}

		Event * nextEvent = eventQueue.top();
		if (nextSet == false) {																// Om nextset = false, fortsätt loopa event i kön
			Time current;
			current.fromMinutes(currentTime);
			cout << current << endl;
		}

		if (currentTime >= nextEvent->getTime() && nextSet == false) {						// Om tiden är större än eller lika med tiden för nästa event
			Time displayTime;																// samt nextSet, fortsätt loopa event som matchar tiden
			displayTime.fromMinutes(currentTime);
			eventQueue.pop();			
			nextEvent->processEvent();														// Processa nästa event

			getRunning();																	// Hämta data för statistik 
			getDone();
			getDelayed();
			getAll();			
			fortsatt = 1;																	// Sätt fortsätt till 1, fortsätt loopa event
			delete nextEvent;
		}

		else if (nextSet) {																	// Om nextSet = true, loopa bara ett event
			Time displayTime;
			displayTime.fromMinutes(nextEvent->getTime());
			cout << "Eventtime" << displayTime << endl;										// Skriv ut tid för event
			currentTime = nextEvent->getTime();												// Sätt tid till eventets tid
			eventQueue.pop();
			nextEvent->processEvent();														// Processa event
			getRunning();																	// Hämta statistik
			getDone();
			getDelayed();
			getAll();
			fortsatt = 0;																	// Sätt fortsätt till 0 för att avbryta eventkön
			delete nextEvent;
			nextSet = true;			
			pThis->showMenu(1);																// anropa meny när ett event har gått klart
		}
		else if (nextEvent->getTime() >= korTill && doneTrains.size() == runTrains.size()) {		// När simulationstiden är slut, vänta tills alla running tåg har kommit fram
			cout << "Done trains" << doneTrains.size() << endl;
			cout << "Running trains" << runTrains.size() << endl;
				//currentTime = korTill;
			system("pause");
				break;																				// Avbryt loop

		}		
		else {
			nextTime = nextEvent->getTime();												// Om simulationen inte är klar/eventen inom tidsintervall är klara sätt nextTime till tid för senaste eventet
			pThis->showMenu(1);																// Visa meny
			fortsatt = 0;																	// Sätt fortsätt = 0

		}		

	}

	showStats(pThis);
	pThis->showMenu(5);																		// När simulationen är klar, visa slutmeny
}

void Simulation::setIntervall(int pIntervall) {
	intervallTime = pIntervall;
}

void Simulation::scheduleEvent(Event * newEvent) {
	eventQueue.push(newEvent);
}

void addtoTrain(Event *e) {
	e->getStatus();
}

void Simulation::getRunning() {
	if (!eventQueue.empty()) {
		Event * nextEvent = eventQueue.top();
		if (nextEvent->getTyp() == "Arrivedevent") {										
			shared_ptr<Tag> endTrain = nextEvent->getTag();
			if (endTrain->getTillstand() == RUNNING)
				runTrains.push_back(endTrain);
		}
	}
}

void Simulation::getDone() {
	if (!eventQueue.empty()) {
		Event * nextEvent = eventQueue.top();
		if (nextEvent->getTyp() == "doneEvent") {
			shared_ptr<Tag> endTrain = nextEvent->getTag();
			if (endTrain->getTillstand() == FINISHED)
				doneTrains.push_back(endTrain);
		}
	}
}

void Simulation::getAll() {
	if (!eventQueue.empty()) {
		Event * nextEvent = eventQueue.top();
		shared_ptr<Tag> endTrain = nextEvent->getTag();
		if (endTrain->getTillstand() == INCOMPLETE) {
			allTrains.push_back(endTrain);
		}
		sort(allTrains.begin(), allTrains.end());
		allTrains.erase(unique(allTrains.begin(), allTrains.end()), allTrains.end());
	}
}

void Simulation::getDelayed() {
	if (!eventQueue.empty()) {
		Event * nextEvent = eventQueue.top();
		if (nextEvent->getTyp() == "Finishevent") {
			shared_ptr<Tag> endTrain = nextEvent->getTag();
			Time compTime;
			compTime.setHour(0);
			compTime.setMinutes(0);
			if (compTime < endTrain->getDelay()) {
				lateTrains.push_back(endTrain);
			}
		}
		sort(lateTrains.begin(), lateTrains.end());
		lateTrains.erase(unique(lateTrains.begin(), lateTrains.end()), lateTrains.end());
	}
}

void Simulation::filter() {
		for (auto i : lateTrains) {
			auto it = remove_if(allTrains.begin(), allTrains.end(), [=](shared_ptr<Tag> m) {return m->getTagnummer() == i->getTagnummer();});
			cout << i->getTagnummer();
		}
	
	sort(allTrains.begin(), allTrains.end());
	allTrains.erase(unique(allTrains.begin(), allTrains.end()), allTrains.end());
}

void Simulation::showStats(Interface *pThis) {
	cout << "**** Simulation Avslutad ****" << endl;
	//filter();
	Time endTime;
	endTime.fromMinutes(currentTime);
	cout << endTime << endl;
	cout << "Tåg som kom fram " << doneTrains.size() << endl;
	Time onTime;
	onTime.setHour(0);
	onTime.setMinutes(0);
	int itid = 0;
	for (auto i : doneTrains) {
		if (i->getDelay() == onTime) {
			cout << i->getTagnummer() << " ";
			itid++;
		}
	}
	cout << "\nTåg som kom fram i tid \n" << itid;
	cout << "\nTågen som aldrig blev ihopplockade:" << endl;
	

	//filter();
	for (auto i : allTrains) {
		if (i->getTillstand() == INCOMPLETE) {
			Time orgAvg = i->getStartTime() - i->getDelay();
			Time orgAnk = i->getDestTime() - i->getDelay();
			cout << "|Tåg nummer[" << i->getTagnummer() << "]" << " från " << i->getStart() << i->getStartTime()
				<< " (" << orgAvg << ") " << " till " << i->getDest() << " " << i->getDestTime() << " (" << orgAnk << ") " << endl;
			cout << "|Försening " << i->getDelay() << endl;
			cout << "______________________" << endl;
			cout << "Nuvarande fordon:" << endl;
			list<shared_ptr<Ralsfordon>>tmpList = i->getFordonList();
			Trainbuilder::printFordonlist(tmpList, 2);
			cout << "Saknade fordon:" << endl;
			list<string> missList = i->getLocigList();
			for (auto l : missList) {
				cout << l << endl;
			}
			cout << "\n\n";
		}
	}
	int totalDelay = 0;
	cout << "\nFörsenade tåg:\n";
	for (auto i : lateTrains) {
		Time orgAvg = i->getStartTime() - i->getDelay();
		Time orgAnk = i->getDestTime() - i->getDelay();
		cout << "|Tåg nummer[" << i->getTagnummer() << "]" << " från " << i->getStart() << i->getStartTime()
			<< " (" << orgAvg << ") " << " till " << i->getDest() << " " << i->getDestTime() << " (" << orgAnk << ") " 
		    << "|Försening " << i->getDelay() << endl;
		totalDelay += i->getDelay().toMinutes();
	}
	Time displayDelay;
	displayDelay.fromMinutes(totalDelay);
	cout << "Total förseningstid :" << displayDelay << endl;
	pThis->showMenu(5);
}