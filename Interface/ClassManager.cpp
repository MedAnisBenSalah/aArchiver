#include "Main.h"

CClassManager::CClassManager()
{
	// Reset pointers
	m_pInterface = NULL;
	m_pArchiver = NULL;
	m_hArchiverModule = NULL;
}

CClassManager::~CClassManager()
{
	// Unload the archiver
	DestroyArchiverInstance();
	// Delete the interface instance
	SAFE_DELETE(m_pInterface);
}

bool CClassManager::LoadArchiverModule()
{
	// Try to load the compressor module
	m_hArchiverModule = LoadLibrary(ARCHIVER_MODULE);
	if(!m_hArchiverModule)
		return false;

	// Get the load function pointer
	CreateArchiver_t pfn_CreateArchiver = NULL;
	pfn_CreateArchiver = (CreateArchiver_t)GetProcAddress(m_hArchiverModule, "CreateArchiver");
	// Validate the function
	if(!pfn_CreateArchiver)
		return false;

	// Call the function and get the archiver pointer
	m_pArchiver = pfn_CreateArchiver();
	return true;
}

void CClassManager::DestroyArchiverInstance()
{
	// Get the unload function pointer
	DestroyArchiver_t pfn_DestroyArchiver = NULL;
	pfn_DestroyArchiver = (DestroyArchiver_t)GetProcAddress(m_hArchiverModule, "DestroyArchiver");
	// Validate the function
	if(!pfn_DestroyArchiver)
		return;

	// Call the destroy function
	pfn_DestroyArchiver(m_pArchiver);
	// Free the archiver module
	FreeLibrary(m_hArchiverModule);
}

bool CClassManager::Initialize()
{
	// Create the interface instance
	m_pInterface = new CInterface();
	if(!m_pInterface)
		return false;

	return true;
}