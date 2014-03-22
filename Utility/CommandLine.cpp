#include "stdafx.h"
#include "CommandLine.h"
#include <iostream>
#include <sstream>
#include "Time.h"

//#define LOGMODULE "commandline"
#include "Log.h"

using namespace Utility;
using namespace std;

CommandLine::CommandLine()
{
}

CommandLine::CommandLine(int argc, char* argv[])
{
	ASSERT(argv != NULL);
	Fill(argc, argv);
}

CommandLine::CommandLine(const std::string& str)
{
	ASSERT(str.length() > 0);
	Read(str); // TODO: CommandLine::Read() returns bool. Add checking the bool value
}

CommandLine::CommandLine(const char* str)
{
	ASSERT(str != NULL);
	Read(string(str)); // TODO: CommandLine::Read() returns bool. Add checking the bool value
}

CommandLine::~CommandLine()
{
	stdlog(Delocust, LOGPLACE, "CommandLine object gets destroyed");
	commands.clear();
}

void CommandLine::Fill(int argc, char* argv[])
{
	ASSERT(argc >= 1);
	ASSERT(argv != NULL);
	for (int i = 0; i < argc; ++i)
	{
		commands.push_back(string(argv[i]));
	}
}

bool CommandLine::Read(std::istream& stream)
{
	string str;
	if (std::getline(stream, str))
	{
		return Read(str);
	}
	else
	{
		return false;
	}
}

bool CommandLine::Read(const string& line)
{
	bool result = false;
	std::stringstream s(line);
	while (!s.eof())
	{
		std::string term;
		s >> term;
		if (term != "")
		{
			commands.push_back(term);
			result = true;
		}
	}
	return result;
}

int CommandLine::Size() const
{
	return static_cast<int>(commands.size());
}

bool CommandLine::IsNumberCorrect(int number) const
{
	return ( (number >= 0) && (number < Size()) );
}

template<typename Type>
Type CommandLine::Get(int number, const Type& defaultValue) const
{
	if (!IsNumberCorrect(number))
		return defaultValue;

	Type value;
	std::stringstream s(commands[number]);
	s >> value;
	return value;
}

// template specialization
template<> string CommandLine::Get(int number, const string& defaultValue) const
{
	return (IsNumberCorrect(number) ? commands[number] : defaultValue);
}

string CommandLine::Get(const string& opt, const string& defaultValue) const
{
	int pos = GetNumber(opt);
	if ( (pos < 0) || (pos > Size() - 2) )
	{
		return defaultValue;
	}
	return Get<string>(pos + 1, defaultValue);
}

int CommandLine::Get(const string& opt, int defaultValue) const
{
	int pos = GetNumber(opt);
	if ( (pos < 0) || (pos > Size() - 2) )
	{
		return defaultValue;
	}
	return Get<int>(pos + 1, defaultValue);
}

int CommandLine::GetNumber(const std::string& opt, int first /* = 0*/) const
{
	//ASSERT(first >= 0);
	int number = first;
	for (std::vector<std::string>::const_iterator itr = commands.begin() + first; itr != commands.end(); ++itr, ++number)
	{
		//ASSERT(number >= first);
		if ((*itr) == opt)
		{
			return number;
		}
	}
	return -1; // TODO: this should not return -1, but rather INVALID_VALUE constant
}

bool CommandLine::IsPresent(const std::string& opt) const
{
	return GetNumber(opt) != -1; // TODO: this should be compared to INVALID_VALUE and not -1
}