#include "Main.h"

CArchiver::CArchiver()
{
}

CArchiver::~CArchiver()
{
}

/*bool CArchiver::Compress(char *szTarget, int iNumber, ...)
{
	// Initialize the last error
	m_byteLastError = ARCHIVING_ERROR_NONE;
	// Open the target file to append in binary
	FILE *pTarget = fopen(szTarget, "ab");
	if(!pTarget)
	{
		if(!(pTarget = fopen(szTarget, "wb")))
		{
			m_byteLastError = ARCHIVING_ERROR_TARGET_FAILED_TO_OPEN;
			return false;
		}
	}
	// Open the destination file
	FILE *pDestination = fopen(szFile, "rb");
	if(!pDestination)
	{
		m_byteLastError = ARCHIVING_ERROR_DESTINATION_FAILED_TO_OPEN;
		fclose(pTarget);
		return false;
	}
	// Get the destination file size
	fseek(pDestination, 0, SEEK_END);
	size_t sDestinationSize = ftell(pDestination);
	fseek(pDestination, 0, SEEK_SET);
	// Read the destination file
	char *szBuffer = (char *)malloc(sDestinationSize);
	fread(szBuffer, sDestinationSize, 1, pDestination);
	// Write the file name length to the target file
	size_t sLength = strlen(szFile);
	fwrite((void *)&sLength, sizeof(size_t), 1, pTarget);
	// Write the file name to the target file
	fwrite(szFile, sLength, 1, pTarget);
	// Write the file size
	fwrite((void *)&sDestinationSize, sizeof(size_t), 1, pTarget);
	// Write the file buffer
	fwrite(szBuffer, sDestinationSize, 1, pTarget);
	// Close both files
	fclose(pTarget);
	fclose(pDestination);
	return true;
}*/

bool CArchiver::PrepareArchive(char *szFileName, long lFilesCount, bool bIsEncoded)
{
	// Initialize the last error
	m_byteLastError = ARCHIVING_ERROR_NONE;
	// Open the target file to write in binary
	FILE *pTarget = fopen(szFileName, "wb");
	if (!pTarget)
	{
		m_byteLastError = ARCHIVING_ERROR_TARGET_FAILED_TO_OPEN;
		return false;
	}
	// Create a header
	sHeader header;
	header.iVersion = VERSION;
	header.lFilesCount = lFilesCount;
	header.bIsEncoded = bIsEncoded;
	// Write the header
	if (fwrite(&header, sizeof(sHeader), 1, pTarget) == 0)
	{
		m_byteLastError = ARCHIVING_ERROR_FAILED_TO_WRITE_TO_TARGET;
		fclose(pTarget);
		return false;
	}
	// Close the file
	fclose(pTarget);
	return true;
}

bool CArchiver::PrepareArchive(FILE *pFile, sHeader *pHeader)
{
	// Initialize the last error
	m_byteLastError = ARCHIVING_ERROR_NONE;
	// Write the header
	if (fwrite(pHeader, sizeof(sHeader), 1, pFile) == 0)
	{
		m_byteLastError = ARCHIVING_ERROR_FAILED_TO_WRITE_TO_TARGET;
		return false;
	}
	return true;
}

sHeader *CArchiver::GetHeader(FILE *pFile)
{
	// Set file pointer to the beginning
	int iPointer = ftell(pFile);
	fseek(pFile, 0, SEEK_SET);
	// Read the header
	sHeader header;
	fread(&header, sizeof(sHeader), 1, pFile);
	// Restore pointer
	fseek(pFile, iPointer, SEEK_SET);
	return &header;
}

void CArchiver::UpdateHeader(FILE *pFile, sHeader *pHeader)
{
	// Set file pointer to the beginning
	int iPointer = ftell(pFile);
	fseek(pFile, 0, SEEK_SET);
	// Read the header
	fwrite(pHeader, sizeof(sHeader), 1, pFile);
	// Restore pointer
	fseek(pFile, iPointer, SEEK_SET);
}

