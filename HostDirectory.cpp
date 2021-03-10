#include "HostDirectory.h"
#include "BitCoinTransfer.h"

#pragma warning(disable : 4996)

// Konstruktor
HostDirectory::HostDirectory(string hostdirectory)
{
	path p = path(hostdirectory.c_str());

	for (directory_iterator next(p), end; next != end; ++next)
	{
		directory_entry d = *next;
		auto *fileName = d.path().c_str();
		file_time_type ftime = last_write_time(d);

		FILETIME ft;
		memcpy(&ft, &ftime, sizeof(FILETIME));

		// Struktur mit 0 initialisieren
		SYSTEMTIME st = {};

		if (FileTimeToSystemTime(&ft, &st))
		{
			// werden im Server nicht gefüllt
			st.wDayOfWeek = 0;
			st.wMilliseconds = 0;

			// Länge der Dateinamen: BitCoinPrice-dd.mm.yyyy
			// Anzahl benötigter Bytes bestimmen
			size_t size = wcstombs(NULL, fileName, 1);

			// Platz allokieren
			char *fn = (char *)malloc(size + 1);
			if (fn == NULL) throw domain_error("No more memory available!");
			wcstombs(fn, fileName, size);
			fn[size] = 0;

			// nur die "BitCoinPrice-" Dateien
			char *pfn = strstr(fn, FILENAME);
			if (pfn != NULL)
				pairs.push_back(pair <string, SYSTEMTIME>(pfn, st));

			free(fn);
		}
	}
}

list<pair <string, SYSTEMTIME> > HostDirectory::GetResult()
{
	return pairs;
}