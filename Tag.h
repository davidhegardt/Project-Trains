//-------------------------------------------------------------------
// Tag.h
// Train class
// David Hegardt 2016-10-28
//-------------------------------------------------------------------

#ifndef TagH
#define TagH

#include "Dragandefordon.h"
#include "Godsvagn.h"
#include "Personvagn.h"
#include "Timer.h"
#include <list>
#include <memory>

enum STATUS  {NOT_ASSEMBLED,INCOMPLETE,ASSEMBLED,READY,RUNNING,ARRIVED,FINISHED};
static const char * statusStrings[] = { "NOT_ASSEMBLED","INCOMPLETE","ASSEMBLED","READY","RUNNING","ARRIVED","FINISHED" };

class Tag {
public:
	Tag(int pTagnummer,string pUrsprung,string pDest,Time pUrsprungtid, Time pDesttid, int pMaxSpeed, Time pDelay, STATUS pTillstand,list<string> pLogiclist) :
		tagnummer(pTagnummer), ursprung(pUrsprung), destination(pDest), ursprungtid(pUrsprungtid), desttid(pDesttid), maxSpeed(pMaxSpeed),
		delay(0,0), tillstand(NOT_ASSEMBLED),logicList(pLogiclist) {}

	~Tag();

	Time getStartTime() {return ursprungtid;}
	/* Get Start time
	   return start time for train
	*/

	Time getDestTime() { return desttid; }
	/* Get Dest Time
	   return destination time for train
	*/

	Time getDelay() { return delay; }
	/* Get delay
	   return time of delay
	*/

	void setDelay(Time pDelay);
	/* Set delay
	   set time of delay
	*/

	string getDest();
	/* Get destination
	   return destination name
	*/

	string getStart();
	/* Get Start
	   return start station name
	*/

	int getTagnummer();
	/* Get tagnummer
	   Return train number
	*/
	int getHastighet();
	/* Get hastighet
	   Return hastighet
	*/

	void setMedelH(int pSpeed) { medelH = pSpeed; }
	/* set medelhastighet for train*/

	double getMedelH() { return medelH; }
	/* get medelhastighet */

	void changeTime(Time newTime);
	/* change time of of start and departure time*/

	void addFordon(shared_ptr<Ralsfordon> pFordon);
	/* Add fordon
	   Add fordon to train
	*/
	void remList(string aTyp);
	/* Remove one fordon from list of ingående fordon
	*/
	void clearList();
	/* Clear whole list of ingående fordon*/
	void printASCII(string typ);
	/* Print graphics based on type of fordon*/

	STATUS getTillstand();
	/* Get current status of train*/

	void setTillstand(STATUS ptillstand);
	/* Set current status of train*/

	list<shared_ptr<Ralsfordon>> getFordonList();
	/* Retrieve list of current fordon*/

	list <string> getLocigList();
	/* Retrieve list of ingående fordon*/

	list <string> getFordontyp();
	

private:
	list<shared_ptr<Ralsfordon>> fordonList;
	list<string> logicList;
	list<string> fordonTyp;
	int tagnummer;
	string ursprung;
	string destination;
	Time ursprungtid;
	Time desttid;
	Time delay;
	STATUS tillstand;
	int maxSpeed;
	double medelH = 0.0;
};











#endif
