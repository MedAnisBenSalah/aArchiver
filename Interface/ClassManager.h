#ifndef CLASSMANAGER_H
#define CLASSMANAGER_H

#include "Interface.h"
#include "Archiver.h"

class CClassManager
{
	public:
		CClassManager();
		~CClassManager();

		bool LoadArchiverModule();
		void DestroyArchiverInstance();

		bool Initialize(); 

		inline HWND GetHWND() { return m_hWnd; };

		inline CInterface		*GetInterface() { return m_pInterface; };
		inline CArchiver		*GetArchiver() { return m_pArchiver; };

	private:
		// Instances
		CArchiver				*m_pArchiver;
		CInterface				*m_pInterface;
		// General variables
		HMODULE					m_hArchiverModule;
		HWND					m_hWnd;

};

#endif