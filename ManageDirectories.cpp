#include "ManageDirectories.h"
#include "RemoteFile.h"
#include "BitCoinTransfer.h"
#include "Crypt.h"

// Konstruktor
ManageDirectories::ManageDirectories(string hostdir)
{
	if (*(hostdir.end() - 1) != '\\') hostdir = hostdir + string("\\");
	hostdirectory = hostdir;

	int tmpArray[arraySize] = { 0x90, 0x47, 0xca, 0x00, 0xc8, 0x44, 0xac, 0xdd, 0xcd, 0x3e,
	   0x9d, 0x83, 0x81, 0x04, 0xef, 0x12, 0xcb, 0xf4, 0x3c, 0x19,
	   0x59, 0xb5, 0xe6, 0xd1, 0x86, 0x8d, 0x66, 0x96, 0x73, 0x18,
	   0x86, 0xe0 };

	char* credent = new char[arraySize+1];
	for (int i = 0; i < arraySize; ++i) credent[i] = (char)tmpArray[i];
	credent[arraySize] = 0;

	crypt.SetKey(key.substr(7, arraySize >> 1));
	credential = crypt.DeCrypt(credent, arraySize);

	delete [] credent;

	// Der folgende auskommentierte Code dient zum Ermitteln der verschlüsselten Credentials. Ausgabe auf der Konsole.
	// Auskommentierung entfernen und User:Passwort entsprechend ersetzen!
	//string s("User:Passwort");
	//crypt.EnCrypt(s);

	remote = new RemoteDirectory(FTPSERVER, credential, 0L);
	host = new HostDirectory(hostdirectory);

	remotePairs = remote->GetResult();
	hostPairs = host->GetResult();
}

// Destruktor
ManageDirectories::~ManageDirectories()
{
	delete remote;
	delete host; 
	delete credential;
}

bool ManageDirectories::TestRemoteFileOlder(decltype(remotePairs)::iterator s, decltype(hostPairs)::iterator h)
{
	bool remoteOlder = false;

	// compare modify times
	SYSTEMTIME remote = s->second;
	SYSTEMTIME host = h->second;

	FILETIME ft;
	ULARGE_INTEGER uliRemote = {};
	ULARGE_INTEGER uliHost = {};

	if (SystemTimeToFileTime(&remote, &ft))
	{
		uliRemote.HighPart = ft.dwHighDateTime;
		uliRemote.LowPart = ft.dwLowDateTime;

		if (SystemTimeToFileTime(&host, &ft))
		{
			uliHost.HighPart = ft.dwHighDateTime;
			uliHost.LowPart = ft.dwLowDateTime;
		}
	}

	// QuadParts grow with time-distance to a fixed time in the very past.
	// Newer files have a bigger QuadPart, older files a smaller one.
	// remote: 15:39:37   host: 15.17:39
	if (uliRemote.QuadPart <= uliHost.QuadPart) remoteOlder = true;

	return remoteOlder;
}

void ManageDirectories::DownloadIfNeccessary()
{
	// für jedes Remotefile
	for (auto r = remotePairs.begin(); r != remotePairs.end(); ++r)
	{
		bool hit = false;
		// für jedes Hostfile
		for (auto h = hostPairs.begin(); h != hostPairs.end(); ++h)
		{
			bool equal = (r->first == h->first);
			if (equal)
			{
				// compare modify times
				bool fileOlder = TestRemoteFileOlder(r, h);
				// nur downloaden wenn remoteFile neuer ist als hostFile
				if (!fileOlder)	DownloadIt(r->first);
				hit = true;
				break;
			}
		}

		// s nicht in h gefunden: downloaden
		if (!hit) DownloadIt(r->first);
	}
}

void ManageDirectories::DownloadIt(string s)
{
	// File downloaden
	string remoteFileName = FTPSERVER + s;
	RemoteFile remoteFile(remoteFileName, string(credential), VERBOSE);
	remoteFile.GetRemoteFile();

	// File remote abspeichern
	string hostFileName = hostdirectory + s;
	remoteFile.SaveRemoteFileToHost(hostFileName);
}