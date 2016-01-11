#ifndef __UTILITY_STRING_UTILITY_H__
#define __UTILITY_STRING_UTILITY_H__

#include "Utility.h"
#include <string>
#include <sstream>

namespace Utility { namespace StringUtility
{
	template <class T>
	bool FromString(T &t, const std::string& str, std::ios_base& (*f)(std::ios_base&))
	{
		std::istringstream iss(str);
		return !(iss >> f >> t).fail();
	}

	/// <summary>
	/// Trims the given string <paramref name="s"/> from its start.
	/// </summary>
	void LeftTrim(std::string& s);

	/// <summary>
	/// Trims the given string <paramref name="s"/> at its end.
	/// </summary>
	void UTILITY_API RightTrim(std::string& s);

	/// <summary>
	/// Trims the given string <paramref name="s"/> from both ends.
	/// </summary>
	//void Trim(std::string& s);

	/// <summary>
	/// Trims the given characters from the beginning and end of a given string <paramref name="str"/>. The default is to trim whitespace.
	/// If the string is empty or contains only the trim characters, an empty string is returned.
	/// </summary>
	std::string Trim(const std::string& str, const std::string& trimStr = std::string(" \t\n"));

	/// <summary>
	/// Find the given string in the source string and replace it with the <paramref name="replace/> string, everywhere instances of that string exist.
	/// </summary>
	void FindAndReplace(std::string& source, const std::string& find, const std::string& replace);

	/// <summary>
	/// The following function returns a given <paramref name="str"/> string in uppercase.
	/// </summary>
	std::string MakeUppercase(const std::string& str);

	/// <summary>
	/// The following function returns a given <paramref name="str"/> string in lowercase.
	/// </summary>	
	std::string MakeLowercase(const std::string& str);

	/// <summary>
	/// Returns <code>true</code> if specified <paramref name="source"/> contains a given <paramref name="find"/> text. Otherwise, <code>false</code> is returned.
	/// </summary>
	bool Contains(const std::string& source, const char* find);

	std::string Pad(const std::string& str, char padChar, int length);

	/// <summary>
	/// Converts the specified <paramref name="str"/> to an integer. Note that a string exception is thrown if it is invalid.
	/// </summary>
	int ToInt(const std::string& str);

	/// <summary>
	/// Converts the specified <paramref name="str"/> to a floating-point number. Note that a string exception is thrown if it is invalid.
	/// </summary>
	float ToFloat(const std::string& str);

	/// <summary> Returns <code>true</code> if the specified character <paramref name="c"/> is numeric. </summary>
	bool IsNumeric(char c);
} /* end namespace StringUtility */

} /* end namespace Utility */

#endif /* __UTILITY_STRING_UTILITY_H__ */