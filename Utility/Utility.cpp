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
    const char* cstr = str.c_str();
    unsigned int strLength = static_cast<unsigned int>(str.length());
    unsigned int start = 0;
    unsigned int end = 0;
        
    while(end <= strLength)
    {
        while(end <= strLength)
        {
			bool delimFound = false;
			for (int i = 0; i < delimCount; ++i)
			{
				if(cstr[end] == delim[i])
				{
					delimFound = true;
					break;
				}
			}
			if (delimFound)
			{
				break;
			}
            end++;
        }
		//std::string token;
		//Trim(str.substr(start, end - start), token);
		std::string token = str.substr(start, end - start);
		StringUtility::Trim(token);
		tokens.push_back(token);
        start = end + 1;
        end = start;
    }
}