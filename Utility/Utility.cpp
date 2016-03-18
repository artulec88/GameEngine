// Utility.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "Utility.h"
#include <sstream>
#include "ILogger.h"
#include "Logger.h"
#include "StringUtility.h"

using namespace std;

void Utility::CutToTokens(const string& str, vector<string>& tokens, const char delim)
{
	Utility::CutToTokens(str, tokens, &delim, 1);

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

void Utility::CutToTokens(const string& str, vector<string>& tokens, const char* delim, int delimCount)
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
	const char QUOTE_SIGN = '"';
    const char* cstr = str.c_str();
    unsigned int strLength = static_cast<unsigned int>(str.length());
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
		StringUtility::Trim(token);
		if (!token.empty() || quoteFound)
		{
			quoteFound = false;
			tokens.push_back(token);
		}
        start = end + 1;
        end = start;
    }
}