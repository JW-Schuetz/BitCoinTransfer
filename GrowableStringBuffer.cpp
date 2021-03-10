#include "GrowableStringBuffer.h"

using namespace std;

// String 0-Terminierung anfügen
void GrowableStringBuffer::Terminate()
{
	size_t newSize = size + 1;
	char *p = (char *)realloc(ptr, newSize);
	if (p == NULL) throw domain_error("No more memory available!");

	// 0-Terminierung
	*(p + size) = 0;

	ptr = (void *)p;
	size = newSize;
}