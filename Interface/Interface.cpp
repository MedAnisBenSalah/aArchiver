#include "main.h"

extern CClassManager	*pClassManager;

CInterface::CInterface()
{
	// Reset managers pointers
	//m_pDialogManager = NULL;
	m_pMenuManager = NULL;
	// Reset windows pointers
	m_pListWindow = NULL;
	m_pWindow = NULL;
	m_hImageList = NULL;
	// Reset the file name name and path strings
	strcpy(m_szFilePath, "");
	strcpy(m_szFileName, "");
}

CInterface::~CInterface()
{
	// Delete managers instances
	//SAFE_DELETE(m_pDialogManager);
	SAFE_DELETE(m_pMenuManager);
	// Delete windows instances
	SAFE_DELETE(m_pListWindow);
}

bool CInterface::Initialize(HWND hWnd)
{
	// Create the main window instance
	m_pWindow = new CWindow(hWnd);
	// Get the main window coordinates
	RECT rWindow;
	m_pWindow->GetCoordinates(&rWindow);
	// Calculate the listview limits
	int iWidth = rWindow.right - rWindow.left;
	int iHeight = rWindow.bottom - rWindow.top;
	// Create clients listview
	m_pListWindow = new CListView(WS_CHILD | WS_VISIBLE | LVS_REPORT | WS_DLGFRAME | LBS_NOTIFY, 0, 0, iWidth, iHeight, m_pWindow);
	// Set listview style
	ListView_SetExtendedListViewStyle(m_pListWindow->GetHWND(), LVS_EX_FULLROWSELECT | LVS_EX_SUBITEMIMAGES);
	// Change the listview window process callabck
	m_pListWindow->SetCallback(CCallbackManager::FilesListViewProcess);
	// Add listview columns
	m_pListWindow->AddColumn(0, "Name", 0xE0, 0);
	m_pListWindow->AddColumn(1, "Type", 0xE0, 0);
	m_pListWindow->AddColumn(2, "Size", 0xA0, 0);
	m_pListWindow->AddColumn(3, "Encoded", 0xA0, 0);
	m_pListWindow->AddColumn(4, "Created", 0x70, 0);
	m_pListWindow->AddColumn(5, "Last Modified", 0x70, 0);
	// Create ImageList
	m_hImageList = ImageList_Create(16, 16, ILC_COLOR32, 0, 1);
    // Associate the image list with the list-view control. 
	ListView_SetImageList(m_pListWindow->GetHWND(), m_hImageList, LVSIL_SMALL); 
	// Create managers instances
	m_pMenuManager = new CMenuManager(hWnd);

	/*m_pDialogManager = new CDialogManager(m_pWindow);
	// Setup dialog manager
	m_pDialogManager->Setup();*/
	return true;
}

void CInterface::UpdateTitle(char *szTitle)
{
	// Generate the title text
	char szNewTitle[256];
	sprintf(szNewTitle, FULL_NAME " v" VERSION " - %s", szTitle);
	// Set the main window title
	SetWindowText(m_pWindow->GetHWND(), szNewTitle);
}

void CInterface::Process()
{

}

