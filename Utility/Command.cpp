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

Command::Command(int argc, char* argv[]) : ICommand(argc, argv)
{
	DELOCUST_LOG("Command constructor");
	CHECK_CONDITION(argv != NULL, Utility::Error, "Incorrect program parameters specified.");
	Fill(argc, argv);
}

//Command::Command(const std::string& str)
//{
//	CHECK_CONDITION(str.length() > 0, Utility::Error, "Incorrect Command constructor parameter specified \"%s\"", str.c_str());
//	Read(str); // TODO: Command::Read() returns bool. Add checking the bool value
//}
//
//Command::Command(const char* str)
//{
//	CHECK_CONDITION(str != NULL, Utility::Error, "Incorrect Command constructor parameter specified.");
//	Read(string(str)); // TODO: Command::Read() returns bool. Add checking the bool value
//}

Command::~Command()
{
	DELOCUST_LOG("Command object gets destroyed");
	m_commands.clear();
}

void Command::Fill(int argc, char* argv[])
{
	CHECK_CONDITION(argc >= 1, Utility::Error, "Incorrect number of program parameters specified (%d)", argc);
	CHECK_CONDITION(argv != NULL, Utility::Error, "Incorrect program parameters specified.");
	for (int i = 0; i < argc; ++i)
	{
		m_commands.push_back(string(argv[i]));
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
//			m_commands.push_back(term);
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
//	std::stringstream s(m_commands[number]);
//	s >> value;
//	return value;
//}

// template specialization
//template<> string Command::Get(int number, const string& defaultValue) const
//{
//	return (IsArgPosCorrect(number) ? m_commands[number] : defaultValue);
//}

string Command::Get(const string& opt, const string& defaultValue) const
{
	int pos;
	if ( !GetArgPosition(opt, pos) || (pos < 0) || (pos > Size() - 2) )
	{
		return defaultValue;
	}
	pos = pos + 1;
	return (IsArgPosCorrect(pos) ? m_commands[pos] : defaultValue);
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

/**
 * \return True when the argument opt is found in the commands vector and False otherwise
 */
bool Command::GetArgPosition(const std::string& opt, int& argPos, int first /* = 0*/) const
{
	int number = first;
	for (std::vector<std::string>::const_iterator itr = m_commands.begin() + first; itr != m_commands.end(); ++itr, ++number)
	{
		if ((*itr) == opt)
		{
			argPos = number;
			return true;
		}
	}
	argPos = -1;
	return false;
}

bool Command::IsPresent(const std::string& opt) const
{
	int pos;
	return GetArgPosition(opt, pos);
}