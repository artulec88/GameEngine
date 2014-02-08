#pragma once

#include <vector>
#include "Utility.h"

namespace Utility
{

	class UTILITY_API CommandLine : public std::vector<std::string>
	{
	public: // constructors and destructors
		CommandLine();
		CommandLine(std::istream& stream);
		CommandLine(const std::string& str);
		CommandLine(const char* str);
		CommandLine(int argc, char* argv[]);
		~CommandLine();

	public:
		bool IsNumberCorrect(int number) const;
		bool Read(std::istream& stream);
		bool Read(const std::string& line);
		std::string Get(const std::string& opt, const std::string& defaultValue) const;
		int Get(const std::string& opt, int defaultValue) const;
		int GetNumber(const std::string& opt, int first = 0) const;
		bool IsPresent(const std::string& opt) const;
		void Fill(int argc, char* argv[]);

	protected:
		template<typename Type> Type Get(int number, const Type& defaultValue) const;
	}; /* end class CommandLine */

} /* end namespace Utils */