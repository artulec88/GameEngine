#include "stdafx.h"
#include "Command.h"
#include <iostream>
#include <sstream>
#include "Time.h"

//#define LOGMODULE "Command"
#include "Logger.h"

using namespace Utility;
using namespace std;

//Command::Command()
//{
//}

Command::Command(int argc, char* argv[]) : ICommand()
{
	ASSERT(argv != NULL);
	Fill(argc, argv);
}

//Command::Command(const std::string& str)
//{
//	ASSERT(str.length() > 0);
//	Read(str); // TODO: Command::Read() returns bool. Add checking the bool value
//}
//
//Command::Command(const char* str)
//{
//	ASSERT(str != NULL);
//	Read(string(str)); // TODO: Command::Read() returns bool. Add checking the bool value
//}

Command::~Command()
{
	LOG(Delocust, LOGPLACE, "Command object gets destroyed");
	commands.clear();
}

void Command::Fill(int argc, char* argv[])
{
	ASSERT(argc >= 1);
	ASSERT(argv != NULL);
	for (int i = 0; i < argc; ++i)
	{
		commands.push_back(string(argv[i]));
	}
}

//bool Command::Read(std::istream& stream)
//{
//	string str;
//	if (std::getline(stream, str))
//	{
//		return Read(str);
//	}
//	else
//	{
//		return false;
//	}
//}

//bool Command::Read(const string& line)
//{
//	bool result = false;
//	std::stringstream s(line);
//	while (!s.eof())
//	{
//		std::string term;
//		s >> term;
//		if (term != "")
//		{
//			commands.push_back(term);
//			result = true;
//		}
//	}
//	return result;
//}

bool Command::IsArgPosCorrect(int number) const
{
	return ( (number >= 0) && (number < Size()) );
}

//template<typename Type>
//Type Command::Get(int number, const Type& defaultValue) const
//{
//	if (!IsArgPosCorrect(number))
//		return defaultValue;
//
//	Type value;
//	std::stringstream s(commands[number]);
//	s >> value;
//	return value;
//}

// template specialization
//template<> string Command::Get(int number, const string& defaultValue) const
//{
//	return (IsArgPosCorrect(number) ? commands[number] : defaultValue);
//}

string Command::Get(const string& opt, const string& defaultValue) const
{
	int pos = GetArgPosition(opt);
	if ( (pos < 0) || (pos > Size() - 2) )
	{
		return defaultValue;
	}
	pos = pos + 1;
	return (IsArgPosCorrect(pos) ? commands[pos] : defaultValue);
	//return Get<string>(pos + 1, defaultValue);
}

//int Command::Get(const string& opt, int defaultValue) const
//{
//	int pos = GetNumber(opt);
//	if ( (pos < 0) || (pos > Size() - 2) )
//	{
//		return defaultValue;
//	}
//	return Get<int>(pos + 1, defaultValue);
//}

int Command::GetArgPosition(const std::string& opt, int first /* = 0*/) const
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

bool Command::IsPresent(const std::string& opt) const
{
	return GetArgPosition(opt) != -1; // TODO: this should be compared to INVALID_VALUE and not -1
}