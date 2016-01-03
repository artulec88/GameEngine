#include "stdafx.h"
#include "StringUtility.h"
#include "ILogger.h"

void Utility::StringUtility::LeftTrim(std::string& s)
{
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
}

void Utility::StringUtility::RightTrim(std::string& s)
{
	s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
}

//void Utility::StringUtility::Trim(std::string& s)
//{
//	RightTrim(s);
//	LeftTrim(s);
//}

std::string Utility::StringUtility::Trim(const std::string& str, const std::string& trimStr /* = std::string(" \t\n") */)
{
	if (trimStr.size() == 0)
	{
		return str;
	}

	std::string::size_type begPos = str.find_first_not_of(trimStr);
	if (begPos != std::string::npos)
	{
		std::string::size_type endPos = str.find_last_not_of(trimStr);
		return str.substr(begPos, endPos - begPos + 1);
	}
	return "";
}

void Utility::StringUtility::FindAndReplace(std::string& source, const std::string& find, const std::string& replace)
{
	size_t j;
	for (; (j = source.find(find)) != std::string::npos; )
	{
		source.replace(j, find.length(), replace);
	}
}

std::string Utility::StringUtility::MakeUppercase(const std::string& str)
{
	std::string temp = str;
	std::transform(temp.begin(), temp.end(), temp.begin(), ::toupper);
	return temp;
}

std::string Utility::StringUtility::MakeLowercase(const std::string& str)
{
	std::string temp = str;
	std::transform(temp.begin(), temp.end(), temp.begin(), ::tolower);
	return temp;
}

bool Utility::StringUtility::Contains(const std::string& source, const char* find)
{
	return (strstr(source.c_str(), find) != 0);
}

std::string Utility::StringUtility::Pad(const std::string& str, char padChar, int length)
{
	std::string result = str;
	for (int i = static_cast<int>(result.length()); i < length; ++i)
	{
		result += padChar;
	}
	return result;
}

int Utility::StringUtility::ToInt(const std::string& str)
{
	int result = 0;
	std::string inString = Trim(str);
	if (!FromString<int>(result, inString, std::dec))
	{
		ERROR_LOG("Failed to convert \"%s\" to integer. Default 0 will be returned.", inString.c_str());
		return 0;
	}

	bool isPositive = (inString[0] != '-');
	for (unsigned int j = 0; j < inString.length(); ++j)
	{
		if ((j != 0 || isPositive) && !IsNumeric(inString[j]))
		{
			ERROR_LOG("Failed to convert \"%s\" to integer. The character %c is not numeric. Default 0 will be returned.", inString.c_str());
			return 0;
		}
	}
	return result;
}

float Utility::StringUtility::ToFloat(const std::string& str)
{
	CRITICAL_LOG("This function is not yet implemented correctly.");

	float result = 0.0f;
	std::string inString = Trim(str);
	if (!FromString<float>(result, inString, std::dec))
	{
		ERROR_LOG("Failed to convert \"%s\" to floating-point number. Default 0.0 will be returned.", inString.c_str());
		return 0;
	}

	const char decimalMark = '.';
	bool decimalMarkFound = false;
	bool isNegative = (inString[0] == '-');
	for (unsigned int j = 0; j < inString.length(); ++j)
	{
		if (!IsNumeric(inString[j]))
		{
			if (j == 0 && isNegative)
			{
				continue;
			}
			else if (inString[j] == decimalMark && decimalMarkFound)
			{
				ERROR_LOG(" More than one decimal mark %c found in string \"%s\". The surplus decimal marks will be ignored.", decimalMark, inString.c_str());
				continue;
			}
			else
			{
				ERROR_LOG("Failed to convert \"%s\" to integer. The character %c is not numeric. Default 0 will be returned.", inString.c_str());
				return 0;
			}
		}
	}
	return result;
}

bool Utility::StringUtility::IsNumeric(char c)
{
	return ('0' <= c && c <= '9');
}