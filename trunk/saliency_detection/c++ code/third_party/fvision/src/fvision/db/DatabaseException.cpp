#include "DatabaseException.h"

using namespace std;

DatabaseException::DatabaseException(const string& errorMessage) throw() {
	this->errorMessage = errorMessage;
}

DatabaseException::~DatabaseException(void) throw() {
}

