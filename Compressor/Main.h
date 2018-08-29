// Indicate what system we are compiling on
#ifdef SYSTEM_XP
#	define WINVER 0x05010000
#elif SYSTEM_VISTA 
#	define WINVER 0x06000000
#elif SYSTEM_7
#	define WINVER 0x06010000
#else
#	define WINVER 0x05010000
#endif

// Standard includes
#include <windows.h>
#include <stdio.h>
#include <Shlwapi.h>
// Compress includes
#include "Archiver.h"
// Shared
#include "../Shared/Array.h"
#include "../Shared/Encoder.h"
// Util
#include "Util.h"

// Macros
#define SAFE_DELETE(ptr) if(ptr) { delete ptr; ptr = NULL; }

// General
#define VERSION 1