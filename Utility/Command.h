#pragma once

//#pragma warning(disable:4251)

#include <vector>
#include <string>
#include "Utility.h"
#include "ICommand.h"

namespace Utility
{

class Command : public ICommand
{
/* ==================== Constructors and destructors begin ==================== */
public:
	//Command();
	//Command(std::istream& stream);
	//Command(const std::string& str);
	//Command(const char* str);
	Command(int argc, char* argv[]);
	~Command();
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static member functions begin ==================== */
public:
	int Size() const { return static_cast<int>(m_commands.size()); };
	bool IsArgPosCorrect(int number) const;
	//bool Read(std::istream& stream);
	//bool Read(const std::string& line);
	std::string Get(const std::string& opt, const std::string& defaultValue) const;
	//int Get(const std::string& opt, int defaultValue) const;
	bool GetArgPosition(const std::string& opt, int& argPos, int first = 0) const;
	bool IsPresent(const std::string& opt) const;
	void Fill(int argc, char* argv[]);
protected:
	//template<typename Type> Type Get(int number, const Type& defaultValue) const;
/* ==================== Non-static member functions end ==================== */

/* ==================== Non-static member variables begin ==================== */
protected:
	// TODO: Check if Array<std::string> is a better solution than using STL vector
	//Container
	std::vector<std::string> m_commands;
/* ==================== Non-static member variables end ==================== */
}; /* end class Command */

} /* end namespace Utility */