void CInterface::DisplayFileInfo(char *szFile)
{
	// Clear the files listview
	pClassManager->GetInterface()->GetListView()->DeleteAllItems();
	// Clear the file icons
	ImageList_SetImageCount(m_hImageList, 0);
	// Get the files informations array
	CArray *pArray = pClassManager->GetArchiver()->GetArchivedFilesInfo(szFile);
	// Validate the array
	if(!pArray)
		return;

	// Keep reading until we finish all the array
	size_t sLength = 0, sSize = 0;
	while(pArray->GetReadPointer() < pArray->GetSize())
	{
		// Get the name length
		pArray->Pop(sLength);
		// Read the file name
		char szFileName[MAX_PATH];
		pArray->Pop((unsigned char *)szFileName, sLength);
		// Add the EOS
		szFileName[sLength] = 0;
		// Get the icon length
		DWORD dwIconLength = 0;
		pArray->Pop(dwIconLength);
		// Read the icon buffer
		BYTE *pbyteIconBuffer = (BYTE *)malloc(dwIconLength);
		pArray->Pop(pbyteIconBuffer, dwIconLength);
		pbyteIconBuffer[dwIconLength] = 0;
		// Create the bitmapinfo struct
		BITMAPINFO bmpInfo;
		memcpy(&bmpInfo, pbyteIconBuffer, sizeof(BITMAPINFO));
		// Create the icon HBITMAP
		HBITMAP hIcon = CreateDIBitmap(GetDC(NULL), &bmpInfo.bmiHeader, CBM_INIT, pbyteIconBuffer + sizeof(BITMAPINFO), &bmpInfo, DIB_RGB_COLORS);
		// Add the file icon to the files imagelist
		int iImage = ImageList_Add(m_hImageList, hIcon, (HBITMAP)NULL);
		// Add the file name to the listview
		int iItem = pClassManager->GetInterface()->GetListView()->AddItem(szFileName, iImage);
		// Get the type length
		DWORD dwTypeLength = 0;
		pArray->Pop(dwTypeLength);
		// Read the file type
		char *szFileType = (char *)malloc(dwTypeLength);
		pArray->Pop((unsigned char *)szFileType, dwTypeLength);
		szFileType[dwTypeLength] = 0;
		// Add the file type to the listview
		pClassManager->GetInterface()->GetListView()->AddSubItem(iItem, 1, szFileType);
		// Get the creation and last modified time
		SYSTEMTIME lastModifiedTime, creationTime;
		pArray->Pop((unsigned char *)&creationTime, sizeof(SYSTEMTIME));
		pArray->Pop((unsigned char *)&lastModifiedTime, sizeof(SYSTEMTIME));
		// Get the file size
		pArray->Pop(sSize);
		// Convert the file size to string
		char szFileSize[5];
		sprintf(szFileSize, "%d kb", CUtil::Round((float)sSize / 1024) < 1 ? 1 : CUtil::Round((float)sSize / 1024));
		// Add the file size to the listview
		pClassManager->GetInterface()->GetListView()->AddSubItem(iItem, 2, szFileSize);
		pClassManager->GetInterface()->GetListView()->AddSubItem(iItem, 3, "No");
		// Convert last modified to string
		char szDate[11];
		sprintf(szDate, "%d/%d/%d", lastModifiedTime.wDay, lastModifiedTime.wMonth, lastModifiedTime.wYear);
		// Add the creation date
		pClassManager->GetInterface()->GetListView()->AddSubItem(iItem, 4, szDate);
		// Convert creation to string
		sprintf(szDate, "%d/%d/%d", creationTime.wDay, creationTime.wMonth, creationTime.wYear);
		// Add the creation date
		pClassManager->GetInterface()->GetListView()->AddSubItem(iItem, 5, szDate);
	}
}

void CInterface::HandleArchiveMenu(int iItem)
{
	switch(iItem)
	{
		// New
		case ID_NEW:
			New();
			break;

		// Open 
		case ID_OPEN:
			Open();
			break;

		// Extract
		case ID_EXTRACT:
			Extract();
			break;

		// Imprort
		case ID_IMPORT:
			Import();
			break;

		// Export
		case ID_EXPORT:
			Export(true);
			break;

		// Encode
		case ID_ENCODE:
			Encode();
			break;

		// Exit
		case ID_EXIT:
			m_pWindow->SendWindowMessage<void>(WM_CLOSE, NULL, NULL);
			break;
	}
}

