#pragma once

#include <vector>

#include "Observer.h"

using namespace std;

namespace ferry { namespace util {

class Observable
{
private:
	vector<Observer*> observers;

public:
	Observable(void) {}
	~Observable(void) {}

public:
	void addObserver(Observer* po) {
		observers.push_back(po);
	}

	void notifyObservers() {
		for (int i = 0; i < (int)observers.size(); i++) {
			observers[i]->update();
		}
	}

};

}}