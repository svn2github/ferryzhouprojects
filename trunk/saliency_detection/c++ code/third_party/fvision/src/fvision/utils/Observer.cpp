#include "Observer.h"

namespace fvision {


Observable::~Observable() {
	for (int i = 0; i < (int)observers.size(); i++) {
		delete observers[i];
	}
}

void Observable::addObserver(Observer* observer) {
	observers.push_back(observer);
}

void Observable::notify() {
	for (int i = 0; i < (int)observers.size(); i++) {
		observers[i]->update(this);
	}
}

}