void CInterface::New()
{
	char szFile[MAX_PATH];
	char szFileName[MAX_PATH];
	// Initialize OPENFILENAME structure
	OPENFILENAME opFile;
	ZeroMemory(&opFile, sizeof(opFile));
	// Set the structure members
	opFile.lStructSize = sizeof(opFile);
	opFile.hwndOwner = m_pWindow->GetHWND();
	opFile.lpstrFile = szFile;
	opFile.lpstrFile[0] = '\0';
	opFile.nMaxFile = sizeof(szFile);
	opFile.lpstrFilter = NAME "\0*" EXTENSION "\0";
	opFile.nFilterIndex = 1;
	opFile.lpstrFileTitle = szFileName;
	opFile.nMaxFileTitle = sizeof(szFileName);
	opFile.lpstrInitialDir = NULL;
	opFile.lpstrTitle = "Create an " NAME " archive";
	opFile.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	// Get the choosen file name
	BOOL bOpen = GetSaveFileName(&opFile);
	if(!bOpen)
		return;

	// Add the file extension
	strcat(szFile, EXTENSION);
	strcat(szFileName, EXTENSION);
	// Create the archive
	/*FILE *pFile = fopen(szFile, "wb");
	if(!pFile)
	{
		MessageBox(NULL, "Failed to create archive !", "Error", MB_OK | MB_ICONERROR);
		return;
	}
	// Close the file
	fclose(pFile);*/
	// Prepare the archive
	if (!pClassManager->GetArchiver()->PrepareArchive(szFile, 0, false))
	{
		MessageBox(NULL, "Failed to initialize archive !", "Error", MB_OK | MB_ICONERROR);
		return;
	}
	// Save the archive file path and name
	strcpy(m_szFileName, szFileName);
	strcpy(m_szFilePath, szFile);
	// Update the window title
	UpdateTitle(szFileName);
}

void CInterface::Open()
{
	char szFile[MAX_PATH];
	char szFileName[MAX_PATH];
	// Initialize OPENFILENAME structure
	OPENFILENAME opFile;
	ZeroMemory(&opFile, sizeof(opFile));
	// Set the structure members
	opFile.lStructSize = sizeof(opFile);
	opFile.hwndOwner = m_pWindow->GetHWND();
	opFile.lpstrFile = szFile;
	opFile.lpstrFile[0] = '\0';
	opFile.nMaxFile = sizeof(szFile);
	opFile.lpstrFilter = NAME " Archive\0*" EXTENSION "\0";
	opFile.nFilterIndex = 1;
	opFile.lpstrFileTitle = szFileName;
	opFile.nMaxFileTitle = sizeof(szFileName);
	opFile.lpstrInitialDir = NULL;
	opFile.lpstrTitle = "Open an " NAME " archive";
	opFile.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	// Get the choosen file name
	BOOL bOpen = GetOpenFileName(&opFile);
	if(!bOpen)
		return;

	// Display the files informations
	DisplayFileInfo(szFile);
	// Save the decompressed file path and name
	strcpy(m_szFileName, szFileName);
	strcpy(m_szFilePath, szFile);
	// Update the window title
	UpdateTitle(szFileName);
}

void CInterface::Extract()
{
	// Initialize BROWSEINFO structure
	BROWSEINFO biDirectory;
	ZeroMemory(&biDirectory, sizeof(BROWSEINFO));
	// Set the structure members
	biDirectory.hwndOwner = m_pWindow->GetHWND();
	biDirectory.lpszTitle = "Select the destination root";
	biDirectory.ulFlags = BIF_USENEWUI | BIF_RETURNFSANCESTORS; 
	// Get the choosen directory
	char szDirectory[MAX_PATH];
	ITEMIDLIST *pItemList = SHBrowseForFolder(&biDirectory);
	if(!pItemList || !SHGetPathFromIDList(pItemList, szDirectory))
		return;

	if(pClassManager->GetArchiver()->Restore(m_szFileName, szDirectory))
		MessageBox(NULL, "Archive successfully extracted", "Success", MB_OK | MB_ICONINFORMATION);
	else
		MessageBox(NULL, "Extraction failed !", "Error", MB_OK | MB_ICONERROR);
}

