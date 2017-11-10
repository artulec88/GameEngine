#ifndef __UTILITY_ICOMMAND_LINE_MAPPER_H__
#define __UTILITY_ICOMMAND_LINE_MAPPER_H__

#include "Utility.h"
#include <memory>

namespace Utility
{
	/// <summary>
	/// The object giving the user an easy access to command-line parameters defined at the startup of the application.
	/// </summary>
	class ICommandLineMapper
	{
		/* ==================== Static variables and functions begin ==================== */
	public:
		/// <summary>
		/// Creates an <code>ICommandLineMapper</code> specific implementation object.
		/// </summary>
		/// <remarks>
		/// To hide the specific implementation from the client we provide the factory-like method to create specific command instances.
		/// </remarks>
		UTILITY_API static std::unique_ptr<Utility::ICommandLineMapper> CreateCommandLineMapper(int argc, char* argv[]);
		/* ==================== Static variables and functions end ==================== */

		/* ==================== Constructors and destructors begin ==================== */
	public:
		/// <summary>
		/// Command line mapper interface constructor.
		/// </summary>
		ICommandLineMapper();

		/// <summary>
		/// Command line mapper interface destructor.
		/// </summary>
		virtual ~ICommandLineMapper();

		/// <summary>
		/// Command line mapper interface copy constructor.
		/// </summary>
		/// <param name="commandLineMapper"> The reference to the command line mapper interface we want to copy construct from. </param>
		ICommandLineMapper(const ICommandLineMapper& commandLineMapper) = delete;

		/// <summary>
		/// Command line mapper interface move constructor.
		/// </summary>
		/// <param name="commandLineMapper"> The r-value reference to the command line mapper interface we want to move construct from. </param>
		ICommandLineMapper(ICommandLineMapper&& commandLineMapper) = delete;

		/// <summary>
		/// Command line mapper interface copy assignment operator.
		/// </summary>
		/// <param name="commandLineMapper"> The reference to the command line mapper interface we want to copy assign from. </param>
		/// <returns> The reference to the newly copy-assigned command line mapper interface object. </returns>
		ICommandLineMapper& operator=(const ICommandLineMapper& commandLineMapper) = delete;

		/// <summary>
		/// Command line mapper interface move assignment operator.
		/// </summary>
		/// <param name="commandLineMapper"> The r-value reference to the command line mapper interface we want to move assign from. </param>
		/// <returns> The reference to the newly move-assigned command line mapper interface object. </returns>
		ICommandLineMapper& operator=(ICommandLineMapper&& commandLineMapper) = delete;
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
	public:
		/// <summary>
		/// Gives a simple <code>true</code> or <code>false</code> answer to the question whether the specified program argument
		/// <paramref name="opt"/> is defined or not.
		/// </summary>
		/// <param name="opt"> The name of the program argument that we want to know if it is defined or not. </param>
		/// <returns>
		/// <code>true</code> if the program has been started with the specified value for the argument <paramref name="opt"/> and <code>false</code> otherwise.
		/// </returns>
		UTILITY_API virtual inline bool IsPresent(const std::string& opt) const = 0;

		/// <summary>
		/// Gives a value for the specified program argument <paramref name="opt"/> or returns <paramref name="defaultValue"/> if no such argument is found.
		/// </summary>
		/// <param name="opt"> The name of the program argument that we want to know the value of. </param>
		/// <returns>
		/// The specific value for the <paramref name="opt"/> program argument or <paramref name="defaultValue"/> if no such argument is found.
		/// </returns>
		UTILITY_API virtual const std::string& Get(const std::string& opt, const std::string& defaultValue) const = 0;

		//UTILITY_API virtual bool DoesContainRedundantArgs(const std::string* acceptableArgs, const int acceptableArgsCount) const = 0;
	//protected:
		//template<typename Type> Type Get(int number, const Type& defaultValue) const;
	/* ==================== Non-static member functions end ==================== */
	}; /* end class ICommandLineMapper */

} /* end namespace Utility */

#endif /* __UTILITY_ICOMMAND_LINE_MAPPER_H__ */