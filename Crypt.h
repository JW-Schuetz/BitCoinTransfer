#pragma once

#include <cstring>
#include <windows.h>

using namespace std;

class Crypt
{
private:
	BCRYPT_KEY_HANDLE hKey = NULL;
	BCRYPT_ALG_HANDLE provider = NULL;

	void PrintBytes(BYTE *, DWORD);

public:
	// Konstruktor
	Crypt();

	// Destruktor
	~Crypt();

	// Schl�ssel setzen
	void SetKey(string);

	// Verschl�sseln
	void EnCrypt(string);

	// Entschl�sseln
	char* DeCrypt(const char*, size_t);
};