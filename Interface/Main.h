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
#include <Commctrl.h>
#include <ShlObj.h>
#include <Shlwapi.h>
#include <MMSystem.h>
// Managers
#include "ClassManager.h"
#include "CallbackManager.h"
#include "MenuManager.h"
// Virtual includes
#include "Archiver.h"
// Interface
#include "Interface.h"
// GUI 
#include "Window.h"
#include "ListView.h"
// Shared
#include "../Shared/Array.h"
// Util
#include "Util.h"

// General definitions
#define NAME "aArchiver"
#define FULL_NAME "aArchiver - Anis Archiver"
#define VERSION "0.1a"
#define EXTENSION ".aaa"

// Macros
#define SAFE_DELETE(ptr) if(ptr) { delete ptr; ptr = NULL; }

// Compressor functions type
typedef CArchiver *(* CreateArchiver_t)();
typedef void (* DestroyArchiver_t)(CArchiver *pInstance);
