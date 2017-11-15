#ifndef __UTILITY_FILE_MANAGER_H__
#define __UTILITY_FILE_MANAGER_H__

#include "Utility.h"
#include "ILogger.h"
#include <string>
#include <vector>

namespace utility
{

	/// <summary>
	/// This class is just a simple wrapper and provides the interface for listing the files in a given directory.
	/// In the future this class may become a resource manager for handling, loading, allocating, deallocating etc. the files from hard drive.
	/// </summary>
	class FileManager
	{
		/* ==================== Static variables and functions begin ==================== */
	public:
		// TODO: This function should probably be made non-static.
		template <typename T>
		static T AttemptRead(std::istream& stream)
		{
			T out;
			if (!(stream >> out))
			{
				EMERGENCY_LOG_UTILITY("Failed to read a required value from the stream.");
			}
			return out;
		}
		/* ==================== Static variables and functions end ==================== */

		/* ==================== Constructors and destructors begin ==================== */
		/// <summary> File manager constructor. </summary>
		UTILITY_API FileManager();

		/// <summary> File manager destructor. </summary>
		UTILITY_API ~FileManager();

		/// <summary> File manager copy constructor. </summary>
		/// <param name="fileManager"> The file manager to copy construct from. </param>
		FileManager(const FileManager& fileManager) = delete;

		/// <summary> File manager move constructor. </summary>
		/// <param name="fileManager"> The file manager to move construct from. </param>
		FileManager(FileManager&& fileManager) noexcept = delete;

		/// <summary> File manager copy assignment operator. </summary>
		/// <param name="fileManager"> The file manager to copy assign from. </param>
		/// <returns> The reference to the newly copy-assigned file manager. </returns>
		FileManager& operator=(const FileManager& fileManager) = delete;

		/// <summary> File manager move assignment operator. </summary>
		/// <param name="fileManager"> The file manager to move assign from. </param>
		/// <returns> The reference to the newly move-assigned file manager. </returns>
		FileManager& operator=(FileManager&& fileManager) noexcept = delete;
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
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

} /* end namespace utility */

#endif /* __UTILITY_FILE_MANAGER_H__ */