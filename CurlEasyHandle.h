#pragma once

#define CURL_STATICLIB
#include <curl/curl.h>

class CurlEasyHandle
{
private:
	// Handle
	CURL *easy_handle;

public:
	// Konstruktor
	CurlEasyHandle();

	// Destruktor
	~CurlEasyHandle();

	// Handle
	CURL *GetHandle();
};
