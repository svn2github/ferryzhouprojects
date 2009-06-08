#pragma once

class Feature {
public:
	virtual ~Feature(void);
	virtual double computeDistance(const Feature* rhs) = 0;
	virtual Feature* clone() const = 0;
};
