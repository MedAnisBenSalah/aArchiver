#include "Main.h"

HBITMAP CUtil::GetFileIcon(char *szFile)
{
	// Is that file a directory ?
	bool bIsDirectory = false;
	if(GetFileAttributes(szFile) & FILE_ATTRIBUTE_DIRECTORY)
		bIsDirectory = true;

	// Find the file extension
	char *szExtension = PathFindExtension(szFile);
	HICON hIcon = 0;
	// Is that an executable file ?
	if(!strcmp(szExtension, ".exe") || !strcmp(szExtension, ".lnk"))
		// Extract the icon from the executable
		ExtractIconEx(szFile, 0, NULL, &hIcon, 1);
	else 
	{
		// Get the file info
		SHFILEINFO sfFileInfo;  
		SHGetFileInfo(szFile, bIsDirectory ? FILE_ATTRIBUTE_DIRECTORY : FILE_ATTRIBUTE_NORMAL, &sfFileInfo, sizeof(sfFileInfo), SHGFI_ICON | SHGFI_USEFILEATTRIBUTES | SHGFI_SMALLICON);
		// Get the icon handle
		hIcon = sfFileInfo.hIcon;
	}
	// Get the icon info from the file info
	ICONINFO ifIconInfo;
	BOOL bIconInfo = GetIconInfo(hIcon, &ifIconInfo);
	// Did we sucessfully got the icon info ?
	if(bIconInfo)
		// Return the icon bitmap handle
		return ifIconInfo.hbmColor;
	else
		return NULL;
}

bool CUtil::IsDirectory(char *szFile)
{
	// Check if its a directory or not
	if(GetFileAttributes(szFile) & FILE_ATTRIBUTE_DIRECTORY)
		return true;

	return false;
}