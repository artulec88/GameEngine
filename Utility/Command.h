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
protected: // member variables
	std::vector<std::string> commands;

public: // constructors and destructors
	//Command();
	//Command(std::istream& stream);
	//Command(const std::string& str);
	//Command(const char* str);
	Command(int argc, char* argv[]);
	~Command();

public:
	int Size() const { return static_cast<int>(commands.size()); };
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
}; /* end class Command */

} /* end namespace Utility */