bool CArchiver::ArchiveFile(char *szTarget, char *szFile, char *szFileName)
{
	// Initialize the last error
	m_byteLastError = ARCHIVING_ERROR_NONE;
	// Open the target file to read and update in binary
	FILE *pTarget = fopen(szTarget, "rb+");
	if(!pTarget)
	{
		if(!(pTarget = fopen(szTarget, "wb")))
		{
			m_byteLastError = ARCHIVING_ERROR_TARGET_FAILED_TO_OPEN;
			return false;
		}
	}
	// Open the destination file
	FILE *pDestination = fopen(szFile, "rb");
	if(!pDestination)
	{
		m_byteLastError = ARCHIVING_ERROR_DESTINATION_FAILED_TO_OPEN;
		fclose(pTarget);
		remove(szTarget);
		return false;
	}
	// Get the destination file size
	fseek(pDestination, 0, SEEK_END);
	size_t sDestinationSize = ftell(pDestination);
	fseek(pDestination, 0, SEEK_SET);
	// Read the destination file
	char *szBuffer = (char *)malloc(sDestinationSize);
	fread(szBuffer, sDestinationSize, 1, pDestination);
	// Are we encoded ?
	sHeader *pHeader = GetHeader(pTarget);
	if (pHeader->bIsEncoded)
	{
		// Get the target file size
		fseek(pTarget, 0, SEEK_END);
		size_t sTargetSize = ftell(pTarget);
		// Ignore header
		fseek(pTarget, sizeof(sHeader), SEEK_SET);
		// Read encoded data
		char *szFileData = (char *)malloc(sTargetSize);
		fread(szFileData, sTargetSize, 1, pTarget);
		// Close the file
		fclose(pTarget);
		// Open a clean new file
		pTarget = fopen(szTarget, "wb");
		// Prepare archive
		PrepareArchive(pTarget, pHeader);
		// Decode data
		std::string strDecodedData = CEncoder::Decode(std::string(szFileData, sTargetSize));
		// Get decoded data
		const char *szDecodedData = strDecodedData.c_str();
		size_t sDecodedDataLength = strDecodedData.length();
		// Write to the file
		fwrite(szDecodedData, sDecodedDataLength, 1, pTarget);
	}
	else
		// Go to the end of the file
		fseek(pTarget, 0, SEEK_END);

	// Write the file name length to the target file
	size_t sLength = strlen(szFileName);
	fwrite(&sLength, sizeof(size_t), 1, pTarget);
	// Write the file name to the target file
	fwrite(szFileName, sLength, 1, pTarget);
	// Try to write the file icon
	if(!WriteIcon(pTarget, szFile))
	{
		m_byteLastError = ARCHIVING_ERROR_DESTINATION_FAILED_TO_OPEN;
		fclose(pTarget);
		fclose(pDestination);
		remove(szTarget);
		return false;
	}
	// Get the file info
	SHFILEINFO sfFileInfo;  
	SHGetFileInfo(szFile, CUtil::IsDirectory(szFile) ? FILE_ATTRIBUTE_DIRECTORY : FILE_ATTRIBUTE_NORMAL, &sfFileInfo, sizeof(sfFileInfo), SHGFI_TYPENAME);
	// Write the file type length
	DWORD dwTypeLength = strlen(sfFileInfo.szTypeName);
	fwrite(&dwTypeLength, sizeof(DWORD), 1, pTarget);
	// Write the file type
	fwrite(sfFileInfo.szTypeName, dwTypeLength, 1, pTarget);
	// Get the file last modified and created time
	WIN32_FILE_ATTRIBUTE_DATA attributeData;
	GetFileAttributesEx(szFile, GetFileExInfoStandard, &attributeData);
	// Convert file time to system time
	SYSTEMTIME lastModifiedTime, creationTime;
	FileTimeToSystemTime(&attributeData.ftLastWriteTime, &lastModifiedTime);
	FileTimeToSystemTime(&attributeData.ftCreationTime, &creationTime);
	// Write the creation time structure
	fwrite(&creationTime, sizeof(SYSTEMTIME), 1, pTarget);
	// Write the last modified time structure
	fwrite(&lastModifiedTime, sizeof(SYSTEMTIME), 1, pTarget);
	// Write the file size
	fwrite(&sDestinationSize, sizeof(size_t), 1, pTarget);
	// Write the file buffer
	fwrite(szBuffer, sDestinationSize, 1, pTarget);
	// Update header
	pHeader->lFilesCount++;
	UpdateHeader(pTarget, pHeader);
	// Close both files
	fclose(pTarget);
	fclose(pDestination);
	// Re-encode archive if necessary
	if (pHeader->bIsEncoded)
		EncodeFile(szTarget);

	return true;
}

