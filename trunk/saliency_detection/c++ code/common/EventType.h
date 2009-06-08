#pragma once

#include <string>
#include <vector>

class EventType {
private:
	EventType(void);
	~EventType(void);

public:
	static EventType* getEventTypeByString(const std::string& typeName);
	static std::vector<EventType*> getAllTypes() { return allTypes; }

	static void initializeWithDefaultTypes();

private:
	static std::vector<EventType*> allTypes;

private:
	std::string typeName;
	std::string typeDiscriptions;
};
