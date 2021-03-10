#pragma once

#include "CurlEasyHandle.h"
#include "GrowableStringBuffer.h"

using namespace std;

class RemoteFile : public CurlEasyHandle
{
private:
	GrowableStringBuffer *buffer = NULL;
	void *content = NULL;

public:
	// Konstruktor/ Destruktor
	RemoteFile(const string, const string, long int);
	~RemoteFile();

	// File holen
	void GetRemoteFile();

	// File remote abspeichern
	void SaveRemoteFileToHost(string);
};
