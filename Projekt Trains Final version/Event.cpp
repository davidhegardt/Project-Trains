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
	filePtr.open("trainsim.log", ios::app);																				// Skapa och öppna logg
	if (!filePtr.is_open()) {
		throw runtime_error("Cannot open file");																		//Throw runtime error if operation fails
	}
	string dataToWrite = data.str();																					// Skapa sträng från stringstream objekt
	filePtr << dataToWrite.c_str();																						// Skriv data till fil

	filePtr.close();
}

void StartEvent::processEvent() {
	tagVec = status->getTagVec();
	vector<shared_ptr<Tag>>::iterator begin = tagVec.begin(), end = tagVec.end(), it;									// Hämta och loopa vector för samtliga tåg
	for (it = begin; it != end; it++) {
		Time avg = (*it)->getStartTime();																				// Hämta avgånstid		
			int ass_start = avg.toMinutes() - PREP_TIME;																// Tid för assembly = avgångstid - 30min
			if (ass_start > start) {																					// Kontrollera tid för event
				Event *e = new AssembleEvent(theSim, status, ass_start,*it);											// Skapa nya assemble event för samtliga tåg
				theSim->scheduleEvent(e);
			}
			else {
				continue;												
			}
	}

}

void AssembleEvent::processEvent() {
	if (time < SIM_TIME) {																								// Om tiden för eventet ligger innan 23:59
		stringstream buffer;
		cout << "Tåg nummer " << tag->getTagnummer() << " plockas ihop" << endl;										// Skriv ut info och läs in till buffer
		buffer << "Tåg nummer " << tag->getTagnummer() << " plockas ihop" << endl;
		if (status->tryAssemble(tag)) {																					// Skicka tåg för hop plockning

			tag->setTillstand(ASSEMBLED);																				// Ändra tillstånd för tåg
			time += ASSEMBLE_TIME;																						// Öka tiden med const ASSEMBLE_TIME
			Event *e = new ReadyEvent(theSim, status, time, tag);														// skicka tid för ready avgTid - 10
			theSim->scheduleEvent(e);
		}
		else {																											// Om hopkoppling misslyckas, skriv ut tåg
			cout << "Tåg " << tag->getTagnummer() << " Inte redo att köra" << endl;
			buffer << "Tåg " << tag->getTagnummer() << " Inte redo att köra" << endl;
			time += DELAY_TIME;																							// Öka delaytime
			Time addDelay;
			addDelay.setMinutes(10);																					// Skapa Time objekt och lägg till delaytid till tåg
			tag->setDelay(addDelay);
			tag->changeTime(addDelay);
			Event *e = new AssembleEvent(theSim, status, time, tag);													// Schedulera nytt försök att sätta ihop tåg
			theSim->scheduleEvent(e);
		}
		onCallBack(buffer);																								// Läs in till loggfil
	}
}

void ReadyEvent::processEvent() {
	if (time < SIM_TIME) {																								// Om tid är innan 23:59
		stringstream buffer;																							// Skapa buffer för inläsning
		status->ready(tag);
		cout << "Tåg nummer " << tag->getTagnummer() << " Redo för avgång från " << tag->getStart() << endl;			// Skriv ut info och läs till buffer
		buffer << "Tåg nummer " << tag->getTagnummer() << " Redo för avgång från " << tag->getStart() << endl;
		time += READY_TIME;																								// Lägg till READY_TIME
		Time testTime;
		testTime.fromMinutes(time);																						// Skapa timeobjekt
		cout << "Starttid : " << testTime << endl;																		// Skriv ut starttid och läs till buffer
		buffer << "Starttid : " << testTime << endl;

		tag->setTillstand(READY);																						// Ändra tillstånd
		Event *e = new RunningEvent(theSim, status, time, tag);															// Skapa och skicka nytt running event
		theSim->scheduleEvent(e);

		onCallBack(buffer);
	}
}

void RunningEvent::processEvent() {
	status->running(tag);																								// Anropa funktion för running
	stringstream buffer;
	cout << "Tåg med tågnummer " << tag->getTagnummer() << " har lämnat " << tag->getStart() << endl;					// Ändra status och skriv ut information, läs till buffer
	cout << " och kör mot " << tag->getDest() << endl;
	cout << "Medelhastighet : " << tag->getMedelH() << " km/h" << endl;

	buffer << "Tåg med tågnummer " << tag->getTagnummer() << " har lämnat " << tag->getStart() << endl << " och kör mot " << tag->getDest() << endl;

	cout << "Medelhastighet : " << tag->getMedelH() << " km/h" << endl;
	buffer << "Medelhastighet : " << tag->getMedelH() << " km/h" << endl;

	Time restid = tag->getDestTime() - tag->getStartTime();																// Räkna ut restid och presentera
	cout << "Starttid : " << tag->getStartTime() << endl;
	buffer << "Starttid : " << tag->getStartTime() << endl;

	int resMin = restid.toMinutes();
	cout << "Restid : " << restid << endl;
	buffer << "Restid : " << restid << endl;
	time += resMin;
	tag->setTillstand(RUNNING);																							// Ändra tillstånd
	Event *e = new ArrivedEvent(theSim, status, time, tag);																// Skapa och skicka nytt Arrive event 
	theSim->scheduleEvent(e);

	onCallBack(buffer);
}

void ArrivedEvent::processEvent() {
	stringstream buffer;
	status->arrived(tag);																								// Ändra tillstånd
	cout << "Tåg " << tag->getTagnummer() << " från " << tag->getStart() <<												// Ändra status och skriv ut information, läs till buffer
		" har anlänt vid " << tag->getDest() << endl;

	buffer << "Tåg " << tag->getTagnummer() << " från " << tag->getStart() << " har anlänt vid " << tag->getDest() << endl;

	Time testTime;
	testTime.fromMinutes(time);
	cout << "Sluttid : " << testTime << endl;
	buffer << "Sluttid : " << testTime << endl;

	time += ARRIVAL_TIME;																								// Lägg till ARRIVAL_TIME
	tag->setTillstand(ARRIVED);																							// Ändra tillstånd
	Event *e = new FinishEvent(theSim, status, time, tag);																// Skapa och skicka nytt Finish event
	theSim->scheduleEvent(e);

	onCallBack(buffer);
}

void FinishEvent::processEvent() {
	stringstream buffer;																								// Ändra status och skriv ut information, läs till buffer
	status->finishUp(tag);																								// Anropa funktion för finish
	cout << "Tåg " << tag->getTagnummer() << " från " << tag->getStart() <<
		" har lämnat ralsfordon till " << tag->getDest() << endl;

	buffer << "Tåg " << tag->getTagnummer() << " från " << tag->getStart() << " har lämnat ralsfordon till " << tag->getDest() << endl;

	tag->setTillstand(FINISHED);																						// Ändra tillstånd
	Event *e = new doneEvent(theSim, status, time, tag);																// Skapa och skicka nytt Arrive event
	theSim->scheduleEvent(e);
}

void doneEvent::processEvent() {
	cout << "Tåg isärplockat" << endl;


}

