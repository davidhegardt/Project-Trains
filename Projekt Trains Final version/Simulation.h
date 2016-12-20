//-------------------------------------------------------------------
// Simulation.h
// Simulation class for running discreet event driven simulation
// David Hegardt 2016-10-28
//-------------------------------------------------------------------

#ifndef SIMULATION_H
#define SIMULATION_H

#include <queue>
#include <vector>
#include "Event.h"
#include "Trainbuilder.h"


using namespace std;

class Interface;

class Simulation {
	
public:
	Simulation() : currentTime(0), eventQueue() { setIntervall(10); }

	~Simulation();

	// Add a new event to event queue.
	void scheduleEvent(Event * newEvent);
	/* Schedule event - function to add a new event to the event queu
	   pre : event created
	*/

	int getTime() const { return currentTime; }

	void setIntervall(int pIntervall);
	/* Set intervall - retrieve how much time should proceed in simulation
	
	*/

	int getIntervall() { return intervallTime; }
	/* Get intervall - retrieve current time of intervall
	
	*/

	int getNextTime() { return nextTime; }
	/* Return time of next event in eventqueue
	
	*/

	void setNextSet(bool pNext) { nextSet = pNext; }
	/* Verify if next event time should be used
	
	*/

	void showStats(Interface *pThis);
	/* Showstats - show statistics from simulation
	   pre : simulation has been completed
	*/

	int getSimStart() { return SimStart; }
	void setSimStart(int pStart) { SimStart = pStart; }

	void run(int mins, Interface *pThis);
	void testMenu();

	void getAll();
	void getRunning();
	void getDone();
	void getDelayed();
	void filter();

private:
	int currentTime; // Time for last processed event

					 /* The event queue. Always sorted with respect to the times
					 for the events. The event with the 'smallest' time is always
					 placed first in queue and will be processed next. */

	int intervallTime;
	int nextTime;
	vector<shared_ptr<Tag>> runTrains;
	vector<shared_ptr<Tag>> doneTrains;
	vector<shared_ptr<Tag>> allTrains;
	vector<shared_ptr<Tag>> lateTrains;
	Interface* newInt;
	bool nextSet = false;
	int SimStart = 0;
	priority_queue<Event*, vector<Event*>, EventComparison> eventQueue;
};

#endif