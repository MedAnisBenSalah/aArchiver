#ifndef INTERFACE_H
#define INTERFACE_H

//#include "DialogManager.h"
#include "MenuManager.h"
#include "Window.h"
#include "ListView.h"

#define ID_ARCHIVE	1
#define ID_ABOUT	2
/////////////////////////
#define ID_NEW			20
#define ID_OPEN			21
#define ID_EXTRACT		22
#define ID_IMPORT		23
#define ID_EXPORT		24
#define ID_ENCODE		25
#define ID_EXIT			26
/////////////////////////
#define ID_RENAME		30
#define ID_DELETE		31

class CInterface
{
	public:
		CInterface();
		~CInterface();

		bool Initialize(HWND hWnd);
		void UpdateTitle(char *szTitle);
		
		//CDialogManager *GetDialogManager() { return m_pDialogManager; };
		CMenuManager *GetMenuManager() { return m_pMenuManager; };
		
		void Process();

		// Menus handling
		void HandleArchiveMenu(int iItem);
		void HandleEditMenu(int iItem);
		void HandleAboutMenu(int iItem);

		// Menu functions
		void New();
		void Open();
		void Extract();
		void Import();
		void Export(bool bChooseDir);
		void Encode();

		// Utilities
		void DisplayFileInfo(char *szFile);

		// GUI functions
		inline CWindow		*GetMainWindow() { return m_pWindow; };
		inline CListView	*GetListView() { return m_pListWindow; };

	private:
		// General variables
		char			m_szFilePath[MAX_PATH];
		char			m_szFileName[MAX_PATH];
		HIMAGELIST		m_hImageList;
		// Managers classes pointers
		//CDialogManager	*m_pDialogManager;
		CMenuManager	*m_pMenuManager;
		// Windows pointers
		CWindow			*m_pWindow;
		CListView		*m_pListWindow;
};

#endif