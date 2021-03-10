#pragma once

#include <filesystem>
#include <windows.h>
#include <list>

using namespace std;
using namespace std::filesystem;

class HostDirectory
{
private:
	list<pair <string, SYSTEMTIME>> pairs;

public:
	// Konstruktor
	HostDirectory(string);

	// Destruktor
	~HostDirectory() {};

	// Ergebnis abholen
	list<pair <string, SYSTEMTIME>> GetResult();
};
