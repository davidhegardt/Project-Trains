//-------------------------------------------------------------------
// Interface.cpp
// Class for handeling user interface and printout - implementation
// David Hegardt 2016-10-28
//-------------------------------------------------------------------

#include "Interface.h"
#include "Simulation.h"

Interface::~Interface() {

	delete theSim;
	delete status;
}

void Interface::showMenu(int level) {																			// Print menu items based on current level
	
	cout << "**** TRAINSIMULATOR 3000 ****" << endl;
	if (level == 0) {
		theLevel = 0;
		cout << "1. Ändra start tid " << starTim << endl;
		cout << "2. Ändra slut tid " << endTim << endl;
		cout << "3. Starta Simulering" << endl;
	}
	if (level == 1) {
		theLevel = 1;
		cout << "* Simuleringsfunktioner *" << endl;
		cout << "1. Ändra intervall" << endl;
		cout << "2. Kör nästa [" << currTyp << "]" << endl;
		cout << "3. Kör nästa event" << endl;
		cout << "4. Change log level[" << loglevel << "]" << endl;
		cout << "5. Train menu" << endl;
		cout << "6. Station menu" << endl;
		cout << "7. Timetable" << endl;
		cout << "8. Return" << endl;
	}
	if (level == 2) {
		theLevel = 2;
		cout << "* Train menu *" << endl;
		cout << "1. Search for vehicle ID" << endl;
		cout << "2. Search for Train" << endl;
		cout << "3. Change log level" << endl;
		cout << "4. Return" << endl;
	}
	if (level == 3) {
		theLevel = 3;
		cout << "* Station menu *" << endl;
		cout << "1. Show station names" << endl;
		cout << "2. Search for station" << endl;
		cout << "3. Change log level" << endl;
		cout << "4. Return" << endl;
	}
	if (level == 5) {
		theLevel = 5;
		simFinished = true;
		cout << "* Sim Slut Meny *" << endl;
		cout << "1. Visa statistik" << endl;
		cout << "2. Train menu" << endl;
		cout << "3. Station menu" << endl;
		cout << "4. Change log level" << endl;
		cout << "5. Return" << endl;
	}
	int val;
	cout << "Enter Choice:";																				// Get and verify user input
	if (cin >> val) {
		getMenuOption(val);
	}
	else {
		cout << "invalid input" << endl;
		cin.clear();
		cin.get();
		showMenu(theLevel);
	}
}

void Interface::getMenuOption(int val) {																	// Get menu option and call function corresponding to choice
	if (theLevel == 0) {
		switch (val) {
		case 1: changeSimStart();
			break;
		case 2: changeSimEnd();
			break;
		case 3: runSim();
			break;
		default: showMenu(0);
			break;
		}
	}
	else if (theLevel == 1) {
		switch (val) {

		case 1: changeInterval();
			break;
		case 2: //
			break;
		case 3: nextEvent();
			break;
		case 4: changeLogLevel();
			break;
		case 5: showMenu(2);
			break;
		case 6: showMenu(3);
			break;
		case 7: timeTable();
			break;
		case 8: showMenu(1);
			break;
		default: showMenu(1);
			break;
		}
	}
	else if (theLevel == 2) {
		switch (val) {

		case 1: findFordon();
			break;
		case 2: searchTrains();
			break;
		case 3: changeLogLevel();
			break;
		case 4: if (simFinished) {
						showMenu(5);
				}
				else { showMenu(1); }
			break;		
		default: showMenu(2);
			break;
		}
	}
	else if (theLevel == 3) {
		switch (val) {

		case 1: showAllStations();
			break;
		case 2: searchStation();
			break;
		case 3: changeLogLevel();
			break;
		case 4: if (simFinished) {																		// If Sim is finished, disabled user from calling obsolete functions
						showMenu(5);
				}
				else { showMenu(1); }
			break;		
		default: showMenu(3);
			break;
		}
	}
	else if (theLevel == 5) {
		switch (val) {

		case 1: theSim->showStats(this);																// Call function to show statistics
			break;
		case 2: showMenu(2);
			break;
		case 3: showMenu(3);
			break;
		case 4: changeLogLevel();
			break;
		case 5: showMenu(5);
			break;
		default: showMenu(5);
			break;
		}
	}

}


void Interface::createSim() {																			// Entry point for simulation
	theSim = new Simulation;																			// Create simulation objects and trainstatus objects
	status = new Trainstatus;
	starTim.fromMinutes(0);
	endTim.fromMinutes(SIM_TIME);
	int simTime = 0;																					// Set default simulation time of not changed
	int steg = 10;																						// Set default intervall time

	status->readInfo();
	tagVec = status->getTagVec();
	theSim->scheduleEvent(new StartEvent(theSim, status, simTime, tagVec));								// Schedule new startevent

	showMenu(0);
	
}

void Interface::runSim() {
	theSim->run(sim_end, this);																			// Run simulation aftter start/end time is chosen, send end_time and interface
}

void Interface::changeSimStart() {
	int newStart;
	cout << "Enter start time of Simulation" << endl;
	if (cin >> newStart) {
		if (newStart > SIM_TIME || newStart < 0) {
			cout << "Simulationen måste starta innan 23:59" << endl;									// Verify input
		}
		else {
			theSim->setSimStart(newStart);
			starTim.fromMinutes(newStart);
			cout << "Simulationen kommer löpa fram till startpunkt" << endl;
		}
	}
	else {
		cout << "invalid input" << endl;
		cin.clear();
		cin.get();
		changeSimStart();
	}

	showMenu(0);
}

