#pragma once

namespace ferry { namespace util {

class Observer
{
public:
	Observer(void) {}
	~Observer(void) {}

public:
	virtual void update() = 0;
};

}}