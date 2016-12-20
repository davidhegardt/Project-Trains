//-------------------------------------------------------------------
// Event.cpp
// Event classes - implementation
// David Hegardt 2016-10-28
//-------------------------------------------------------------------

#include "Event.h"
#include "Simulation.h"
#include "Trainstatus.h"
#include <sstream>
#include <fstream>
#include <ostream>

/* Classless function to retrieve and write logfile
   inparam : stringstream data to read
*/
void onCallBack(const std::stringstream &data)
{
	ofstream filePtr;																									// Skapa ostream objekt																					
	filePtr.open("trainsim.log", ios::app);																				// Skapa och �ppna logg
	if (!filePtr.is_open()) {
		throw runtime_error("Cannot open file");																		//Throw runtime error if operation fails
	}
	string dataToWrite = data.str();																					// Skapa str�ng fr�n stringstream objekt
	filePtr << dataToWrite.c_str();																						// Skriv data till fil

	filePtr.close();
}

void StartEvent::processEvent() {
	tagVec = status->getTagVec();
	vector<shared_ptr<Tag>>::iterator begin = tagVec.begin(), end = tagVec.end(), it;									// H�mta och loopa vector f�r samtliga t�g
	for (it = begin; it != end; it++) {
		Time avg = (*it)->getStartTime();																				// H�mta avg�nstid		
			int ass_start = avg.toMinutes() - PREP_TIME;																// Tid f�r assembly = avg�ngstid - 30min
			if (ass_start > start) {																					// Kontrollera tid f�r event
				Event *e = new AssembleEvent(theSim, status, ass_start,*it);											// Skapa nya assemble event f�r samtliga t�g
				theSim->scheduleEvent(e);
			}
			else {
				continue;												
			}
	}

}

void AssembleEvent::processEvent() {
	if (time < SIM_TIME) {																								// Om tiden f�r eventet ligger innan 23:59
		stringstream buffer;
		cout << "T�g nummer " << tag->getTagnummer() << " plockas ihop" << endl;										// Skriv ut info och l�s in till buffer
		buffer << "T�g nummer " << tag->getTagnummer() << " plockas ihop" << endl;
		if (status->tryAssemble(tag)) {																					// Skicka t�g f�r hop plockning

			tag->setTillstand(ASSEMBLED);																				// �ndra tillst�nd f�r t�g
			time += ASSEMBLE_TIME;																						// �ka tiden med const ASSEMBLE_TIME
			Event *e = new ReadyEvent(theSim, status, time, tag);														// skicka tid f�r ready avgTid - 10
			theSim->scheduleEvent(e);
		}
		else {																											// Om hopkoppling misslyckas, skriv ut t�g
			cout << "T�g " << tag->getTagnummer() << " Inte redo att k�ra" << endl;
			buffer << "T�g " << tag->getTagnummer() << " Inte redo att k�ra" << endl;
			time += DELAY_TIME;																							// �ka delaytime
			Time addDelay;
			addDelay.setMinutes(10);																					// Skapa Time objekt och l�gg till delaytid till t�g
			tag->setDelay(addDelay);
			tag->changeTime(addDelay);
			Event *e = new AssembleEvent(theSim, status, time, tag);													// Schedulera nytt f�rs�k att s�tta ihop t�g
			theSim->scheduleEvent(e);
		}
		onCallBack(buffer);																								// L�s in till loggfil
	}
}

