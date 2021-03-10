#pragma once

#include "RemoteDirectory.h"
#include "HostDirectory.h"
#include "BitCoinTransfer.h"
#include "Crypt.h"

class ManageDirectories
{
private:
	char * credential;
	HostDirectory *host;
	RemoteDirectory *remote;
	string hostdirectory;

	Crypt crypt = Crypt();
	// Dieser Schlüssseltext ist etwas zu lang, der richtige Schlüssel ist: "Leck mich 2 mal!"
	string key = string("Idiot! Leck mich 2 mal! Du Depp!");
	// AES-verschlüsselt mit dem obigen Schlüssel
	static const size_t arraySize = 32;

	// Directory-Inhalte
	decltype(remote->GetResult()) remotePairs;
	decltype(host->GetResult()) hostPairs;

	// Funktionen
	bool TestRemoteFileOlder(decltype(remotePairs)::iterator, decltype(hostPairs)::iterator);
	void DownloadIt(string);

public:
	// Konstruktor/ Destruktor
	ManageDirectories(string);
	~ManageDirectories();

	// DoIt!
	void DownloadIfNeccessary();
};