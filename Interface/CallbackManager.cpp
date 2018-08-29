#include "Main.h"

extern CClassManager			*pClassManager;

LRESULT CALLBACK CCallbackManager::WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
		// On window create 
		case WM_CREATE:
			// Initialize the interface
			pClassManager->GetInterface()->Initialize(hWnd);
			break;
	
		case WM_SIZE:
			// Set the height and width
			RECT rWindow;
			rWindow.top = rWindow.left = 0;
			rWindow.right = LOWORD(lParam);
			rWindow.bottom = HIWORD(lParam);
			// Update the files info listview
			pClassManager->GetInterface()->GetListView()->SetCoordinates(rWindow);
			break;

		// On window destroy
		case WM_DESTROY:
			PostQuitMessage(0);
			break;

		// Process commands related to menus
		case WM_COMMAND:
			// If we have a client list menu command
			if(wParam >= ID_NEW && wParam <= ID_DELETE)
				pClassManager->GetInterface()->HandleArchiveMenu((int)wParam);

			break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

// Listview process callback
LRESULT CALLBACK CCallbackManager::FilesListViewProcess(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
		// Right click	
		case WM_CONTEXTMENU:
		{
			// Get cursor position
			POINT pointCursor;
			GetCursorPos(&pointCursor);
			// Show our menu
			TrackPopupMenu(pClassManager->GetInterface()->GetMenuManager()->GetFilesListMenu(), TPM_CENTERALIGN, pointCursor.x, 
				pointCursor.y, NULL, pClassManager->GetInterface()->GetListView()->GetHWND(), NULL);

			break;
		}

		// Double click
		case WM_LBUTTONDBLCLK:
		{
			// Make sure there is an item selected
			if(pClassManager->GetInterface()->GetListView()->GetSelectedItem() != -1)
				pClassManager->GetInterface()->Export(false);

			break;
		}

		// Process commands related to menus
		case WM_COMMAND:
		{
			// If we have a client list menu command
			if (wParam >= ID_EXTRACT && wParam <= ID_IMPORT)
				pClassManager->GetInterface()->HandleArchiveMenu((int)wParam);

			break;
		}
	}
	return CallWindowProc(pClassManager->GetInterface()->GetListView()->GetWindowProc(), hWnd, uMsg, wParam, lParam);
}