CArray *CArchiver::GetArchivedFilesInfo(char *szTarget)
{
	// Initialize the last error
	m_byteLastError = RESTORING_ERROR_NONE;
	// Open our target file 
	FILE *pTarget = fopen(szTarget, "rb");
	// Make sure the file correctly opened
	if(!pTarget)
	{
		m_byteLastError = RESTORING_ERROR_TARGET_FAILED_TO_OPEN;
		return NULL;
	}
	// Get header
	sHeader *pHeader = GetHeader(pTarget);
	char t[128];
	sprintf(t, "hdr: %d", pHeader->lFilesCount);
	//MessageBox(NULL, t, "info", MB_OK);
	// Get the target file size
	fseek(pTarget, 0, SEEK_END);
	size_t sTargetSize = ftell(pTarget) - sizeof(sHeader);
	//fseek(pTarget, 0, SEEK_SET);
	// Ignore header
	fseek(pTarget, sizeof(sHeader), SEEK_SET);
	// Decode the file if its encoded
	FILE *pEncodedFile = NULL;
	char szTempFileName[MAX_PATH];
	if (pHeader->bIsEncoded)
	{
		// Read encoded data
		char *szFileData = (char *)malloc(sTargetSize);
		fread(szFileData, sTargetSize, 1, pTarget);		
		// Decode data
		std::string strDecodedData = CEncoder::Decode(std::string(szFileData, sTargetSize));
		// Open a temp file		
		sprintf(szTempFileName, "%s.tmp", szTarget);
		FILE *pTemp = fopen(szTempFileName, "wb+");
		if (!pTemp)
		{
			m_byteLastError = RESTORING_ERROR_TARGET_FAILED_TO_OPEN;
			return NULL;
		}
		// Get decoded data
		const char *szDecodedData = strDecodedData.c_str();
		size_t sDecodedDataLength = strDecodedData.length();
		// Write to the temp file
		fwrite(szDecodedData, sDecodedDataLength, 1, pTemp);
		// Flush the file
		fflush(pTemp);
		// Switch targets
		pEncodedFile = pTarget;
		pTarget = pTemp;
		// Set the target's position
		fseek(pTarget, 0, SEEK_SET);
	}
	// Read the file name length from the target
	size_t sNameLength = 0;
	int iReadedBytes = 0;
	if(!(iReadedBytes = fread(&sNameLength, sizeof(size_t), 1, pTarget)))
	{
		size_t ss = ftell(pTarget);
		fclose(pTarget);
		m_byteLastError = RESTORING_ERROR_TARGET_FILE_EMPTY;
		return NULL;
	}
	// Create the array
	CArray *pFilesArray = new CArray();
	// Keep going until we finish all the file
	bool bFinished = false;
	long lFilesRead = 0;
	while(!bFinished)
	{
		// Read the file name
		char *szFile = (char *)malloc(sNameLength);
		if(!fread(szFile, sNameLength, 1, pTarget))
		{
			SAFE_DELETE(pFilesArray);
			fclose(pTarget);
			if (pEncodedFile != NULL)
			{
				fclose(pEncodedFile);
				remove(szTempFileName);
			}
			m_byteLastError = RESTORING_ERROR_INVALID_FILE_FORMAT;
			return NULL;
		}
		szFile[sNameLength] = '\0';
		// Push the file name to the array
		pFilesArray->Push(sNameLength);
		pFilesArray->Push((unsigned char *)szFile, sNameLength);
		// Read the file icon size
		DWORD dwIconLength = 0;
		fread(&dwIconLength, sizeof(DWORD), 1, pTarget);
		// Allocate for the icon buffer
		BYTE *pbyteIconBuffer = new BYTE[dwIconLength];
		// Read the icon buffer
		fread(pbyteIconBuffer, dwIconLength, 1, pTarget);
		// Write the icon to the array
		pFilesArray->Push(dwIconLength);
		pFilesArray->Push(pbyteIconBuffer, dwIconLength);
		// Read the file type length
		DWORD dwTypeLength = 0;
		fread(&dwTypeLength, sizeof(DWORD), 1, pTarget);
		// Read the file type
		char *szType = (char *)malloc(dwTypeLength);
		// Read the file type
		fread(szType, dwTypeLength, 1, pTarget);
		szType[dwTypeLength] = '\0';
		// Write the file type to the array
		pFilesArray->Push(dwTypeLength);
		pFilesArray->Push((unsigned char *)szType, dwTypeLength);
		// Get the creation and last modified time
		SYSTEMTIME lastModifiedTime, creationTime;
		fread(&creationTime, sizeof(SYSTEMTIME), 1, pTarget);
		fread(&lastModifiedTime, sizeof(SYSTEMTIME), 1, pTarget);
		// Push them into the array
		pFilesArray->Push((unsigned char *)&creationTime, sizeof(SYSTEMTIME));
		pFilesArray->Push((unsigned char *)&lastModifiedTime, sizeof(SYSTEMTIME));
		// Read the file size
		size_t sSize = 0;
		if(!fread(&sSize, sizeof(size_t), 1, pTarget))
		{
			SAFE_DELETE(pFilesArray);
			fclose(pTarget);
			if (pEncodedFile != NULL)
			{
				fclose(pEncodedFile);
				remove(szTempFileName);
			}
			m_byteLastError = RESTORING_ERROR_INVALID_FILE_FORMAT;
			return NULL;
		}
		// Push the file size
		pFilesArray->Push(sSize);
		// Skip the file buffer
		fseek(pTarget, sSize, SEEK_CUR);
		// Increase files read
		lFilesRead++;
		// Check if we have finished decompressing
		if((size_t)ftell(pTarget) >= sTargetSize || lFilesRead >= pHeader->lFilesCount)
			bFinished = true;

		// If we havent finished yet then read the next file name length
		if(!bFinished)
		{
			if(!fread(&sNameLength, sizeof(size_t), 1, pTarget))
			{
				SAFE_DELETE(pFilesArray);
				fclose(pTarget);
				if (pEncodedFile != NULL)
				{
					fclose(pEncodedFile);
					remove(szTempFileName);
				}
				m_byteLastError = RESTORING_ERROR_INVALID_FILE_FORMAT;
				return NULL;
			}
		}
	}
	fclose(pTarget);
	if (pEncodedFile != NULL)
	{
		fclose(pEncodedFile);
		remove(szTempFileName);
	}
	return pFilesArray;
}

