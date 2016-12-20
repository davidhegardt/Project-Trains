//-------------------------------------------------------------------
// Interface.h
// Class for handeling user interface and printout
// David Hegardt 2016-10-28
//-------------------------------------------------------------------

#ifndef InterfaceH
#define InterfaceH

#include "Trainstatus.h"
#include "Event.h"



class Simulation;

class Interface {
public:
	void showMenu(int level);
	/* Show menu items based on lavel
	   inparam : level
	*/

	void getMenuOption(int val);
	/* Check user input for menu option
	   switch correct value based on level
	*/

	~Interface();

	void createSim();
	/* Create simulation, runs first before file load
	
	*/
	void runSim();
	/* Run sim - func to start simulation after sim time has been set and files read
	   pre : data has been read
	*/

	void changeInterval();
	/* Change ineterval - change the current intervall time
	   pre : simulation is running
	*/

	void nextEvent();
	/* Next event - function to run to the next event in simulation
	   pre : simulation is running
	*/

	void findFordon();
	/* Find fordon - function to search and find fordon
	   pre : files have been read, objects created
	*/
	void timeTable();
	/* Time Table - print timetable for trains
	   pre : files have been read, objects created
	*/
	void searchTrains();
	/* Search trains - function to search for trains
	   pre : files read
	*/

	void searchStation();
	/* Search stations - function to search for spec. train
	   pre : stations created
	*/

	void showAllStations();
	/* Show all stations - print list of stations names
	   pre : files read
	*/

	void changeLogLevel();
	/* Change log level - function to change how much info is printed
		
	*/

	void changeSimStart();
	/* Change sim start - change start time of simulation
	   pre : Files read
	*/

	void changeSimEnd();
	/* Change sim end - change end time of simulation
	pre : Files read
	*/

	
private:
	Simulation *theSim;
	vector<shared_ptr<Tag>> tagVec;
	vector<shared_ptr<Station>> statVec;
	Trainstatus *status;
	int theLevel;
	int intervall;
	int sim_end = SIM_TIME;
	int loglevel = 1;
	bool Eventtime = false;
	Time starTim;
	Time endTim;
	string currTyp = "intervalltid";
	bool simFinished = false;
};















#endif
