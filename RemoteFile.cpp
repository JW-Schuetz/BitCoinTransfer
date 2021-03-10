#pragma warning(disable : 4996)

#include <stdexcept>

#include "RemoteFile.h"
#include "BitCoinTransfer.h"

using namespace std;

// Konstruktor
RemoteFile::RemoteFile(const string file, const string credential, long int verbose)
{
	// Buffer konstruieren
	buffer = new GrowableStringBuffer();

	// set verbose mode on/off
	CURLcode res = curl_easy_setopt(GetHandle(), CURLOPT_VERBOSE, verbose);
	if (res != CURLE_OK) throw domain_error("CURL error!");

	// custom string for request
	curl_easy_setopt(GetHandle(), CURLOPT_CUSTOMREQUEST, "MLSD");
	if (res != CURLE_OK) throw domain_error("CURL error!");

	// provide the URL to use in the request
	res = curl_easy_setopt(GetHandle(), CURLOPT_URL, file.c_str());
	if (res != CURLE_OK) throw domain_error("CURL error!");

	// user name and password to use in authentication
	res = curl_easy_setopt(GetHandle(), CURLOPT_USERPWD, credential.c_str());
	if (res != CURLE_OK) throw domain_error("CURL error!");

	// custom pointer passed to the write callback
	res = curl_easy_setopt(GetHandle(), CURLOPT_WRITEDATA, buffer);
	if (res != CURLE_OK) throw domain_error("CURL error!");

	// set callback for writing received data
	res = curl_easy_setopt(GetHandle(), CURLOPT_WRITEFUNCTION, write_callback);
	if (res != CURLE_OK) throw domain_error("CURL error!");
}

// Destruktor
RemoteFile::~RemoteFile()
{
	delete buffer;
}

// File holen
void RemoteFile::GetRemoteFile()
{
	// perform a blocking file transfer
	CURLcode res = curl_easy_perform(GetHandle());
	if (res != CURLE_OK) throw domain_error("CURL error!");

	// buffer enthält keinen String, d.h. die 0-Terminierung fehlt, also 0-Terminierung anfügen
	buffer->Terminate();
	content = buffer->GetContent();
}

// File remote abspeichern
void RemoteFile::SaveRemoteFileToHost(string hostFileName)
{
	FILE *f = fopen(hostFileName.c_str(), "w");
	if (f == NULL) throw domain_error("Error writing host-file!");

	fwrite(content, 1, strlen((char *)content), f);

	int err = fclose(f);

	if (err != 0) throw domain_error("Error closing host-file!");
}