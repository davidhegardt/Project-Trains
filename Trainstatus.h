//-------------------------------------------------------------------
// Trainstatus.h
// Class for handeling train event functions
// David Hegardt 2016-10-28
//-------------------------------------------------------------------

#ifndef TrainstatusH
#define TrainstatusH
#include "Datafile.h"
#include <memory>
#include <map>

using namespace std;

class Trainstatus {
public:
	Trainstatus();

	~Trainstatus();

	void readInfo();
	/* Read info - function to call subfunctions to read all textfiles
	   pre : files created and availible
	*/

	bool tryAssemble(shared_ptr<Tag> currTrain);
	/* Try assemble - try to assign fordon to train based on list of ingående fordon, return true if assembled
	   inparam : train to assemble
	   pre : train created
	*/

	void incomplete();
	/* Incomplete - handle trains that are not completed
	   pre : tryAssemble function
	*/


	//void assembled();
	/* Assembled - 
	
	*/

	void ready(shared_ptr<Tag> currTrain);
	/* Ready - handle ready state of train
	   pre : Train assembled
	*/

	void running(shared_ptr<Tag> currTrain);
	/* Running - handle running state of train
	pre : Train ready
	*/

	void arrived(shared_ptr<Tag> currTrain);
	/* Arrived - handle arrived state of train
	pre : Train running
	*/

	void finishUp(shared_ptr<Tag> currTrain);
	/* Finishup - set train to finished and leave train at station
	pre : Train arrived
	*/

	/* Get functions*/
	vector<shared_ptr<Tag>> getTagVec() { return  tagVec; }
	vector<shared_ptr<Tag>> getCopyTag() { return tagVec; }
	Trainmap getMap() { return newMap; }
	vector<shared_ptr<Station>> getStations() { return statVec; }

	void findFordon(int pFordonid, int pLoglvl);
	/*	findFordon - function to locate Ralsfordon based on Id
	inparam : id, current log level
	*/

	void makeHist(shared_ptr<Ralsfordon> currTrain);
	/* makeHist - function to store previous location for vehicle
	inparam : pointer to train object
	*/

	void readHist(int id);
	/* readHist - function to read history of where vechicle has been
	inaparam : id of vehicle
	*/

private:
	int counter = 0;
	vector<shared_ptr<Station>> statVec;
	vector<shared_ptr<Tag>> tagVec;
	Trainmap newMap;
	multimap<int, vector<string>> histMap;
};









#endif 
