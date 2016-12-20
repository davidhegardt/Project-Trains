//-------------------------------------------------------------------
// Tag.h
// Train class - implementation
// David Hegardt 2016-10-28
//-------------------------------------------------------------------

#include "Tag.h"

Tag::~Tag() {
	
}

string Tag::getDest() {
	return destination;
}

string Tag::getStart() {
	return ursprung;
}

int Tag::getTagnummer() {
	return tagnummer;
}

void Tag::setDelay(Time pDelay) {

	delay = delay + pDelay;
}

void Tag::changeTime(Time newTime) {
	
	ursprungtid = ursprungtid + newTime;
	desttid = desttid + newTime;
}

void Tag::addFordon(shared_ptr<Ralsfordon> pFordon) {

	fordonList.push_back(pFordon);
}

void Tag::clearList() {
	fordonList.clear();
}

STATUS Tag::getTillstand() {
	return tillstand;
}

void Tag::setTillstand(STATUS ptillstand) {
	tillstand = ptillstand;
}

list<shared_ptr<Ralsfordon>> Tag::getFordonList() {
	return fordonList;
}

list<string> Tag::getLocigList() {
	return logicList;
}

void Tag::remList(string aTyp) {
	list<string>::iterator begin = logicList.begin(), end = logicList.end(), it;
	for (it = begin; it != end; it++) {
		if (aTyp == *it) {
			logicList.erase(it);
			break;
		}
	}
}

list<string> Tag::getFordontyp() {
	return fordonTyp;
}

void Tag::printASCII(string typ) {
	if (typ == "Diesel-lok") {
		cout << "  Oo  ____    " << endl;
		cout << " _\\/__|_|_  " << endl;
		cout << "(=DIESEL_|-- " << endl;
		cout << "//o--=OOO- " << endl;
	}
	if (typ == "El-lok") {
		cout << "     ____    " << endl;
		cout << " __/__|_|_  " << endl;
		cout << "(=ELLOK_|-- " << endl;
		cout << "//o--=OOO- " << endl;
	}
	if (typ == "Open Godsvagn") {
		cout << " _,o+-o,_" << endl;
		cout << "|_______|--" << endl;
		cout << "`O`   `O`" << endl;
	}
	if (typ == "Closed Godsvagn") {
		cout << " _______" << endl;
		cout << "|_______|--" << endl;
		cout << "`O`   `O`" << endl;
	}
	if (typ == "Sittvagn") {
		cout << "_-------_" << endl;
		cout << "||_| |_| |" << endl;
		cout << "|________|--" << endl;
		cout << "`O`   `O`" << endl;
	}
	if (typ == "Sovvagn") {
		cout << "__________" << endl;
		cout << "| SOVVAGN |" << endl;
		cout << "|_________|--" << endl;
		cout << "`O`   `O`" << endl;
	}
}

int Tag::getHastighet() {
	Time avg = getStartTime();
	Time ank = getDestTime();

	Time restid = ank - avg;
	cout << restid;

	return 0;
}