void ReadyEvent::processEvent() {
	if (time < SIM_TIME) {																								// Om tid �r innan 23:59
		stringstream buffer;																							// Skapa buffer f�r inl�sning
		status->ready(tag);
		cout << "T�g nummer " << tag->getTagnummer() << " Redo f�r avg�ng fr�n " << tag->getStart() << endl;			// Skriv ut info och l�s till buffer
		buffer << "T�g nummer " << tag->getTagnummer() << " Redo f�r avg�ng fr�n " << tag->getStart() << endl;
		time += READY_TIME;																								// L�gg till READY_TIME
		Time testTime;
		testTime.fromMinutes(time);																						// Skapa timeobjekt
		cout << "Starttid : " << testTime << endl;																		// Skriv ut starttid och l�s till buffer
		buffer << "Starttid : " << testTime << endl;

		tag->setTillstand(READY);																						// �ndra tillst�nd
		Event *e = new RunningEvent(theSim, status, time, tag);															// Skapa och skicka nytt running event
		theSim->scheduleEvent(e);

		onCallBack(buffer);
	}
}

void RunningEvent::processEvent() {
	status->running(tag);																								// Anropa funktion f�r running
	stringstream buffer;
	cout << "T�g med t�gnummer " << tag->getTagnummer() << " har l�mnat " << tag->getStart() << endl;					// �ndra status och skriv ut information, l�s till buffer
	cout << " och k�r mot " << tag->getDest() << endl;
	cout << "Medelhastighet : " << tag->getMedelH() << " km/h" << endl;

	buffer << "T�g med t�gnummer " << tag->getTagnummer() << " har l�mnat " << tag->getStart() << endl << " och k�r mot " << tag->getDest() << endl;

	cout << "Medelhastighet : " << tag->getMedelH() << " km/h" << endl;
	buffer << "Medelhastighet : " << tag->getMedelH() << " km/h" << endl;

	Time restid = tag->getDestTime() - tag->getStartTime();																// R�kna ut restid och presentera
	cout << "Starttid : " << tag->getStartTime() << endl;
	buffer << "Starttid : " << tag->getStartTime() << endl;

	int resMin = restid.toMinutes();
	cout << "Restid : " << restid << endl;
	buffer << "Restid : " << restid << endl;
	time += resMin;
	tag->setTillstand(RUNNING);																							// �ndra tillst�nd
	Event *e = new ArrivedEvent(theSim, status, time, tag);																// Skapa och skicka nytt Arrive event 
	theSim->scheduleEvent(e);

	onCallBack(buffer);
}

void ArrivedEvent::processEvent() {
	stringstream buffer;
	status->arrived(tag);																								// �ndra tillst�nd
	cout << "T�g " << tag->getTagnummer() << " fr�n " << tag->getStart() <<												// �ndra status och skriv ut information, l�s till buffer
		" har anl�nt vid " << tag->getDest() << endl;

	buffer << "T�g " << tag->getTagnummer() << " fr�n " << tag->getStart() << " har anl�nt vid " << tag->getDest() << endl;

	Time testTime;
	testTime.fromMinutes(time);
	cout << "Sluttid : " << testTime << endl;
	buffer << "Sluttid : " << testTime << endl;

	time += ARRIVAL_TIME;																								// L�gg till ARRIVAL_TIME
	tag->setTillstand(ARRIVED);																							// �ndra tillst�nd
	Event *e = new FinishEvent(theSim, status, time, tag);																// Skapa och skicka nytt Finish event
	theSim->scheduleEvent(e);

	onCallBack(buffer);
}

void FinishEvent::processEvent() {
	stringstream buffer;																								// �ndra status och skriv ut information, l�s till buffer
	status->finishUp(tag);																								// Anropa funktion f�r finish
	cout << "T�g " << tag->getTagnummer() << " fr�n " << tag->getStart() <<
		" har l�mnat ralsfordon till " << tag->getDest() << endl;

	buffer << "T�g " << tag->getTagnummer() << " fr�n " << tag->getStart() << " har l�mnat ralsfordon till " << tag->getDest() << endl;

	tag->setTillstand(FINISHED);																						// �ndra tillst�nd
	Event *e = new doneEvent(theSim, status, time, tag);																// Skapa och skicka nytt Arrive event
	theSim->scheduleEvent(e);
}

void doneEvent::processEvent() {
	cout << "T�g is�rplockat" << endl;


}

