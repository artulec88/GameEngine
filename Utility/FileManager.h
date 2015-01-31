#ifndef UTILITY_FILE_MANAGER_H
#define UTILITY_FILE_MANAGER_H

#include "Utility.h"
#include <string>
#include <vector>

namespace Utility
{

/**
 * \brief This class is just a simple wrapper and provides the interface for listing the files in a given directory.
 * In the future this class may become a resource manager for handling, loading, allocating, deallocating etc. the files from hard drive.
 */
class UTILITY_API FileManager
{
/* ==================== Static variables and functions begin ==================== */
/* ==================== Static variables and functions end ==================== */

/* ==================== Constructors and destructors begin ==================== */
public:
	FileManager();
	~FileManager(void);
private:
	FileManager(FileManager& fileManager) {}
	void operator=(FileManager& fileManager) {}
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static member functions begin ==================== */
public:
	std::vector<std::string> ListAllFilesInDirectory(const std::string& directoryPath) const;
/* ==================== Non-static member functions end ==================== */

/* ==================== Non-static member variables begin ==================== */
/* ==================== Non-static member variables end ==================== */
}; /* end class FileManager */

} /* end namespace Utility */

#endif // UTILITY_FILE_MANAGER_H