#ifndef UTIL_H
#define UTIL_H

class CUtil
{
	public:
		static HBITMAP GetFileIcon(char *szFile);
		static bool IsDirectory(char *szFile);
};

#endif