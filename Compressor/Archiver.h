#ifndef COMPRESS_H
#define COMPRESS_H

#include "../Shared/Array.h"

// Archiving errors
#define ARCHIVING_ERROR_NONE 0
#define ARCHIVING_ERROR_TARGET_FAILED_TO_OPEN 1
#define ARCHIVING_ERROR_DESTINATION_FAILED_TO_OPEN 2
#define ARCHIVING_ERROR_FAILED_TO_WRITE_TO_TARGET 3

// Restoring errors
#define RESTORING_ERROR_NONE 0
#define RESTORING_ERROR_TARGET_FAILED_TO_OPEN 1
#define RESTORING_ERROR_DEST_FAILED_TO_OPEN 2
#define RESTORING_ERROR_TARGET_FILE_EMPTY 3
#define RESTORING_ERROR_FAILED_TO_WRITE_TO_DEST 4
#define RESTORING_ERROR_INVALID_FILE_FORMAT 5

struct sHeader
{
	int iVersion;
	long lFilesCount;
	bool bIsEncoded;
};

class CArchiver
{
	public:

		// Constructor
		CArchiver();
		
		// Destructor
		~CArchiver();

		/* *********************************************************************
		- PrepareArchive():
			This function initiates an empty archive with a header

		- Parameters:
			szFileName: The archive's file name
			lFilesCount: The number of files to add later
			bIsEncoded: Is the archive encoded

		- Return:
			-

		**********************************************************************/
		virtual bool PrepareArchive(char *szFileName, long lFilesCount, bool bIsEncoded);

		/* *********************************************************************
		- Archive():
			This function archives a list of files into a single archive
		
		- Parameters: 
				szTarget: The archive's file name
				iNumer: The number of files to archives
				...: File names

		- Return:
				Boolean to indicate whether the archiving has succeeded or not

		**********************************************************************/
		//virtual bool Archive(char *szTarget, int iNumber, ...);

		/* *********************************************************************
		- ArchiveFile():
			This function archives a single file
		
		- Parameters: 
				szTarget: The target archive name
				szFile: The destination file name to archive

		- Return:
				Boolean to indicate whether the archiving has succeeded or not

		**********************************************************************/
		virtual bool ArchiveFile(char *szTarget, char *szFile, char *szFileName);

		/* *********************************************************************
		- GetArchivedFilesInfo():
			Retrieves information about the archived files 
		
		- Parameters: 
				szTarget: The target archive name

		- Return:
				Pointer to an array containing all the information

		**********************************************************************/
		virtual CArray *GetArchivedFilesInfo(char *szTarget);

		/* *********************************************************************
		- Restore():
			This function restores all of the archived files 
		
		- Parameters: 
				szFile: The destination archive name to restore
				szDirectory: The destination directory to write in

		- Return:
				Boolean to indicate whether the restoring has succeeded or not

		**********************************************************************/
		virtual bool Restore(char *szTarget, char *szDirectory);

		/* *********************************************************************
		- RestoreFile():
			Restores a single file from an archive
		
		- Parameters: 
				szTarget: The destination archive name
				szFileName: The file name to restore
				szDirectory: The destination directory to write in

		- Return:
				Boolean to indicate whether the restoring has succeeded or not

		**********************************************************************/
		virtual bool RestoreFile(char *szTarget, char *szFileName, char *szDirectory);

		/* *********************************************************************
		- EncodeFile():
			This function encodes an archive

		- Parameters:
			szFileName: The archive name

		- Return:
			Boolean to indicate whether the encoding has succeeded or not

		**********************************************************************/
		virtual bool EncodeFile(char *szFileName);

		/* *********************************************************************
		- IsEncoded():
			This function checks if the archive is encoded

		- Parameters:
			szFileName: The archive name

		- Return:
			Boolean to indicate whether the archive is encoded

		**********************************************************************/
		virtual bool IsEncoded(char *szFileName);

		/* *********************************************************************
		- GetError():
			This function returns the last error that occured
		
		- Parameters: 
				None

		- Return:
				Error value as BYTE

		**********************************************************************/
		virtual BYTE GetError() { return m_byteLastError; };

	private:

		/* *********************************************************************
		- GetHeader():
			This function returns an open archive's header

		- Parameters:
			pFile: Pointer to an open file

		- Return:
			Pointer the file's header

		**********************************************************************/
		sHeader *GetHeader(FILE *pFile);

		/* *********************************************************************
		- UpdateHeader():
			Will update an open archive's header

		- Parameters:
			pFile: Pointer to an open archive file
			pHeader: Pointer to the new header

		- Return:
			-

		**********************************************************************/
		void UpdateHeader(FILE *pFile, sHeader *pHeader);

		/* *********************************************************************
		- WriteIcon():
			Will write a file's icon information

		- Parameters:
			pFile: Pointer to an open archive file
			szFile: The name of the file to write its icon data

		- Return:
			A boolean indicating the operation's success

		**********************************************************************/
		bool WriteIcon(FILE *pFile, char *szFile);

		bool PrepareArchive(FILE *pFile, sHeader *pHeader);

		BYTE		m_byteLastError;

};

#endif