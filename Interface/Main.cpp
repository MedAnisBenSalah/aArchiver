#include "Main.h"

CClassManager			*pClassManager;

// So we can use the Win7 controls style instead of the old ugly Win95 style(default)
#pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

// Startup callback for Win32 applications
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// Create the class manager
	pClassManager = new CClassManager();
	// Validate the class manager
	if(!pClassManager)
	{
		MessageBox(NULL, "Failed to initialize " NAME ".", "Error", MB_OK | MB_ICONERROR);
		return 0;
	}
	// Initialize the class manager
	if(!pClassManager->Initialize())
	{
		MessageBox(NULL, "Failed to initialize compnenets.", "Error", MB_OK | MB_ICONERROR);
		return 0;
	}
	// Load the archiver
	if(!pClassManager->LoadArchiverModule())
	{
		MessageBox(NULL, "Failed to load the " NAME ".", "Error", MB_OK | MB_ICONERROR);
		return 0;
	}
	HWND hWnd; 
	// Init dialog common controls
	INITCOMMONCONTROLSEX InitCtrlEx;
	InitCtrlEx.dwSize = sizeof(INITCOMMONCONTROLSEX);
	InitCtrlEx.dwICC  = ICC_BAR_CLASSES;
	InitCommonControlsEx(&InitCtrlEx);
	// Clear the WNDCLASSEX buffer
	WNDCLASSEX wc; 
	ZeroMemory(&wc, sizeof(WNDCLASSEX));
	// Set the window class values to default
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = (WNDPROC)CCallbackManager::WindowProc;
    wc.hInstance = hInstance;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);
    wc.lpszClassName = NAME;
	// Register the window
    RegisterClassEx(&wc);
	// Create the window
	hWnd = CreateWindowEx(NULL, NAME, FULL_NAME " v" VERSION "", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 1024, 700,
		NULL, NULL, hInstance, NULL);

	// Show the window
	ShowWindow(hWnd, nCmdShow);
	// Update the window
	UpdateWindow(hWnd);
	// Show the mouse
	ShowCursor(TRUE);
	// Loop until we quit
	MSG Msg;
	while(true)
    {
        // Check for incoming messages
		if(PeekMessage(&Msg, NULL, 0, 0, PM_REMOVE))
        {
            // Should we quit ?
			if(Msg.message == WM_QUIT)
                break;

			// Translate & Dispatch the message           
			TranslateMessage(&Msg);
            DispatchMessage(&Msg);
		}
		// Sleep 5ms
		Sleep(5);
	}
	// Delete the class manager
	SAFE_DELETE(pClassManager);
	return 0;
}
