#pragma once

#include <exception>
#include <string>

class DatabaseException : public std::exception {
public:
	DatabaseException(const std::string& errorMessage) throw();

	virtual ~DatabaseException(void) throw();

public:
	virtual const char* what() const throw() {
		return errorMessage.c_str();
	}

private:
	std::string errorMessage;
};

