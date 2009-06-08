#include "SaliencyEvents.h"

#include <cassert>

SaliencyEvents::SaliencyEvents(void)
{
}

SaliencyEvents::~SaliencyEvents(void)
{
}

void SaliencyEvents::addEvent(const std::string& eventTypeName, const IndexRangeList& ranges) {
	EventType* eventType = EventType::getEventTypeByString(eventTypeName);
	assert (eventType != NULL); 
	events.push_back(EventPair(eventType, ranges));
}