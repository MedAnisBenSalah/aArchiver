#ifndef COMPRESS_H
#define COMPRESS_H

#include "../Shared/Array.h"

// General defintions
#define ARCHIVER_MODULE		"aArchiverCore.dll"

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

class CArchiver
{
	public:

		virtual bool PrepareArchive(char *szFileName, long lFilesCount, bool bIsEncoded) = 0;
		virtual bool ArchiveFile(char *szTarget, char *szFile, char *szFileName) = 0;
		virtual CArray *GetArchivedFilesInfo(char *szTarget) = 0;
		virtual bool Restore(char *szTarget, char *szDirectory) = 0;
		virtual bool RestoreFile(char *szTarget, char *szFileName, char *szDirectory) = 0;
		virtual bool EncodeFile(char *szFileName) = 0;
		virtual bool IsEncoded(char *szFileName) = 0;
		virtual BYTE GetError() = 0;

};

#endif