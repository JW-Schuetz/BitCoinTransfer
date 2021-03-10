#include <iostream>
#include <stdexcept>

#include "CurlEasyHandle.h"
#include "ManageDirectories.h"
#include "Crypt.h"

#ifdef WITH_MEM_LEAK_TESTING
	#define _CRTDBG_MAP_ALLOC
	#include <stdlib.h>
	#include <crtdbg.h>
#endif

using namespace std;

// callback wird benutzt von: RemoteDirectory, RemoteFile
size_t write_callback(void* buffer, size_t size, size_t nmemb, void* userp)
{
	// Anzahl der neuen Bytes
	size_t realsize = size * nmemb;

	// Zieldatenspeicher holen und neue Inhalte anfügen
	GrowableBuffer* b = (GrowableBuffer*)userp;
	b->AddContent(realsize, buffer);

	return realsize;
}

// Benutzung: BitCoinTransfer FTP-Passwort Zielverzeichnis
int main(int argc, const char *argv[])
{
	if (argc != 2) return -1;

	// LibCurl initialisieren
	curl_global_init(CURL_GLOBAL_DEFAULT);

	int ret = 0;

	try
	{
		ManageDirectories *manager = new ManageDirectories(argv[1]);
		manager->DownloadIfNeccessary();

		delete manager;
	}
	catch (exception& e)
	{
		ret = -2;

		cerr << "Caught: " << e.what() << endl;
		cerr << "Type: " << typeid(e).name() << endl;
	};

	curl_global_cleanup();

#ifdef WITH_MEM_LEAK_TESTING
	_CrtDumpMemoryLeaks();
#endif

	return ret;
}