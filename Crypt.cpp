#include <stdexcept>
#include <cstring>

#include "Crypt.h"

#ifndef NT_SUCCESS
#define NT_SUCCESS(status) (status >= 0)
#endif

using namespace std;

void Crypt::PrintBytes(BYTE *pbPrintData, DWORD cbDataLen)
{
	DWORD dwCount = 0;

	for (dwCount = 0; dwCount < cbDataLen; dwCount++)
	{
		printf("0x%02x, ", pbPrintData[dwCount]);

		if (0 == (dwCount + 1) % 10) putchar('\n');
	}
}

Crypt::Crypt()
{
	// Konstruktion Algorithmus-Provider AES
	NTSTATUS state = BCryptOpenAlgorithmProvider(&provider, BCRYPT_AES_ALGORITHM, NULL, 0L);
	if (!NT_SUCCESS(state)) throw domain_error("CRYPT 1 error!");
}

void Crypt::SetKey(string keyString)
{
	DWORD cbKeyObject = 0;
	DWORD cbData = 0;

	// Calculate the size of the buffer to hold the KeyObject.
	NTSTATUS state = BCryptGetProperty(provider, BCRYPT_OBJECT_LENGTH, (PBYTE)&cbKeyObject,
		sizeof(DWORD), &cbData, 0);
	if (!NT_SUCCESS(state)) throw domain_error("CRYPT 2 error!");

	// Allocate the key object on the heap.
	PBYTE key = (PBYTE)HeapAlloc(GetProcessHeap(), 0, cbKeyObject);
	if (key == NULL) throw domain_error("CRYPT 3 error!");

	// Generate the key from supplied input key bytes.
	state = BCryptGenerateSymmetricKey(provider, &hKey, key, cbKeyObject,
		(PBYTE)keyString.c_str(), keyString.length(), 0);
	if (!NT_SUCCESS(state)) throw domain_error("CRYPT 4 error!");
}

void Crypt::EnCrypt(string toBeEncrypted)
{
	// Get the output buffer size
	DWORD cipherSize = 0;
	DWORD cbData = 0;

	NTSTATUS state = BCryptEncrypt(hKey, (PBYTE)toBeEncrypted.c_str(), toBeEncrypted.length(), NULL, NULL, 0,
		NULL, 0, &cipherSize, BCRYPT_BLOCK_PADDING);
	if (!NT_SUCCESS(state)) throw domain_error("CRYPT 5 error!");

	// Get the output buffer
	PBYTE encrypted = (PBYTE)HeapAlloc(GetProcessHeap(), 0, cipherSize);
	if (NULL == encrypted) throw domain_error("CRYPT 6 error!");

	// Use the key to encrypt the plaintext buffer. For block sized messages, block padding will add an extra block.
	state = BCryptEncrypt(hKey, (PBYTE)toBeEncrypted.c_str(), toBeEncrypted.length(), NULL, NULL,
		0, encrypted, cipherSize, &cbData, BCRYPT_BLOCK_PADDING);
	if (!NT_SUCCESS(state)) throw domain_error("CRYPT 7 error!");

	PrintBytes(encrypted, cipherSize);

	if (encrypted != NULL) HeapFree(GetProcessHeap(), 0, encrypted);
}

char* Crypt::DeCrypt(const char *cipherT, size_t cipherS)
{
	// Get the output buffer size.
	DWORD cipherSize = 0;
	NTSTATUS state = BCryptDecrypt(hKey, (PBYTE)cipherT, cipherS, NULL, NULL,
		0, NULL, 0, &cipherSize, BCRYPT_BLOCK_PADDING);
	if (!NT_SUCCESS(state)) throw domain_error("CRYPT 8 error!");

	PBYTE decrypted = 0;
	decrypted = (PBYTE)HeapAlloc(GetProcessHeap(), 0, cipherSize);
	if (NULL == decrypted) throw domain_error("CRYPT 9 error!");

	state = BCryptDecrypt(hKey, (PBYTE)cipherT, cipherS, NULL, NULL, 0,
		decrypted, cipherSize, &cipherSize, BCRYPT_BLOCK_PADDING);
	if (!NT_SUCCESS(state)) throw domain_error("CRYPT 10 error!");

	char* decryptedChars = new char[cipherSize+1];
	for (int i = 0; i < int(cipherSize); ++i) decryptedChars[i] = (char)decrypted[i];
	decryptedChars[cipherSize] = 0;

	if (decrypted != NULL) HeapFree(GetProcessHeap(), 0, decrypted);

	return decryptedChars;
}

Crypt::~Crypt()
{
	if (provider != NULL) BCryptCloseAlgorithmProvider(&provider, 0L);
}