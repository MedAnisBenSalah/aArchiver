#ifndef CALLBACKMANAGER_H
#define CALLBACKMANAGER_H

#include "Interface.h"

class CCallbackManager
{
	public:
		static LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
		static LRESULT CALLBACK FilesListViewProcess(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};

#endif