void CInterface::Import()
{
	if(!strcmp(m_szFilePath, ""))
	{
		MessageBox(NULL, "Open an archive before importing !", "Error", MB_OK | MB_ICONERROR);
		return;
	}
	char szFile[MAX_PATH];
	char szFileName[MAX_PATH];
	// Initialize OPENFILENAME structure
	OPENFILENAME opFile;
	ZeroMemory(&opFile, sizeof(opFile));
	// Set the structure members
	opFile.lStructSize = sizeof(opFile);
	opFile.hwndOwner = m_pWindow->GetHWND();
	opFile.lpstrFile = szFile;
	opFile.lpstrFile[0] = '\0';
	opFile.nMaxFile = sizeof(szFile);
	opFile.lpstrFilter = "All\0*.*\0";
	opFile.nFilterIndex = 1;
	opFile.lpstrFileTitle = szFileName;
	opFile.nMaxFileTitle = sizeof(szFileName);
	opFile.lpstrInitialDir = NULL;
	opFile.lpstrTitle = "Impot a file";
	opFile.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	// Get the choosen file name
	BOOL bOpen = GetOpenFileName(&opFile);
	if(!bOpen)
		return;

	// Archive the file
	if(!pClassManager->GetArchiver()->ArchiveFile(m_szFilePath, szFile, szFileName))
	{
		MessageBox(NULL, "Failed to import the file !", "Error", MB_OK | MB_ICONERROR);
		return;
	}
	// Refresh the file
	DisplayFileInfo(m_szFilePath);
}

void CInterface::Encode()
{
	// Check if the archive is already encoded
	if (pClassManager->GetArchiver()->IsEncoded(m_szFilePath))
	{
		MessageBox(NULL, "Archive is already encoded !", "Error", MB_OK | MB_ICONERROR);
		return;
	}
	// Try to encode the archive
	if (pClassManager->GetArchiver()->EncodeFile(m_szFilePath))
		MessageBox(NULL, "Archive successfully encoded", "Success", MB_OK | MB_ICONINFORMATION);
	else
		MessageBox(NULL, "Encoding failed !", "Error", MB_OK | MB_ICONERROR);
}

void CInterface::Export(bool bChooseDir)
{
	// Get the current selected item
	int iItem = pClassManager->GetInterface()->GetListView()->GetSelectedItem();
	if(iItem == -1)
	{
		MessageBox(NULL, "Select a file to export !", "Error", MB_OK | MB_ICONERROR);
		return;
	}
	char szDirectory[MAX_PATH];
	// Are we choosing a directory ?
	if(bChooseDir)
	{
		// Initialize BROWSEINFO structure
		BROWSEINFO biDirectory;
		ZeroMemory(&biDirectory, sizeof(BROWSEINFO));
		// Set the structure members
		biDirectory.hwndOwner = m_pWindow->GetHWND();
		biDirectory.lpszTitle = "Select the destination root";
		biDirectory.ulFlags = BIF_USENEWUI | BIF_RETURNFSANCESTORS; 
		// Get the choosen directory
		ITEMIDLIST *pItemList = SHBrowseForFolder(&biDirectory);
		if(!pItemList || !SHGetPathFromIDList(pItemList, szDirectory))
			return;
	}
	else
		// Get the current working directory
		GetCurrentDirectory(MAX_PATH, szDirectory);

	// Get the item filename
	char szFileName[MAX_PATH];
	pClassManager->GetInterface()->GetListView()->GetItemText(iItem, szFileName);
	// Restore the file
	if(pClassManager->GetArchiver()->RestoreFile(m_szFileName, szFileName, szDirectory))
		MessageBox(NULL, "File successfully exported", "Success", MB_OK | MB_ICONINFORMATION);
	else
		MessageBox(NULL, "Exporting failed !", "Error", MB_OK | MB_ICONERROR);
}
