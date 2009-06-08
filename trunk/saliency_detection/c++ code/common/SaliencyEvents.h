#pragma once

#include "IndexRangeList.h"
#include "EventType.h"

#include <vector>
#include <string>

class SaliencyEvents {
public:
	SaliencyEvents(void);
	~SaliencyEvents(void);

public:
	void setVideoRange(const IndexRange& range) { this->videoRange = range; }

	void addEvent(const std::string& eventTypeName, const IndexRangeList& ranges);

	IndexRange getVideoRange() const { return videoRange; }

	typedef std::pair<EventType*, IndexRangeList> EventPair;
	typedef std::vector<EventPair> EventList;

	const EventList& getEventList() const { return events; }
	
private:
	IndexRange videoRange;
	EventList events;
};
