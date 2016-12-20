//-------------------------------------------------------------------
// Trainbuilder.h
// Static class for creation of objects
// David Hegardt 2016-10-28
//-------------------------------------------------------------------

#include "Tag.h"
#include "Station.h"

class Trainbuilder {
public:


	static shared_ptr<Ralsfordon> createFordon(int pId, int pTyp, int pPar1, int pPar2);
	/* Create fordon - retrieves information in order to create fordon
	   inparam : id, typ, param1, param2 for fordon
	   pre : files read and information retrieved
	*/
	static shared_ptr<Tag> createTag(int pTagnummer, string pUrsprung, string pDest, Time pUrsprungtid, Time pDesttid, int pMaxSpeed, Time pDelay, STATUS pTillstand, list<int> pLogiclist);
	/* Create tag - retrieves all information in order to create a train
	   inparam : all information for constructor class for train
	   pre : files read and information retrieved
	*/

	static void printFordonlist(list<shared_ptr<Ralsfordon>>currList, int logglevel);
	/* Print fordon list - retrieve information from train or station and print detailed information
	   print different information based on list
	*/
	static void printASCII(string typ);
	/* Print ASCII
	   Print graphics based on type of fordon
	*/

};
