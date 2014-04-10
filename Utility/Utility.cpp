// Utility.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "Utility.h"
#include <sstream>

using namespace std;

//void Utility::LeftTrim(const std::string& s, std::string& result)
//{
//	result = s;
//	result.erase(result.begin(), std::find_if(result.begin(), result.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
//}

std::string& Utility::LeftTrim(std::string& s)
{
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
	return s;
}

//void Utility::RightTrim(const std::string& s, std::string& result)
//{
//	result = s;
//	result.erase(std::find_if(result.rbegin(), result.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), result.end());
//}

std::string& Utility::RightTrim(std::string& s)
{
	s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
	return s;
}

//void Utility::Trim(const std::string& s, std::string& result)
//{
//	RightTrim(s, result);
//	const std::string copyStr = result;
//	LeftTrim(copyStr, result);
//}

std::string& Utility::Trim(std::string& s)
{
	return LeftTrim(RightTrim(s));
}

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
		tokens.push_back(Trim(str.substr(start, end - start)));
        start = end + 1;
        end = start;
    }
}