#ifndef MENUMANAGER_H
#define MENUMANAGER_H

class CMenuManager
{
	public:
		CMenuManager(HWND hWnd);
		~CMenuManager();

		inline HMENU GetFilesListMenu() { return m_hFilesListMenu; };

		inline HMENU GetArchiveMenu() { return m_hArchiveMenu; };
		inline HMENU GetAboutMenu() { return m_hAboutMenu; };

	private:
		HMENU		m_hArchiveMenu;
		HMENU		m_hAboutMenu;

		HMENU		m_hFilesListMenu;
};

#endif
