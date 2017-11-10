#include "StdAfx.h"
#include "FileManager.h"
#include "ILogger.h"

#ifdef _WIN32
#include <windows.h>
#include <tchar.h> 
#include <stdio.h>
#include <strsafe.h>
#endif
//#include <dirent.h>

Utility::FileManager::FileManager()
{
}


Utility::FileManager::~FileManager()
{
}

std::vector<std::string> Utility::FileManager::ListAllFilesInDirectory(const std::string& directoryPath, bool isRecursiveSearchEnabled /* = false */) const
{
	DEBUG_LOG_UTILITY("Listing all files in a directory \"", directoryPath, "\" (recursive search enabled = ", isRecursiveSearchEnabled, ")");
	std::vector<std::string> filenames;
#ifdef _WIN32
	// Prepare directoryPath string for use with FindFile functions. First, copy the string to a buffer, then append '\*' to the directory name.
	TCHAR szDir[MAX_PATH];
	StringCchCopy(szDir, MAX_PATH, std::wstring(directoryPath.begin(), directoryPath.end()).c_str());
	StringCchCat(szDir, MAX_PATH, TEXT("\\*"));

	// Find the first file in the directory
	WIN32_FIND_DATA ffd;
	HANDLE hFind = FindFirstFile(szDir, &ffd);
	if (INVALID_HANDLE_VALUE == hFind)
	{
		ERROR_LOG_UTILITY("Cannot list files in a directory \"", directoryPath, "\". FindFirstFile() function failed.");
		exit(EXIT_FAILURE);
	}

	// List all the files in the directory with some information about them.
	LARGE_INTEGER fileSize;
	do
	{
		if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			// TODO: Recursive search
			if (isRecursiveSearchEnabled)
			{
				WARNING_LOG_UTILITY("The recursive search mode is not yet supported.");
			}
		}
		else
		{
			std::wstring filename(ffd.cFileName);
			fileSize.LowPart = ffd.nFileSizeLow;
			fileSize.HighPart = ffd.nFileSizeHigh;
			filenames.push_back(std::string(filename.begin(), filename.end()));
		}
	} while (FindNextFile(hFind, &ffd) != 0);

	DWORD dwError = GetLastError();
	if (dwError != ERROR_NO_MORE_FILES)
	{
		ERROR_LOG_UTILITY("No more files.");
	}

	FindClose(hFind);
#endif
	//if (directoryPath.length() > MAX_PATH - 3) // Check that the input path plus 3 is not longer than MAX_PATH. Three characters are for the "\*" plus NULL appended below.
	//{
	//	ERROR_LOG_UTILITY("Cannot list the files in a given directory. The directory path is too long");
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
	//	EMERGENCY_LOG_UTILITY("Cannot list the files in a given directory. Invalid directory handle instance.");
	//	//return dwError;
	//}
	//
	//// List all the files in the directory with some info about them.
	//do
	//{
	//	if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
	//	{
	//		std::basic_string<TCHAR> s(ffd.cFileName);
	//		INFO_LOG_UTILITY("\"", s, "\"\t<DIR>\n");
	//	}
	//	else
	//	{
	//		filesize.LowPart = ffd.nFileSizeLow;
	//		filesize.HighPart = ffd.nFileSizeHigh;
	//		std::basic_string<TCHAR> s(ffd.cFileName);
	//		INFO_LOG_UTILITY("\"", s, "\"\t", filesize.QuadPart, " bytes\n");
	//	}
	//}
	//while (FindNextFile(hFind, &ffd) != 0);
	//
	//dwError = GetLastError();
	//if (dwError != ERROR_NO_MORE_FILES)
	//{
	//	//DisplayErrorBox(TEXT("FindFirstFile"));
	//	ERROR_LOG_UTILITY("Listing all files in a directory \"", directoryPath, "\" failed.");
	//}
	//
	//FindClose(hFind);
	////return dwError;


	//INFO_LOG_UTILITY("Listing all files in a directory \"", directoryPath, "\".");
	//if (directoryPath.length() > MAX_PATH - 3) // Check that the input path plus 3 is not longer than MAX_PATH. Three characters are for the "\*" plus NULL appended below.
	//{
	//	ERROR_LOG_UTILITY("Cannot list the files in a given directory. The directory path is too long");
	//	return std::vector<std::string>();
	//}
	//std::vector<std::string> filenames;
	//DIR* dir;
	//struct dirent* ent;
	//if ((dir = opendir(directoryPath.c_str())) != nullptr)
	//{
	//	// print all files and directories within directory
	//	/* 
	//	 * Possible values for d_type variable:
	//	 * #define DT_UNKNOWN  0
	//	 * #define DT_REG      S_IFREG
	//	 * #define DT_DIR      S_IFDIR
	//	 * #define DT_FIFO     S_IFFIFO
	//	 * #define DT_SOCK     S_IFSOCK
	//	 * #define DT_CHR      S_IFCHR
	//	 * #define DT_BLK      S_IFBLK
	//	 */
	//	while ((ent = readdir(dir)) != nullptr)
	//	{
	//		if (ent->d_type == DT_REG) // is regular file
	//		{
	//			DEBUG_LOG_UTILITY(ent->d_name);
	//			filenames.push_back(ent->d_name);
	//		}
	//	}
	//	closedir(dir);
	//}
	//else
	//{
	//	ERROR_LOG_UTILITY("Could not open directory \"", directoryPath, "\".");
	//}
	return filenames;
}