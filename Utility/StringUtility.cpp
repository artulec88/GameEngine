#include "stdafx.h"
#include "StringUtility.h"
#include "ILogger.h"

void utility::string_utility::LeftTrim(std::string& s)
{
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
}

void utility::string_utility::RightTrim(std::string& s)
{
	s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
}

//void Utility::StringUtility::Trim(std::string& s)
//{
//	RightTrim(s);
//	LeftTrim(s);
//}

std::string utility::string_utility::Trim(const std::string& str, const std::string& trimStr /* = std::string(" \t\n") */)
{
	if (trimStr.size() == 0)
	{
		return str;
	}

	const std::string::size_type begPos = str.find_first_not_of(trimStr);
	if (begPos != std::string::npos)
	{
		const std::string::size_type endPos = str.find_last_not_of(trimStr);
		return str.substr(begPos, endPos - begPos + 1);
	}
	return "";
}

void utility::string_utility::FindAndReplace(std::string& source, const std::string& find, const std::string& replace)
{
	size_t j;
	for (; (j = source.find(find)) != std::string::npos; )
	{
		source.replace(j, find.length(), replace);
	}
}

std::string utility::string_utility::MakeUppercase(const std::string& str)
{
	std::string temp = str;
	std::transform(temp.begin(), temp.end(), temp.begin(), ::toupper);
	return temp;
}

std::string utility::string_utility::MakeLowercase(const std::string& str)
{
	std::string temp = str;
	std::transform(temp.begin(), temp.end(), temp.begin(), ::tolower);
	return temp;
}

bool utility::string_utility::Contains(const std::string& source, const char* find)
{
	return (strstr(source.c_str(), find) != nullptr);
}

std::string utility::string_utility::Pad(const std::string& str, const char padChar, const int length)
{
	std::string result = str;
	for (int i = static_cast<int>(result.length()); i < length; ++i)
	{
		result += padChar;
	}
	return result;
}

int utility::string_utility::ToInt(const std::string& str)
{
	int result = 0;
	std::string inString = Trim(str);
	if (!FromString<int>(result, inString, std::dec))
	{
		ERROR_LOG_UTILITY("Failed to convert \"", inString, "\" to integer. Default 0 will be returned.");
		return 0;
	}

	const bool isPositive = (inString[0] != '-');
	for (unsigned int j = 0; j < inString.length(); ++j)
	{
		if ((j != 0 || isPositive) && !IsNumeric(inString[j]))
		{
			ERROR_LOG_UTILITY("Failed to convert \"", inString, "\" to integer. The character \"", inString[j], "\" is not numeric. Default 0 will be returned.");
			return 0;
		}
	}
	return result;
}

float utility::string_utility::ToFloat(const std::string& str)
{
	WARNING_LOG_UTILITY("This function has not been tested yet.");

	float result = 0.0f;
	std::string inString = Trim(str);
	if (!FromString<float>(result, inString, std::dec))
	{
		ERROR_LOG_UTILITY("Failed to convert \"", inString, "\" to floating-point number. Default 0.0f will be returned.");
		return 0.0f;
	}

	const char decimalMark = '.';
	const bool decimalMarkFound = false; // TODO: This is weird. It causes the below else if statement unreachable.
	const bool isNegative = (inString[0] == '-');
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
				ERROR_LOG_UTILITY("More than one decimal mark \"", decimalMark, "\" found in the string \"", inString, "\". The surplus decimal marks will be ignored.");
				continue;
			}
			else
			{
				ERROR_LOG_UTILITY("Failed to convert \"", inString, "\" to integer. The character \"", inString[j], "\" %c is not numeric. Default 0.0f will be returned.");
				return 0.0f;
			}
		}
	}
	return result;
}

bool utility::string_utility::IsNumeric(const char c)
{
	return ('0' <= c && c <= '9');
}

void utility::string_utility::CutToTokens(const std::string& str, std::vector<std::string>& tokens, const char delim)
{
	CutToTokens(str, tokens, &delim, 1);

	// const char* cstr = str.c_str();
	// unsigned int strLength = static_cast<unsigned int>(str.length());
	// unsigned int start = 0;
	// unsigned int end = 0;
	//     
	// while(end <= strLength)
	// {
	//     while(end <= strLength)
	//     {
	//if(cstr[end] == delim)
	//{
	//	break;
	//}
	//         end++;
	//     }
	//         
	//     tokens.push_back(str.substr(start, end - start));
	//     start = end + 1;
	//     end = start;
	// }
}

void utility::string_utility::CutToTokens(const std::string& str, std::vector<std::string>& tokens, const char* delim, const int delimCount)
{
	//stringstream ss(str);
	//string temp = "";
	//ss >> temp;
	//while (temp != "")
	//{
	//	tokens.push_back(temp);
	//	temp = "";
	//	ss >> temp;
	//}

	if (str.empty())
	{
		return;
	}

	const char QUOTE_SIGN = '"';
	const char* cstr = str.c_str();
	const unsigned int strLength = static_cast<unsigned int>(str.length());
	unsigned int start = 0;
	unsigned int end = 0;

	//std::cout << "str = \"" << str << "\"." << std::endl;
	bool quoteFound = false;
	while (end <= strLength)
	{
		while (end <= strLength)
		{
			if (cstr[end] == QUOTE_SIGN)
			{
				++start; // to omit the QUOTE_SIGN
				for (end = end + 1; end < strLength; ++end)
				{
					if (cstr[end] == QUOTE_SIGN)
					{
						quoteFound = true;
						break;
					}
				}
				if (quoteFound)
				{
					break;
				}
			}
			else
			{
				bool delimFound = false;
				for (int i = 0; i < delimCount; ++i)
				{
					if (cstr[end] == delim[i])
					{
						delimFound = true;
						break;
					}
				}
				if (delimFound)
				{
					break;
				}
			}
			++end;
		}
		//std::string token;
		//Trim(str.substr(start, end - start), token);
		std::string token = str.substr(start, end - start);
		Trim(token);
		if (!token.empty() || quoteFound)
		{
			quoteFound = false;
			tokens.push_back(token);
		}
		start = end + 1;
		end = start;
	}
}