bool CArchiver::Restore(char *szTarget, char *szDirectory)
{
	// Initialize the last error
	m_byteLastError = RESTORING_ERROR_NONE;
	// Open our target file 
	FILE *pTarget = fopen(szTarget, "rb");
	// Make sure the file correctly opened
	if(!pTarget)
	{
		m_byteLastError = RESTORING_ERROR_TARGET_FAILED_TO_OPEN;
		return false;
	}
	// Get the target file size
	fseek(pTarget, 0, SEEK_END);
	size_t sTargetSize = ftell(pTarget);
	//fseek(pTarget, 0, SEEK_SET);
	// Ignore header
	fseek(pTarget, sizeof(sHeader), SEEK_SET);
	// Get header
	sHeader *pHeader = GetHeader(pTarget);
	// Decode the file if its encoded
	FILE *pEncodedFile = NULL;
	char szTempFileName[MAX_PATH];
	if (pHeader->bIsEncoded)
	{
		// Read encoded data
		char *szFileData = (char *)malloc(sTargetSize);
		fread(szFileData, sTargetSize, 1, pTarget);
		// Decode data
		std::string strDecodedData = CEncoder::Decode(std::string(szFileData, sTargetSize));
		// Open a temp file		
		sprintf(szTempFileName, "%s.tmp", szTarget);
		FILE *pTemp = fopen(szTempFileName, "wb+");
		if (!pTemp)
		{
			m_byteLastError = RESTORING_ERROR_TARGET_FAILED_TO_OPEN;
			return NULL;
		}
		// Get decoded data
		const char *szDecodedData = strDecodedData.c_str();
		size_t sDecodedDataLength = strDecodedData.length();
		// Write to the temp file
		fwrite(szDecodedData, sDecodedDataLength, 1, pTemp);
		// Flush the file
		fflush(pTemp);
		// Switch targets
		pEncodedFile = pTarget;
		pTarget = pTemp;
		// Set the target's position
		fseek(pTarget, 0, SEEK_SET);
	}
	// Read the file name length from the target
	size_t sNameLength = 0;
	int iReadedBytes = 0;
	if(!(iReadedBytes = fread(&sNameLength, sizeof(size_t), 1, pTarget)))
	{
		fclose(pTarget);
		if (pEncodedFile != NULL)
		{
			fclose(pEncodedFile);
			remove(szTempFileName);
		}
		m_byteLastError = RESTORING_ERROR_TARGET_FILE_EMPTY;
		return false;
	}
	// Keep going until we finish all the file
	bool bFinished = false;
	long lFilesRead = 0;
	while(!bFinished)
	{
		// Read the file name
		char *szFile = (char *)malloc(sNameLength);
		if(!fread(szFile, sNameLength, 1, pTarget))
		{
			fclose(pTarget);
			if (pEncodedFile != NULL)
			{
				fclose(pEncodedFile);
				remove(szTempFileName);
			}
			m_byteLastError = RESTORING_ERROR_INVALID_FILE_FORMAT;
			return false;
		}
		szFile[sNameLength] = '\0';
		// Generate the file name
		char szFileName[MAX_PATH];
		sprintf(szFileName, "%s/%s", szDirectory, szFile);
		// Open the destination file
		FILE *pFile = fopen(szFileName, "wb");
		// Make sure the file correctly opened
		if(!pFile)
		{
			fclose(pTarget);
			if (pEncodedFile != NULL)
			{
				fclose(pEncodedFile);
				remove(szTempFileName);
			}
			m_byteLastError = RESTORING_ERROR_DEST_FAILED_TO_OPEN;
			return false;
		}
		// Read the file icon size
		size_t sIconSize = 0;
		if(!fread(&sIconSize, sizeof(size_t), 1, pTarget))
		{
			fclose(pTarget);
			fclose(pFile);
			if (pEncodedFile != NULL)
			{
				fclose(pEncodedFile);
				remove(szTempFileName);
			}
			m_byteLastError = RESTORING_ERROR_INVALID_FILE_FORMAT;
			return false;
		}
		// Skip the icon buffer
		fseek(pTarget, sIconSize, SEEK_CUR);
		// Read the file type size
		size_t sTypeSize = 0;
		if(!fread(&sTypeSize, sizeof(size_t), 1, pTarget))
		{
			fclose(pTarget);
			fclose(pFile);
			if (pEncodedFile != NULL)
			{
				fclose(pEncodedFile);
				remove(szTempFileName);
			}
			m_byteLastError = RESTORING_ERROR_INVALID_FILE_FORMAT;
			return false;
		}
		// Skip the file type buffer and last modified and creation time
		fseek(pTarget, sTypeSize + sizeof(SYSTEMTIME) * 2, SEEK_CUR);
		// Read the file size
		size_t sSize = 0;
		if(!fread(&sSize, sizeof(size_t), 1, pTarget))
		{
			fclose(pTarget);
			fclose(pFile);
			if (pEncodedFile != NULL)
			{
				fclose(pEncodedFile);
				remove(szTempFileName);
			}
			m_byteLastError = RESTORING_ERROR_INVALID_FILE_FORMAT;
			return false;
		}
		// Create and allocate the destination buffer
		unsigned char *ucBuffer = (unsigned char *)malloc(sSize);
		// Read the file data from the target
		if(!fread(ucBuffer, sSize, 1, pTarget))
		{
			// Close files
			fclose(pTarget);
			fclose(pFile);
			if (pEncodedFile != NULL)
			{
				fclose(pEncodedFile);
				remove(szTempFileName);
			}
			// Set the last error
			m_byteLastError = RESTORING_ERROR_INVALID_FILE_FORMAT;
			return false;
		}
		// Write to the destination file
		if(!fwrite(ucBuffer, sSize, 1, pFile))
		{
			// Close files
			fclose(pTarget);
			fclose(pFile);
			// Free the buffer
			free(ucBuffer);
			if (pEncodedFile != NULL)
			{
				fclose(pEncodedFile);
				remove(szTempFileName);
			}
			// Set the last error
			m_byteLastError = RESTORING_ERROR_FAILED_TO_WRITE_TO_DEST;
			return false;
		}
		// Close files
		fclose(pFile);
		// Increase files read
		lFilesRead++;
		// Check if we have finished decompressing
		if((size_t)ftell(pTarget) + sizeof(sHeader) >= sTargetSize || lFilesRead >= pHeader->lFilesCount)
			bFinished = true;

		// If we havent finished yet then read the next file name length
		if(!bFinished)
		{
			if(!fread(&sNameLength, sizeof(size_t), 1, pTarget))
			{
				fclose(pTarget);
				m_byteLastError = RESTORING_ERROR_INVALID_FILE_FORMAT;
				if (pEncodedFile != NULL)
				{
					fclose(pEncodedFile);
					remove(szTempFileName);
				}
				return false;
			}
		}
	}
	fclose(pTarget);
	if (pEncodedFile != NULL)
	{
		fclose(pEncodedFile);
		remove(szTempFileName);
	}
	return true;
}

