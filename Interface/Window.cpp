#include "Main.h"

CWindow::CWindow(char *szWindowClass, char *szWindowTitle, DWORD dwStyle, int iX, int iY, int iW, int iH, CWindow *pParent)
{
	// Create the window
	m_hWnd = CreateWindow(szWindowClass, szWindowTitle, dwStyle, iX, iY, iW, iH, pParent->GetHWND(), NULL, GetModuleHandle(NULL), NULL);
	// Get the window process
	m_hWindowProc = (WNDPROC)GetWindowLong(m_hWnd, GWL_WNDPROC);
	// Reset the show flag
	m_bShow = false;
	// Reset pages(only used in TabControl)
	m_bytePages = 0;
}

CWindow::CWindow(char *szResource, CWindow *pParent)
{
	// Create the dialog from resource
	m_hWnd = CreateDialog((HINSTANCE)GetModuleHandle(NULL), szResource, pParent->GetHWND(), NULL);
	// Get the window process
	m_hWindowProc = (WNDPROC)GetWindowLong(m_hWnd, GWL_WNDPROC);
	// Reset the show flag
	m_bShow = false;
	// Reset pages(only used to TabControl)
	m_bytePages = 0;
}

CWindow::CWindow(HWND hWnd)
{
	// Create the dialog from resource
	m_hWnd = hWnd;
	// Get the window process
	m_hWindowProc = (WNDPROC)GetWindowLong(m_hWnd, GWL_WNDPROC);
	// Reset the show flag
	m_bShow = false;
	// Reset pages(only used to TabControl)
	m_bytePages = 0;
}

CWindow::~CWindow()
{
	// Reset the show flag
	m_bShow = false;
	// Destroy the window
	DestroyWindow(m_hWnd);
}

HWND CWindow::GetHWND()
{ 
	return m_hWnd; 
}

void CWindow::Show(int iCmdShow)
{
	// Set visible state
	ShowWindow(m_hWnd, iCmdShow);
	// Set this window as foreground window
	SetAsForegroundWindow();
	// Set the show flag
	m_bShow = true;
}

void CWindow::Hide()
{
	// Hide the window
	ShowWindow(m_hWnd, SW_HIDE);
	// Reset the show flag
	m_bShow = false;
}

void CWindow::SetCoordinates(RECT rCoords)
{
	// Set the window coordinates
	SetWindowPos(m_hWnd, NULL, rCoords.left, rCoords.top, rCoords.right, rCoords.bottom, SWP_DRAWFRAME);
}

void CWindow::SetCallback(WindowCallback_t WindowCallback)
{
	// Set window callback
	SetWindowLong(m_hWnd, GWL_WNDPROC, (LONG)WindowCallback);
}

void CWindow::SetAsFocusWindow()
{
	SetFocus(m_hWnd);
}

void CWindow::SetAsForegroundWindow()
{
	SetForegroundWindow(m_hWnd);
}

void CWindow::Update()
{
	UpdateWindow(m_hWnd);
}


int CWindow::AddTabControlItem(int iItemId, char *szText)
{
	// Make sure the pages count dosent go over the limit
	if(m_bytePages >= MAX_TABCONTROL_PAGES)
		return -1;

	// Prepare the TCITEM structure
	TCITEM tiItem;
	tiItem.mask = TCIF_TEXT | TCIF_IMAGE; // Mask params
	tiItem.iImage = -1; // No image
	tiItem.pszText = szText;
	// Insert the tab control item
	int iTabId = TabCtrl_InsertItem(m_hWnd, iItemId, &tiItem);
	// Clear the tab list
	m_listTabPage[iTabId].clear();
	// Increase the pages count
	m_bytePages++;
	// Return the page id
	return iTabId;
}

void CWindow::AddTabControlPageElement(int iPageId, HWND hElement)
{
	// Add the element the elements list
	m_listTabPage[iPageId].push_back(hElement);
}

int CWindow::GetTabControlPage()
{
	return TabCtrl_GetCurSel(m_hWnd);
}

void CWindow::OnTabControlChangeSelection(int iPageId)
{
	// Loop throught all pages
	for(BYTE j = 0; j < m_bytePages; j++)
	{
		// Loop throught all the pages elements
		for(std::list<HWND>::iterator i = m_listTabPage[j].begin(); i != m_listTabPage[j].end(); ++i)
		{
			// Make sure the element handle is valid
			if(*i)
			{
				// Show or hide depending on the page id
				if(j == iPageId)
					ShowWindow((*i), SW_SHOW);
				else
					ShowWindow((*i), SW_HIDE);
			}
		}
	}
}
