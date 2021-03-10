#include "CurlEasyHandle.h"

// Konstruktor
CurlEasyHandle::CurlEasyHandle()
{
	easy_handle = curl_easy_init();
}

// Destruktor
CurlEasyHandle::~CurlEasyHandle()
{
	curl_easy_cleanup(easy_handle);
}

// Liefert das Handle
CURL *CurlEasyHandle::GetHandle()
{
	return easy_handle;
}