void Interface::changeSimEnd() {
	int newEnd;
	cout << "Enter end time of Simulation" << endl;
	if (cin >> newEnd) {
		if (newEnd < starTim.toMinutes()) {
			cout << "Sluttid måste vara efter " << starTim << endl;										// Verify input
		}
		else {
			sim_end = newEnd;
			endTim.fromMinutes(newEnd);
		}
	}
	else {
		cout << "invalid input" << endl;
		cin.clear();
		cin.get();
		changeSimEnd();
	}
	
	showMenu(0);
}

void Interface::changeInterval() {
	intervall;

	cout << "Enter intervall lenght" << endl;
	if (cin >> intervall) {
		theSim->setIntervall(intervall);		 
		theSim->setNextSet(false);																	
		currTyp = "intervalltid";
	}
	else {
		cout << "invalid input" << endl;
		cin.clear();
		cin.get();
		changeInterval();
	}

	showMenu(1);
}

void Interface::nextEvent() {
	cout << "Running to Next event" << endl;
	intervall = theSim->getNextTime();
	theSim->setIntervall(intervall);
	theSim->setNextSet(true);
	Eventtime = true;
	currTyp = "Eventtid";
}

void Interface::findFordon() {
	int id;
	cout << "**** Vehicle Finder ****" << endl;
	cout << "Enter fordon Id to find" << endl;
	if (cin >> id) {
		status->findFordon(id, loglevel);																// Call function to find vehicle
		cin.get();
	}
	else {
		cout << "invalid input" << endl;
		cin.clear();
		cin.get();
		findFordon();
	}
	showMenu(2);
}

void Interface::searchTrains() {

	int trainId;

	vector<shared_ptr<Tag>> copyVec = status->getCopyTag();

	cout << "Enter train number to search for" << endl;
	if (cin >> trainId) {
		cout << "***** Trains *****" << endl;
		vector<shared_ptr<Tag>>::iterator begin = copyVec.begin(), end = copyVec.end(), it;				 //Loop vector of all trains
		for (it = begin; it != end; it++) {
			if ((*it)->getTagnummer() == trainId) {														// If tågnummer matches input trainid
				cout << "| Tågnummer : " << (*it)->getTagnummer() << endl;
				cout << "| Avgångstation : " << (*it)->getStart() << endl;
				cout << "| Destination : " << (*it)->getDest() << endl;
				cout << "| Medelhastighet : " << (*it)->getMedelH() << " km/h" << endl;
				cout << "| Tillstånd " << statusStrings[(*it)->getTillstand()] << endl;
				cout << "|_____________________________________" << endl;

				cout << "Påkopplade fordon" << endl;
				Trainbuilder::printFordonlist((*it)->getFordonList(), loglevel);						// Call functiion to print list of fordon

			}
		}
	}
	else {
		cout << "invalid input" << endl;
		cin.clear();
		cin.get();
		searchTrains();
	}

	
	showMenu(2);
}

void Interface::searchStation() {

	string statName;

	statVec = status->getStations();

	cout << "Enter Station name to search for" << endl;
	cin.get();
	getline(cin, statName);

	cout << "***** Stations *****" << endl;
	vector<shared_ptr<Station>>::iterator begin = statVec.begin(), end = statVec.end(), it;				//Loop vector of all stations
	for (it = begin; it != end; it++) {
		if ((*it)->getStationName() == statName) {														// If input station name matches
			(*it)->printInfo(loglevel);
		}
	}
	showMenu(3);

}

bool wayToSort( shared_ptr<Tag> left, shared_ptr<Tag> right) { return left->getStartTime() < right->getStartTime(); }			// Predicate function to sort by start time

void Interface::timeTable() {

	vector<shared_ptr<Tag>> copyVec = status->getCopyTag();

	cout << "***** Trains *****" << endl;
	sort(copyVec.begin(), copyVec.end(), wayToSort);
	vector<shared_ptr<Tag>>::iterator begin = copyVec.begin(), end = copyVec.end(), it;								//Loop vector of all trains
	for (it = begin; it != end; it++) {
			cout << "| Tågnummer : " << (*it)->getTagnummer() << endl;												// Print information
			cout << "| Avgångstation : " << (*it)->getStart() << endl;
			cout << "| Avgångstid : " << (*it)->getStartTime() << endl;
			cout << "| Ankomsttid : " << (*it)->getDestTime() << endl;
			cout << "| Destination : " << (*it)->getDest() << endl;
			cout << "| Tillstånd " << statusStrings[(*it)->getTillstand()] << endl;									// If train is running, print medelhastighet
			if ((*it)->getTillstand() == RUNNING) {
				cout << "| Meddelhastighet : " << (*it)->getMedelH() << " km/h" << endl;
			}
			cout << "|_____________________________________" << endl;
			}		
	showMenu(1);
}

void Interface::showAllStations() {
	statVec = status->getStations();

	vector<shared_ptr<Station>>::iterator begin = statVec.begin(), end = statVec.end(), it;				// Loop vector of all stations, print station name
	for (it = begin; it != end; it++) {
		cout << (*it)->getStationName() << endl;
	}
	showMenu(3);
}

void Interface::changeLogLevel() {
	int newLevel;
	cout << "Choose level :" << endl;
	cout << "1. Low Log level" << endl;
	cout << "2. High Log level" << endl;

	cout << "Enter loglevel" << endl;
	if (cin >> newLevel) {
		loglevel = newLevel;
		cout << "Log level changed to " << loglevel << endl;
	}
	else {
		cout << "invalid input" << endl;
		cin.clear();
		cin.get();
		changeLogLevel();
	}
	if (simFinished) {
		showMenu(5);
	}
	else { showMenu(1); }
}

