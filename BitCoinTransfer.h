#pragma once

// Benötigt das "Cyptographic Provider Development Kit" von Microsoft

constexpr auto FTPSERVER = "ftp://94.46.94.151/homes/Jürgen/Eigene Software/BitCoinPrice/";

// Parser-Token
constexpr auto FILENAME = "BitCoinPrice-";
constexpr auto MODYFY = "modify=";
constexpr auto TYPE = "type=";

// Debugging und Memory-Leaks-Erkennung
const bool VERBOSE = false;
//#define WITH_MEM_LEAK_TESTING

// Externe Funktionen
size_t write_callback(void *, size_t, size_t, void *);