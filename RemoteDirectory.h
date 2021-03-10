#pragma once

#include <windows.h>
#include <list>

#include "CurlEasyHandle.h"
#include "GrowableStringBuffer.h"

using namespace std;

class RemoteDirectory : public CurlEasyHandle
{
private:
	char *content = NULL;
	GrowableStringBuffer *buffer = NULL;
	list<pair <string, SYSTEMTIME>> pairs;

	// Contentparser
	void ParseContent();

public:
	// Konstruktor/ Destruktor
	RemoteDirectory(const string, const string, long int);
	~RemoteDirectory();

	// Ergebnis abholen
	list<pair <string, SYSTEMTIME>> GetResult();
};
