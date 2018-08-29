#include "Main.h"

CMenuManager::CMenuManager(HWND hWnd)
{
	// Create main menu
	HMENU hMainMenu = CreateMenu();
	// Insert control menu items
	/*InsertMenu(hMainMenu, 1, MF_BYPOSITION, (UINT)ID_ARCHIVE, "&Archive");
	InsertMenu(hMainMenu, 2, MF_BYPOSITION, (UINT)ID_EDIT, "&Edit");
	InsertMenu(hMainMenu, 3, MF_BYPOSITION, (UINT)ID_ABOUT, "&About");*/

	// Create the about menu
	/*m_hAboutMenu = CreatePopupMenu();
	// Create the archive menu
	InsertMenu(m_hAboutMenu, 1, MF_BYPOSITION, (UINT)ID_ABOUT, "&About");*/

	// Create the archive menu
	m_hArchiveMenu = CreatePopupMenu();
	// Create the archive menu
	InsertMenu(m_hArchiveMenu, 1, MF_BYPOSITION, (UINT)ID_NEW, "&New");
	InsertMenu(m_hArchiveMenu, 2, MF_BYPOSITION, (UINT)ID_OPEN, "&Open");
	InsertMenu(m_hArchiveMenu, 3, MF_BYPOSITION, (UINT)ID_EXTRACT, "&Extract");
	InsertMenu(m_hArchiveMenu, 4, MF_SEPARATOR, NULL, "");
	InsertMenu(m_hArchiveMenu, 5, MF_BYPOSITION, (UINT)ID_IMPORT, "&Import");
	InsertMenu(m_hArchiveMenu, 6, MF_BYPOSITION, (UINT)ID_EXPORT, "&Export");
	InsertMenu(m_hArchiveMenu, 7, MF_SEPARATOR, NULL, "");
	InsertMenu(m_hArchiveMenu, 8, MF_BYPOSITION, (UINT)ID_ENCODE, "&Encode");
	InsertMenu(m_hArchiveMenu, 9, MF_SEPARATOR, NULL, "");
	InsertMenu(m_hArchiveMenu, 10, MF_BYPOSITION, (UINT)ID_EXIT, "&Exit");
	// Insert it into the main menu
	InsertMenu(hMainMenu, 1, MF_POPUP, (UINT)m_hArchiveMenu, "&Archive");
	//InsertMenu(hMainMenu, 2, MF_POPUP, (UINT)m_hAboutMenu, "&About");
	// Set the main menu
	SetMenu(hWnd, hMainMenu);

	// Create the listview menu
	m_hFilesListMenu = CreatePopupMenu();
	// Insert menu items
	InsertMenu(m_hFilesListMenu, 1, MF_BYPOSITION, (UINT)ID_EXTRACT, "Extract");
	InsertMenu(m_hFilesListMenu, 2, MF_SEPARATOR, NULL, "");
	InsertMenu(m_hFilesListMenu, 3, MF_BYPOSITION, (UINT)ID_EXPORT, "Export");
	InsertMenu(m_hFilesListMenu, 4, MF_SEPARATOR, NULL, "");
	/*InsertMenu(m_hFilesListMenu, 6, MF_BYPOSITION, (UINT)ID_RENAME, "Rename");
	InsertMenu(m_hFilesListMenu, 7, MF_BYPOSITION, (UINT)ID_DELETE, "Delete");*/
}

CMenuManager::~CMenuManager()
{
}