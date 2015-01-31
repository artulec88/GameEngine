#include "StdAfx.h"
#include "FileManager.h"
#include "ILogger.h"

//#include <windows.h>
//#include <tchar.h> 
//#include <stdio.h>
//#include <strsafe.h>
#include <dirent.h>

using namespace Utility;

FileManager::FileManager(void)
{
}


FileManager::~FileManager(void)
{
}

std::vector<std::string> FileManager::ListAllFilesInDirectory(const std::string& directoryPath) const
{
	//LOG(Info, LOGPLACE, "Listing all files in a directory %s", directoryPath.c_str());
	//if (directoryPath.length() > MAX_PATH - 3) // Check that the input path plus 3 is not longer than MAX_PATH. Three characters are for the "\*" plus NULL appended below.
	//{
	//	LOG(Error, LOGPLACE, "Cannot list the files in a given directory. The directory path is too long");
	//	return;
	//}
	//WIN32_FIND_DATA ffd;
	//LARGE_INTEGER filesize;
	//TCHAR szDir[MAX_PATH];
	//HANDLE hFind = INVALID_HANDLE_VALUE;
	//DWORD dwError = 0;

	////const wchar_t* = STRSAFE_LPCWSTR
	//std::wstring wideStrDirPath = std::wstring(directoryPath.begin(), directoryPath.end());
	//StringCchCopy(szDir, MAX_PATH, wideStrDirPath.c_str()); // Prepare string for use with FindFile functions.  First, copy the string to a buffer...
	//StringCchCat(szDir, MAX_PATH, TEXT("\\*")); // ... then append '\*' to the directory name.

	//hFind = FindFirstFile(szDir, &ffd); // Find the first file in a directory
	//
	//if (hFind == INVALID_HANDLE_VALUE)
	//{
	//	LOG(Emergency, LOGPLACE, "Cannot list the files in a given directory. Invalid directory handle instance.");
	//	//return dwError;
	//}
	//
	//// List all the files in the directory with some info about them.
	//do
	//{
	//	if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
	//	{
	//		std::basic_string<TCHAR> s(ffd.cFileName);
	//		LOG(Info, LOGPLACE, "%s\t<DIR>\n", s.c_str());
	//	}
	//	else
	//	{
	//		filesize.LowPart = ffd.nFileSizeLow;
	//		filesize.HighPart = ffd.nFileSizeHigh;
	//		std::basic_string<TCHAR> s(ffd.cFileName);
	//		LOG(Info, LOGPLACE, "%s\t%ld bytes\n", s.c_str(), filesize.QuadPart);
	//	}
	//}
	//while (FindNextFile(hFind, &ffd) != 0);
	//
	//dwError = GetLastError();
	//if (dwError != ERROR_NO_MORE_FILES)
	//{
	//	//DisplayErrorBox(TEXT("FindFirstFile"));
	//	LOG(Error, LOGPLACE, "Listing all files in a directory %s failed.", directoryPath.c_str());
	//}
	//
	//FindClose(hFind);
	////return dwError;


	LOG(Info, LOGPLACE, "Listing all files in a directory %s", directoryPath.c_str());
	if (directoryPath.length() > MAX_PATH - 3) // Check that the input path plus 3 is not longer than MAX_PATH. Three characters are for the "\*" plus NULL appended below.
	{
		LOG(Error, LOGPLACE, "Cannot list the files in a given directory. The directory path is too long");
		return std::vector<std::string>();
	}
	std::vector<std::string> filenames;
	DIR* dir;
	struct dirent* ent;
	if ((dir = opendir(directoryPath.c_str())) != NULL)
	{
		// print all files and directories within directory
		/* 
		 * Possible values for d_type variable:
		 * #define DT_UNKNOWN  0
		 * #define DT_REG      S_IFREG
		 * #define DT_DIR      S_IFDIR
		 * #define DT_FIFO     S_IFFIFO
		 * #define DT_SOCK     S_IFSOCK
		 * #define DT_CHR      S_IFCHR
		 * #define DT_BLK      S_IFBLK
		 */
		while ((ent = readdir(dir)) != NULL)
		{
			if (ent->d_type == DT_REG) // is regular file
			{
				LOG(Debug, LOGPLACE, "%s", ent->d_name);
				filenames.push_back(ent->d_name);
			}
		}
		closedir(dir);
	}
	else
	{
		LOG(Error, LOGPLACE, "Could not open directory \"%s\"", directoryPath.c_str());
	}
	return filenames;
}