#include <stdexcept>
#include "GrowableBuffer.h"

using namespace std;

// Konstruktor
GrowableBuffer::GrowableBuffer()
{
	size = 0;
	ptr = malloc(1);
	if (ptr == NULL) throw domain_error("No more memory available!");
}

// Destruktor
GrowableBuffer::~GrowableBuffer()
{
	free(ptr);
}

// neuen Inhalt anfügen
void GrowableBuffer::AddContent(size_t additionalSize, void *src)
{
	// mehr Platz bereitstellen
	size_t newSize = size + additionalSize;
	char *p = (char *)realloc(ptr, newSize);
	if (p == NULL) throw domain_error("No more memory available!");

	// neue Bytes anfügen, mit Nullterminierung
	memcpy_s(p + size, additionalSize, src, additionalSize);

	size = newSize;
	ptr = (void *)p;
}

// Inhalt liefern
void *GrowableBuffer::GetContent()
{
	return ptr;
}

// Länge des Inhaltes liefern
size_t GrowableBuffer::GetSize()
{
	return size;
}