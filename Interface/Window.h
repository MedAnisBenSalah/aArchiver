/* =========================================================
			MRAT - http://www.hackforums.com

-- File: Window.h
-- Project: Client
-- Author(s): m0niSx
=============================================================*/

#ifndef WINDOW_H
#define WINDOW_H

#include <iostream>
#include <list>

typedef LRESULT (CALLBACK *WindowCallback_t)(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

#define MAX_TABCONTROL_PAGES 20

class CWindow
{
	public:
		CWindow(char *szWindowClass, char *szWindowTitle, DWORD dwStyle, int iX, int iY, int iW, int iH, CWindow *pParent);
		CWindow(char *szResource, CWindow *pParent);
		CWindow(HWND hWnd);
		~CWindow();

		virtual HWND GetHWND();
		virtual WNDPROC GetWindowProc() { return m_hWindowProc; };
		
		virtual void SetCallback(WindowCallback_t WindowCallback);
		
		virtual void Show(int iCmdShow);
		virtual void Hide();
		virtual void SetCoordinates(RECT rCoords);
		virtual void SetAsFocusWindow();
		virtual void SetAsForegroundWindow();
		virtual void Update();

		void GetCoordinates(RECT *pRect) { GetWindowRect(m_hWnd, pRect); };

		template <typename T>
		T SendWindowMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
		{
			return (T)(SendMessage(m_hWnd, uMsg, wParam, lParam));
		};

		virtual int _SendWindowMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
		{
			return SendWindowMessage<int>(uMsg, wParam, lParam);
		};

		virtual bool IsWindowShowen() { return m_bShow; };

		// TabControl controls
		int AddTabControlItem(int iItemId, char *szText);
		int GetTabControlPage();
		void AddTabControlPageElement(int iPageId, HWND hElement);
		void OnTabControlChangeSelection(int iPageId);



	private:
		HWND						m_hWnd;
		WNDPROC						m_hWindowProc;
		bool						m_bShow;
		BYTE						m_bytePages;
		std::list<HWND>				m_listTabPage[MAX_TABCONTROL_PAGES];

};

#endif