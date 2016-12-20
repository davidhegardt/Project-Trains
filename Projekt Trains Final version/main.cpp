//-------------------------------------------------------------------
// main.cpp
// 
// David Hegardt 2016-10-28
//-------------------------------------------------------------------

#include "Tag.h"
#include "Station.h"
#include "Trainmap.h"
#include "Datafile.h"
#include "Trainstatus.h"
#include "Simulation.h"
#include "Interface.h"
#include <memory>
#include <list>

int main() {
	setlocale(LC_ALL, "swedish"); // tillåter svenska tecken

	Interface newInterface;
	newInterface.createSim();


	return 0;
}