bool CArchiver::RestoreFile(char *szTarget, char *szFileName, char *szDirectory)
{
	// Initialize the last error
	m_byteLastError = RESTORING_ERROR_NONE;
	// Open our target file 
	FILE *pTarget = fopen(szTarget, "rb");
	// Make sure the file correctly opened
	if(!pTarget)
	{
		m_byteLastError = RESTORING_ERROR_TARGET_FAILED_TO_OPEN;
		return false;
	}
	// Get the target file size
	fseek(pTarget, 0, SEEK_END);
	size_t sTargetSize = ftell(pTarget);
	//fseek(pTarget, 0, SEEK_SET);
	// Ignore header
	fseek(pTarget, sizeof(sHeader), SEEK_SET);
	// Get header
	sHeader *pHeader = GetHeader(pTarget);
	// Decode the file if its encoded
	FILE *pEncodedFile = NULL;
	char szTempFileName[MAX_PATH];
	if (pHeader->bIsEncoded)
	{
		// Read encoded data
		char *szFileData = (char *)malloc(sTargetSize);
		fread(szFileData, sTargetSize, 1, pTarget);
		// Decode data
		std::string strDecodedData = CEncoder::Decode(std::string(szFileData, sTargetSize));
		// Open a temp file		
		sprintf(szTempFileName, "%s.tmp", szTarget);
		FILE *pTemp = fopen(szTempFileName, "wb+");
		if (!pTemp)
		{
			m_byteLastError = RESTORING_ERROR_TARGET_FAILED_TO_OPEN;
			return NULL;
		}
		// Get decoded data
		const char *szDecodedData = strDecodedData.c_str();
		size_t sDecodedDataLength = strDecodedData.length();
		// Write to the temp file
		fwrite(szDecodedData, sDecodedDataLength, 1, pTemp);
		// Flush the file
		fflush(pTemp);
		// Switch targets
		pEncodedFile = pTarget;
		pTarget = pTemp;
		// Set the target's position
		fseek(pTarget, 0, SEEK_SET);
	}
	// Read the file name length from the target
	size_t sNameLength = 0;
	int iReadedBytes = 0;
	if(!(iReadedBytes = fread(&sNameLength, sizeof(size_t), 1, pTarget)))
	{
		fclose(pTarget);
		m_byteLastError = RESTORING_ERROR_TARGET_FILE_EMPTY;
		return false;
	}
	// Keep going until we find the file
	bool bFinished = false;
	FILE *pFile = NULL;
	long lFilesRead = 0;
	while(!bFinished)
	{
		// Read the file name
		char *szFile = (char *)malloc(sNameLength);
		if(!fread(szFile, sNameLength, 1, pTarget))
		{
			fclose(pTarget);
			if (pEncodedFile != NULL)
			{
				fclose(pEncodedFile);
				remove(szTempFileName);
			}
			m_byteLastError = RESTORING_ERROR_INVALID_FILE_FORMAT;
			return false;
		}
		szFile[sNameLength] = '\0';
		// Is it the file we're looking for ?
		if(!strcmp(szFile, szFileName))
		{
			// Indicate the we found the file
			bFinished = true;
			// Generate the file name
			char _szFileName[MAX_PATH];
			sprintf(_szFileName, "%s/%s", szDirectory, szFile);
			// Open the destination file
			pFile = fopen(_szFileName, "wb");
			// Make sure the file correctly opened
			if(!pFile)
			{
				fclose(pTarget);
				if (pEncodedFile != NULL)
				{
					fclose(pEncodedFile);
					remove(szTempFileName);
				}
				m_byteLastError = RESTORING_ERROR_DEST_FAILED_TO_OPEN;
				return false;
			}
		}
		// Read the file icon size
		size_t sIconSize = 0;
		if(!fread(&sIconSize, sizeof(size_t), 1, pTarget))
		{
			fclose(pTarget);
			fclose(pFile);
			if (pEncodedFile != NULL)
			{
				fclose(pEncodedFile);
				remove(szTempFileName);
			}
			m_byteLastError = RESTORING_ERROR_INVALID_FILE_FORMAT;
			return false;
		}
		// Skip the icon buffer
		fseek(pTarget, sIconSize, SEEK_CUR);
		// Read the file type size
		size_t sTypeSize = 0;
		if(!fread(&sTypeSize, sizeof(size_t), 1, pTarget))
		{
			fclose(pTarget);
			fclose(pFile);
			if (pEncodedFile != NULL)
			{
				fclose(pEncodedFile);
				remove(szTempFileName);
			}
			m_byteLastError = RESTORING_ERROR_INVALID_FILE_FORMAT;
			return false;
		}
		// Skip the file type buffer and last modified and creation time
		fseek(pTarget, sTypeSize + sizeof(SYSTEMTIME) * 2, SEEK_CUR);
		// Read the file size
		size_t sSize = 0;
		if(!fread(&sSize, sizeof(size_t), 1, pTarget))
		{
			fclose(pTarget);
			fclose(pFile);
			if (pEncodedFile != NULL)
			{
				fclose(pEncodedFile);
				remove(szTempFileName);
			}
			m_byteLastError = RESTORING_ERROR_INVALID_FILE_FORMAT;
			return false;
		}
		// Did we found the file yet ?
		if(bFinished)
		{
			// Create and allocate the destination buffer
			unsigned char *ucBuffer = (unsigned char *)malloc(sSize);
			// Read the file data from the target
			if(!fread(ucBuffer, sSize, 1, pTarget))
			{
				// Close files
				fclose(pTarget);
				fclose(pFile);
				if (pEncodedFile != NULL)
				{
					fclose(pEncodedFile);
					remove(szTempFileName);
				}
				// Set the last error
				m_byteLastError = RESTORING_ERROR_INVALID_FILE_FORMAT;
				return false;
			}
			// Write to the destination file
			if(!fwrite(ucBuffer, sSize, 1, pFile))
			{
				// Close files
				fclose(pTarget);
				fclose(pFile);
				if (pEncodedFile != NULL)
				{
					fclose(pEncodedFile);
					remove(szTempFileName);
				}
				// Free the buffer
				free(ucBuffer);
				// Set the last error
				m_byteLastError = RESTORING_ERROR_FAILED_TO_WRITE_TO_DEST;
				return false;
			}
			// Close files
			fclose(pFile);
		}	
		else
			// Skip the file buffer
			fseek(pTarget, sSize, SEEK_CUR);

		// Check if we have finished decompressing
		if(((size_t)ftell(pTarget) + sizeof(sHeader) >= sTargetSize || lFilesRead >= pHeader->lFilesCount) && !bFinished)
			bFinished = true;

		// If we havent finished yet then read the next file name length
		if(!bFinished)
		{
			if(!fread(&sNameLength, sizeof(size_t), 1, pTarget))
			{
				fclose(pTarget);
				if (pEncodedFile != NULL)
				{
					fclose(pEncodedFile);
					remove(szTempFileName);
				}
				m_byteLastError = RESTORING_ERROR_INVALID_FILE_FORMAT;
				return false;
			}
		}
	}
	fclose(pTarget);
	if (pEncodedFile != NULL)
	{
		fclose(pEncodedFile);
		remove(szTempFileName);
	}
	return bFinished;
}

