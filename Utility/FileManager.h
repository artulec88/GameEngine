#ifndef __UTILITY_FILE_MANAGER_H__
#define __UTILITY_FILE_MANAGER_H__

#include "Utility.h"
#include <string>
#include <vector>

namespace Utility
{

/// <summary>
/// This class is just a simple wrapper and provides the interface for listing the files in a given directory.
/// In the future this class may become a resource manager for handling, loading, allocating, deallocating etc. the files from hard drive.
/// </summary>
class FileManager
{
/* ==================== Static variables and functions begin ==================== */
/* ==================== Static variables and functions end ==================== */

/* ==================== Constructors and destructors begin ==================== */
public:
	UTILITY_API FileManager();
	UTILITY_API ~FileManager(void);
private:
	FileManager(FileManager& fileManager) {} // don't implement
	void operator=(FileManager& fileManager) {} // don't implement
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static member functions begin ==================== */
public:
	/// <summary>
	/// Lists all files in a specified directory given as parameter <paramref name="directoryPath"/>.
	/// The second parameter <paramref name="isRecursiveSearchEnabled"/> specifies whether the result should contain files from the subdirectories.
	/// </summary>
	/// <param name="directoryPath">The directory the user wants to list files from.</param>
	/// <param name="isRecursiveSearchEnabled">
	/// <code>True</code> if the user wants to list files from subdirectories too.
	/// Otherwise it is set to <code>false</code>. Default value is <code>false</code>.
	/// </param>
	/// <returns>
	/// A vector of file names located in the specified directory with files from subdirectories added or not depending on the value of the parameter <paramref name="isRecursiveSearchEnabled"/>.
	/// </returns>
	UTILITY_API std::vector<std::string> ListAllFilesInDirectory(const std::string& directoryPath, bool isRecursiveSearchEnabled = false) const;
/* ==================== Non-static member functions end ==================== */

/* ==================== Non-static member variables begin ==================== */
/* ==================== Non-static member variables end ==================== */
}; /* end class FileManager */

} /* end namespace Utility */

#endif /* __UTILITY_FILE_MANAGER_H__ */