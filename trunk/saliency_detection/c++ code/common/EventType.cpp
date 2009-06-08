#include "EventType.h"

EventType::EventType(void)
{
}

EventType::~EventType(void)
{
}

EventType* EventType::getEventTypeByString(const std::string& typeName) {
	for (size_t i = 0; i < allTypes.size(); i++) {
		if (allTypes[i]->typeName == typeName) return allTypes[i];
	}
	return NULL;
}

void EventType::initializeWithDefaultTypes() {

}