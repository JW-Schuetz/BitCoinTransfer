#pragma once

#include <cstring>
#include <stdexcept>

class GrowableBuffer
{
protected:
	void *ptr;
	size_t size;

public:
	// Konstruktor
	GrowableBuffer();

	// Destruktor
	~GrowableBuffer();

	// neuen Inhalt anfügen
	void AddContent(size_t additionalSize, void *src);

	// Inhalt liefern
	void *GetContent();

	// Länge des Inhaltes liefern
	size_t GetSize();
};
