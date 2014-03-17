// Utility.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "Utility.h"
#include <sstream>

using namespace std;

void Utility::CutToTokens(const string& str, vector<string>& tokens)
{
	stringstream ss(str);
	string temp = "";
	ss >> temp;
	while (temp != "")
	{
		tokens.push_back(temp);
		temp = "";
		ss >> temp;
	}
}