bool CArchiver::WriteIcon(FILE *pFile, char *szFile)
{
	// Get the file icon bitmap handle
	HBITMAP hIcon = CUtil::GetFileIcon(szFile);
	// Do we have a valid icon ?
	if(!hIcon)
		return false;
	
	// Get the BITMAP handle
	BITMAP bmpIcon;
	GetObject(hIcon, sizeof(BITMAP), &bmpIcon);
	// Calculate the ClrBits
	int iClrBits = bmpIcon.bmPlanes * bmpIcon.bmBitsPixel;
	// Create the BITMAPINFO struct
	LPBITMAPINFO lpBmpInfo = new BITMAPINFO;
	memset(lpBmpInfo, 0, sizeof(BITMAPINFO));
	// Setup the bitmap info header struct 
	lpBmpInfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	lpBmpInfo->bmiHeader.biWidth = bmpIcon.bmWidth;
	lpBmpInfo->bmiHeader.biHeight = bmpIcon.bmHeight;
	lpBmpInfo->bmiHeader.biPlanes = bmpIcon.bmPlanes;
	lpBmpInfo->bmiHeader.biBitCount = bmpIcon.bmBitsPixel;
	// Set the used ClrBits
	if(iClrBits < 24)
		lpBmpInfo->bmiHeader.biClrUsed = (1 << iClrBits);
	
	lpBmpInfo->bmiHeader.biCompression = BI_RGB;
	lpBmpInfo->bmiHeader.biSizeImage = ((lpBmpInfo->bmiHeader.biWidth * iClrBits + 15) & ~15) / 8 * lpBmpInfo->bmiHeader.biHeight;
	// Get the DI Bits
	BYTE *pbyteBuffer = new BYTE[bmpIcon.bmHeight * bmpIcon.bmHeight * bmpIcon.bmHeight];
	GetDIBits(GetDC(NULL), hIcon, 0, bmpIcon.bmHeight, pbyteBuffer, lpBmpInfo, DIB_RGB_COLORS);
	// Set the icon buffer
	BYTE *pbyteIconBuffer = new BYTE[bmpIcon.bmHeight * bmpIcon.bmHeight * bmpIcon.bmHeight + sizeof(BITMAPINFO)];
	memset(pbyteIconBuffer, 0, bmpIcon.bmHeight * bmpIcon.bmHeight * bmpIcon.bmHeight + sizeof(BITMAPINFO));
	memcpy(pbyteIconBuffer, lpBmpInfo, sizeof(BITMAPINFO));
	memcpy(pbyteIconBuffer + sizeof(BITMAPINFO), pbyteBuffer, lpBmpInfo->bmiHeader.biSizeImage);
	// Write the icon buffer length
	DWORD dwIconLength = lpBmpInfo->bmiHeader.biSizeImage + sizeof(BITMAPINFO);
	fwrite(&dwIconLength, sizeof(DWORD), 1, pFile);
	// Write the icon to the file
	fwrite(pbyteIconBuffer, dwIconLength, 1, pFile);
	// Free the icon buffer
	SAFE_DELETE(pbyteIconBuffer);
	SAFE_DELETE(pbyteBuffer);
	return true;
}

