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
	int fortsatt = 0;																		// S�tt forts�tt till 0 initialt f�r att inte forts�tta
	int korTill = currentTime + mins;														// Sluttid f�r simulation = inparam
	currentTime = getSimStart();															// S�tt nuvarande tid till vald starttid, def = 0
	if (korTill >= SIM_TIME) {																// Om k�rtill �r st�rre �n SIM_TIME, s�tt k�r till = SIM_TIME
		korTill = SIM_TIME;
	}

	while (!eventQueue.empty()) {
		if (fortsatt == 0) {																// Om forts�tt = 0, l�gg till intervalltid
			currentTime += intervallTime;		
			system("CLS");
		}
		else if (fortsatt == 1) {
			currentTime = currentTime;														// om forts�tt �r 1, beh�ll tiden
		}

		Event * nextEvent = eventQueue.top();
		if (nextSet == false) {																// Om nextset = false, forts�tt loopa event i k�n
			Time current;
			current.fromMinutes(currentTime);
			cout << current << endl;
		}

		if (currentTime >= nextEvent->getTime() && nextSet == false) {						// Om tiden �r st�rre �n eller lika med tiden f�r n�sta event
			Time displayTime;																// samt nextSet, forts�tt loopa event som matchar tiden
			displayTime.fromMinutes(currentTime);
			eventQueue.pop();			
			nextEvent->processEvent();														// Processa n�sta event

			getRunning();																	// H�mta data f�r statistik 
			getDone();
			getDelayed();
			getAll();			
			fortsatt = 1;																	// S�tt forts�tt till 1, forts�tt loopa event
			delete nextEvent;
		}

		else if (nextSet) {																	// Om nextSet = true, loopa bara ett event
			Time displayTime;
			displayTime.fromMinutes(nextEvent->getTime());
			cout << "Eventtime" << displayTime << endl;										// Skriv ut tid f�r event
			currentTime = nextEvent->getTime();												// S�tt tid till eventets tid
			eventQueue.pop();
			nextEvent->processEvent();														// Processa event
			getRunning();																	// H�mta statistik
			getDone();
			getDelayed();
			getAll();
			fortsatt = 0;																	// S�tt forts�tt till 0 f�r att avbryta eventk�n
			delete nextEvent;
			nextSet = true;			
			pThis->showMenu(1);																// anropa meny n�r ett event har g�tt klart
		}
		else if (nextEvent->getTime() >= korTill && doneTrains.size() == runTrains.size()) {		// N�r simulationstiden �r slut, v�nta tills alla running t�g har kommit fram
			cout << "Done trains" << doneTrains.size() << endl;
			cout << "Running trains" << runTrains.size() << endl;
				//currentTime = korTill;
			system("pause");
				break;																				// Avbryt loop

		}		
		else {
			nextTime = nextEvent->getTime();												// Om simulationen inte �r klar/eventen inom tidsintervall �r klara s�tt nextTime till tid f�r senaste eventet
			pThis->showMenu(1);																// Visa meny
			fortsatt = 0;																	// S�tt forts�tt = 0

		}		

	}

	showStats(pThis);
	pThis->showMenu(5);																		// N�r simulationen �r klar, visa slutmeny
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
	cout << "T�g som kom fram " << doneTrains.size() << endl;
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
	cout << "\nT�g som kom fram i tid \n" << itid;
	cout << "\nT�gen som aldrig blev ihopplockade:" << endl;
	

	//filter();
	for (auto i : allTrains) {
		if (i->getTillstand() == INCOMPLETE) {
			Time orgAvg = i->getStartTime() - i->getDelay();
			Time orgAnk = i->getDestTime() - i->getDelay();
			cout << "|T�g nummer[" << i->getTagnummer() << "]" << " fr�n " << i->getStart() << i->getStartTime()
				<< " (" << orgAvg << ") " << " till " << i->getDest() << " " << i->getDestTime() << " (" << orgAnk << ") " << endl;
			cout << "|F�rsening " << i->getDelay() << endl;
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
	cout << "\nF�rsenade t�g:\n";
	for (auto i : lateTrains) {
		Time orgAvg = i->getStartTime() - i->getDelay();
		Time orgAnk = i->getDestTime() - i->getDelay();
		cout << "|T�g nummer[" << i->getTagnummer() << "]" << " fr�n " << i->getStart() << i->getStartTime()
			<< " (" << orgAvg << ") " << " till " << i->getDest() << " " << i->getDestTime() << " (" << orgAnk << ") " 
		    << "|F�rsening " << i->getDelay() << endl;
		totalDelay += i->getDelay().toMinutes();
	}
	Time displayDelay;
	displayDelay.fromMinutes(totalDelay);
	cout << "Total f�rseningstid :" << displayDelay << endl;
	pThis->showMenu(5);
}