//-------------------------------------------------------------------
// Dragandefordon.h
// Event classes for handeling events in simulation
// David Hegardt 2016-10-28
//-------------------------------------------------------------------

#pragma once

#ifndef EVENT_H
#define EVENT_H
#include "Tag.h"
#include "Trainstatus.h"
#include "SimConst.h"



class Simulation;
class Trainstatus;

class Event {
public:
	// Constructor require time of event
	Event (unsigned int t) : time(t) { }

	virtual ~Event() { }

	// Process event by this method
	virtual void processEvent() = 0;
	virtual Trainstatus * getStatus() = 0;
	virtual string getTyp() = 0;
	virtual shared_ptr<Tag> getTag() = 0;

	unsigned int getTime() const {
		return time;
	}

protected:
	// Time for this event
	unsigned int time;
};

// Compare and rank Event with respect to time
class EventComparison {
public:
	bool operator() (Event *left, Event * right) {
		return left->getTime() > right->getTime();
	}
};

// ----- Derived Event-classes ---------------------------------------

class StartEvent : public Event {
public:
	StartEvent (Simulation *sim, Trainstatus *pStatus, int time, vector<shared_ptr<Tag>> pTagVec)
		: Event(time),status(pStatus),theSim(sim),tagVec(pTagVec), typ("Startevent") { }

	virtual void processEvent();
	/*Proccess startevent 
	  pre : Files read, Simulation objects created 
	*/

	virtual Trainstatus * getStatus() { return status; }													// Get Trainstatus object
	virtual string getTyp() { return typ; }																	// Get type of event
	virtual shared_ptr<Tag> getTag() { return 0; }															// Not used for this class

protected:
	Simulation *theSim;
	Trainstatus *status;
	vector<shared_ptr<Tag>> tagVec;
	string typ;
	int start = 0;
};

class AssembleEvent : public Event {
public:
	AssembleEvent(Simulation *sim, Trainstatus *pStatus, int time, shared_ptr<Tag> pTag)
		: Event(time), status(pStatus), theSim(sim), tag(pTag), typ("Assembleevent") { }

	virtual void processEvent();
	/*Process Assemble Event
	  pre : Startevent has been executed
	*/
	virtual Trainstatus * getStatus() { return status; }
	virtual string getTyp() { return typ; }
	virtual shared_ptr<Tag> getTag() { return tag; }														// Return trainobject

protected:
	Simulation *theSim;
	Trainstatus *status;
	shared_ptr<Tag> tag;
	string typ;
};

class ReadyEvent : public Event {
public:
	ReadyEvent(Simulation *sim, Trainstatus *pStatus, int time, shared_ptr<Tag> pTag)
		: Event(time), status(pStatus), theSim(sim), tag(pTag), typ("Readyevent") { }

	virtual void processEvent();
	/* Process Ready Event
	   pre : Assembly evenet completed
	*/

	virtual Trainstatus * getStatus() { return status; }
	virtual string getTyp() { return typ; }
	virtual shared_ptr<Tag> getTag() { return tag; }

protected:
	Simulation *theSim;
	Trainstatus *status;
	shared_ptr<Tag> tag;
	string typ;
};

class RunningEvent : public Event {
public:
	RunningEvent(Simulation *sim, Trainstatus *pStatus, int time, shared_ptr<Tag> pTag)
		: Event(time), status(pStatus), theSim(sim), tag(pTag), typ("Runningevent") { }

	virtual void processEvent();
	/* Process Running Event
	   pre : Ready event completed
	*/
	virtual Trainstatus * getStatus() { return status; }
	virtual string getTyp() { return typ; }
	virtual shared_ptr<Tag> getTag() { return tag; }

protected:
	Simulation *theSim;
	Trainstatus *status;
	shared_ptr<Tag> tag;
	string typ;
};

class ArrivedEvent : public Event {
public:
	ArrivedEvent(Simulation *sim, Trainstatus *pStatus, int time, shared_ptr<Tag> pTag)
		: Event(time), status(pStatus), theSim(sim), tag(pTag), typ("Arrivedevent") { }

	virtual void processEvent();
	/* Process Arrived event
	   pre : RunningEvent finished
	*/

	virtual Trainstatus * getStatus() { return status; }
	virtual string getTyp() { return typ; }
	virtual shared_ptr<Tag> getTag() { return tag; }

protected:
	Simulation *theSim;
	Trainstatus *status;
	shared_ptr<Tag> tag;
	string typ;
};

class FinishEvent : public Event {
public:
	FinishEvent(Simulation *sim, Trainstatus *pStatus, int time, shared_ptr<Tag> pTag)
		: Event(time), status(pStatus), theSim(sim), tag(pTag), typ("Finishevent") { }

	virtual void processEvent();
	/* Process Finish event
	pre : ArrivedEvent finished
	*/

	virtual Trainstatus * getStatus() { return status; }
	virtual string getTyp() { return typ; }
	virtual shared_ptr<Tag> getTag() { return tag; }

protected:
	Simulation *theSim;
	Trainstatus *status;
	shared_ptr<Tag> tag;
	string typ;
};

class doneEvent : public Event {
public:
	doneEvent(Simulation *sim, Trainstatus *pStatus, int time, shared_ptr<Tag> pTag)
		: Event(time), status(pStatus), theSim(sim), tag(pTag), typ("doneEvent") { }

	virtual void processEvent();
	/* Process done event
	pre : Finish event finished
	*/

	virtual Trainstatus * getStatus() { return status; }
	virtual string getTyp() { return typ; }
	virtual shared_ptr<Tag> getTag() { return tag; }

protected:
	Simulation *theSim;
	Trainstatus *status;
	shared_ptr<Tag> tag;
	string typ;
};




#endif