bool CArchiver::EncodeFile(char *szFileName)
{
	// Open the destination file
	FILE *pDestination = fopen(szFileName, "rb");
	if (!pDestination)
	{
		m_byteLastError = ARCHIVING_ERROR_DESTINATION_FAILED_TO_OPEN;
		return false;
	}
	// Get header
	sHeader *pHeader = GetHeader(pDestination);
	// Get the destination file size
	fseek(pDestination, 0, SEEK_END);
	size_t sDestinationSize = ftell(pDestination) - sizeof(sHeader);
	//fseek(pDestination, 0, SEEK_SET);
	// Ignore header
	fseek(pDestination, sizeof(sHeader), SEEK_SET);
	// Read the destination file
	char *szBuffer = (char *)malloc(sDestinationSize);
	fread(szBuffer, sDestinationSize, 1, pDestination);
	// Close the file 
	fclose(pDestination);
	// Open the target file to write in binary
	FILE *pTarget = fopen(szFileName, "wb");
	if (!pTarget)
	{
		m_byteLastError = ARCHIVING_ERROR_TARGET_FAILED_TO_OPEN;
		free(szBuffer);
		return false;
	}
	// Update header
	pHeader->bIsEncoded = true;
	// Write header
	PrepareArchive(pTarget, pHeader);
	// Encode data
	std::string strEncodedData = CEncoder::Encode((const unsigned char *)szBuffer, sDestinationSize);
	const char *szEncodedData = strEncodedData.c_str();
	const size_t sEncodedLength = strEncodedData.length();
	// Write to file
	fwrite(szEncodedData, sEncodedLength, 1, pTarget);
	// Close file
	fclose(pTarget);
	// Free allocated buffer
	free(szBuffer);
	return true;
}

bool CArchiver::IsEncoded(char *szFileName)
{
	// Open the destination file
	FILE *pDestination = fopen(szFileName, "rb");
	if (!pDestination)
	{
		m_byteLastError = ARCHIVING_ERROR_DESTINATION_FAILED_TO_OPEN;
		return false;
	}
	// Get header
	sHeader *pHeader = GetHeader(pDestination);
	// Close file
	fclose(pDestination);
	return pHeader->bIsEncoded;
}