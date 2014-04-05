#include "StdAfx.h"
#include "OBJModel.h"

#include "Utility\Log.h"

#include <fstream>
#include <algorithm>
#include <map>
//#include <hash_map>
#include <unordered_map>

using namespace Rendering;
using namespace std;

OBJModel::OBJModel(const string& fileName)
{
	// TODO: Check whether the fileName is a full path or just a fileName. Act accordingly.
	hasUVs = false;
	hasNormals = false;
    std::ifstream file;
    file.open(fileName.c_str());
    if(!file.is_open())
	{
		stdlog(Utility::Error, LOGPLACE, "Unable to load model \"%s\"", fileName.c_str());
		return;
	}

    std::string line;
    while(file.good())
    {
        getline(file, line);
        
        size_t lineLength = line.length();
            
        if(lineLength < 2)
            continue;
            
        const char* lineCStr = line.c_str();
            
        switch(lineCStr[0])
        {
		case 'v':
            if(lineCStr[1] == 't')
                this->uvs.push_back(ParseOBJVec2(line));
            else if(lineCStr[1] == 'n')
                this->normals.push_back(ParseOBJVec3(line));
            else if(lineCStr[1] == ' ' || lineCStr[1] == '\t')
                this->vertices.push_back(ParseOBJVec3(line));
			else
			{
				stdlog(Utility::Error, LOGPLACE, "Line \"%s\" cannot be processed correctly", lineCStr);
			}
			break;
		case 'f':
			CreateOBJFace(line);
			break;
		case '#': // comment
			break;
		default:
			stdlog(Utility::Delocust, LOGPLACE, "line = \"%s\"", lineCStr);
			break;
        };
    }
}


OBJModel::~OBJModel(void)
{
	// TODO: Clear all vectors
	// TODO 2: Check how to properly deallocate vector of objects
	OBJIndices.clear();
	vertices.clear();
	uvs.clear();
	normals.clear();
}

unsigned int OBJModel::FindLastVertexIndex(const std::vector<OBJIndex*>& indexLookup, const OBJIndex* currentIndex, const IndexedModel& result) const
{
    unsigned int start = 0;
    unsigned int end = (unsigned int)indexLookup.size();
    unsigned int current = (end - start) / 2 + start;
    unsigned int previous = start;
    
    while(current != previous)
    {
        OBJIndex* testIndex = indexLookup[current];
        
        if(testIndex->vertexIndex == currentIndex->vertexIndex)
        {
            unsigned int countStart = current;
        
            for(unsigned int i = 0; i < current; i++)
            {
                OBJIndex* possibleIndex = indexLookup[current - i];
                
                if(possibleIndex == currentIndex)
                    continue;
                    
                if(possibleIndex->vertexIndex != currentIndex->vertexIndex)
                    break;
                    
                countStart--;
            }
            
            for(unsigned int i = countStart; i < indexLookup.size() - countStart; i++)
            {
                OBJIndex* possibleIndex = indexLookup[current + i];
                
                if(possibleIndex == currentIndex)
                    continue;
                    
                if(possibleIndex->vertexIndex != currentIndex->vertexIndex)
                    break;
                else if((!hasUVs || possibleIndex->uvIndex == currentIndex->uvIndex)
                    && (!hasNormals || possibleIndex->normalIndex == currentIndex->normalIndex))
                {
                    Math::Vector3D currentPosition = vertices[currentIndex->vertexIndex];
                    Math::Vector2D currentTexCoord;
                    Math::Vector3D currentNormal;
                    
                    if(hasUVs)
                        currentTexCoord = uvs[currentIndex->uvIndex];
                    else
                        currentTexCoord = Math::Vector2D(0,0);
                        
                    if(hasNormals)
                        currentNormal = normals[currentIndex->normalIndex];
                    else
                        currentNormal = Math::Vector3D(0,0,0);
                    
                    for(unsigned int j = 0; j < result.PositionsSize(); j++)
                    {
                        if(currentPosition == result.GetPosition(j)
                            && ((!hasUVs || currentTexCoord == result.GetTexCoord(j))
                            && (!hasNormals || currentNormal == result.GetNormal(j))))
                        {
                            return j;
                        }
                    }
                }
            }
        
            return (unsigned int)-1;
        }
        else
        {
            if(testIndex->vertexIndex < currentIndex->vertexIndex)
                start = current;
            else
                end = current;
        }
    
        previous = current;
        current = (end - start) / 2 + start;
    }
    
    return (unsigned int)-1;
}

template <class T>
struct Hash;

template<>
struct Hash<OBJIndex>
{
	std::size_t operator()(OBJIndex const& objIndex) const
	{
		const int BASE = 17;
		const int MULTIPLIER = 31;
		size_t result = BASE;
		result = MULTIPLIER * result + objIndex.vertexIndex;
		result = MULTIPLIER * result + objIndex.uvIndex;
		result = MULTIPLIER * result + objIndex.normalIndex;

		return result;
	}
};

IndexedModel OBJModel::ToIndexedModel()
{
	IndexedModel result;
    IndexedModel normalModel;
    
	unordered_map<OBJIndex, unsigned short, Hash<OBJIndex>> resultIndexMap;
	unordered_map<unsigned short, unsigned short> normalIndexMap;
	unordered_map<unsigned short, unsigned short> indexMap;

	for (int i = 0; i < OBJIndices.size(); ++i)
	{
		OBJIndex currentIndex = OBJIndices[i];
		Math::Vector3D currentPosition = vertices[currentIndex.vertexIndex];
		Math::Vector2D currentTexCoord(0, 0);
		Math::Vector3D currentNormal(0, 0, 0);
		if (hasUVs)
		{
			currentTexCoord = uvs[currentIndex.uvIndex];
		}
		if (hasNormals)
		{
			currentNormal = normals[currentIndex.normalIndex];
		}

		unsigned short modelVertexIndex;
		unordered_map<OBJIndex, unsigned short>::iterator itr = resultIndexMap.find(currentIndex);
		if (itr == resultIndexMap.end())
		{
			modelVertexIndex = result.PositionsSize();
			resultIndexMap.insert(std::pair<OBJIndex, unsigned short>(currentIndex, modelVertexIndex));
			result.AddPosition(currentPosition);
			result.AddTexCoord(currentTexCoord);
			if (hasNormals)
			{
				result.AddNormal(currentNormal);
			}
		}
		else
		{
			modelVertexIndex = itr->second;
		}
		
		unsigned short normalModelIndex;
		unordered_map<unsigned short, unsigned short>::iterator normalItr = normalIndexMap.find(currentIndex.vertexIndex);
		if (normalItr == normalIndexMap.end())
		{
			normalModelIndex = normalModel.PositionsSize();
			normalIndexMap.insert(std::pair<unsigned short, unsigned short>(currentIndex.vertexIndex, normalModelIndex));
			
			normalModel.AddPosition(currentPosition);
			normalModel.AddTexCoord(currentTexCoord);
			normalModel.AddNormal(currentNormal);
		}
		else
		{
			normalModelIndex = normalItr->second;
		}

		result.AddIndex(modelVertexIndex);
		normalModel.AddIndex(normalModelIndex);
		indexMap.insert(std::pair<unsigned short, unsigned short>(modelVertexIndex, normalModelIndex));
	}

	if (!hasNormals)
	{
		normalModel.CalcNormals();

		for (int i = 0; i < result.PositionsSize(); ++i)
		{
			//result.SetNormal(i, normalModel.GetNormal(indexMap[i]));
			result.AddNormal(normalModel.GetNormal(indexMap[i]));
		}
	}

	return result;
}

/* static */ bool OBJModel::CompareOBJIndexPtr(const OBJIndex* a, const OBJIndex* b)
{
	return (a->vertexIndex < b->vertexIndex);
}

/* static */ inline unsigned int OBJModel::FindNextChar(unsigned int start, const char* str, unsigned int length, char token)
{
    unsigned int result = start;
    while(result < length)
    {
        result++;
        if(str[result] == token)
            break;
    }
    
    return result;
}

/* static */ inline float OBJModel::ParseOBJFloatValue(const std::string& token, unsigned int start, unsigned int end)
{
	return static_cast<float>(atof(token.substr(start, end - start).c_str()));
}

/* static */ Math::Vector2D OBJModel::ParseOBJVec2(const std::string& line)
{
    unsigned int tokenLength = static_cast<unsigned int>(line.length());
    const char* tokenString = line.c_str();
    
    unsigned int vertIndexStart = 3;
    
    while(vertIndexStart < tokenLength)
    {
        if(tokenString[vertIndexStart] != ' ')
            break;
        vertIndexStart++;
    }
    
    unsigned int vertIndexEnd = FindNextChar(vertIndexStart, tokenString, tokenLength, ' ');
    
    float x = ParseOBJFloatValue(line, vertIndexStart, vertIndexEnd);
    
    vertIndexStart = vertIndexEnd + 1;
    vertIndexEnd = FindNextChar(vertIndexStart, tokenString, tokenLength, ' ');
    
    float y = ParseOBJFloatValue(line, vertIndexStart, vertIndexEnd);
    
    return Math::Vector2D(x, y);
}

/* static */ Math::Vector3D OBJModel::ParseOBJVec3(const std::string& line)
{
    unsigned int tokenLength = static_cast<unsigned int>(line.length());
    const char* tokenString = line.c_str();
    
    unsigned int vertIndexStart = 2;
    
    while(vertIndexStart < tokenLength)
    {
        if(tokenString[vertIndexStart] != ' ')
            break;
        vertIndexStart++;
    }
    
    unsigned int vertIndexEnd = FindNextChar(vertIndexStart, tokenString, tokenLength, ' ');
    
    float x = ParseOBJFloatValue(line, vertIndexStart, vertIndexEnd);
    
    vertIndexStart = vertIndexEnd + 1;
    vertIndexEnd = FindNextChar(vertIndexStart, tokenString, tokenLength, ' ');
    
    float y = ParseOBJFloatValue(line, vertIndexStart, vertIndexEnd);
    
    vertIndexStart = vertIndexEnd + 1;
    vertIndexEnd = FindNextChar(vertIndexStart, tokenString, tokenLength, ' ');
    
    float z = ParseOBJFloatValue(line, vertIndexStart, vertIndexEnd);
    
    return Math::Vector3D(x, y, z);

    //glm::vec3(atof(tokens[1].c_str()), atof(tokens[2].c_str()), atof(tokens[3].c_str()))
}

/* static */ inline unsigned int OBJModel::ParseOBJIndexValue(const std::string& token, unsigned int start, unsigned int end)
{
	return static_cast<unsigned int>(atoi(token.substr(start, end - start).c_str()) - 1);
}

/* static */ inline std::vector<std::string> OBJModel::SplitString(const std::string &s, char delim)
{
    std::vector<std::string> elems;
        
    const char* cstr = s.c_str();
    unsigned int strLength = static_cast<unsigned int>(s.length());
    unsigned int start = 0;
    unsigned int end = 0;
        
    while(end <= strLength)
    {
        while(end <= strLength)
        {
            if(cstr[end] == delim)
                break;
            end++;
        }
            
        elems.push_back(s.substr(start, end - start));
        start = end + 1;
        end = start;
    }
        
    return elems;
}

void OBJModel::CreateOBJFace(const std::string& line)
{
	std::vector<std::string> tokens = SplitString(line, ' ');

	for(size_t i = 0; i < tokens.size() - 3; ++i)
	{
		this->OBJIndices.push_back(ParseOBJIndex(tokens[1]));
		this->OBJIndices.push_back(ParseOBJIndex(tokens[2 + i]));
		this->OBJIndices.push_back(ParseOBJIndex(tokens[3 + i]));
	}
}

OBJIndex OBJModel::ParseOBJIndex(const std::string& token)
{
    unsigned int tokenLength = static_cast<unsigned int>(token.length());
    const char* tokenString = token.c_str();
    
    unsigned int vertIndexStart = 0;
    unsigned int vertIndexEnd = FindNextChar(vertIndexStart, tokenString, tokenLength, '/');
    
    OBJIndex result;
    result.vertexIndex = ParseOBJIndexValue(token, vertIndexStart, vertIndexEnd);
    result.uvIndex = 0;
    result.normalIndex = 0;
    
    if(vertIndexEnd >= tokenLength)
        return result;
    
    vertIndexStart = vertIndexEnd + 1;
    vertIndexEnd = FindNextChar(vertIndexStart, tokenString, tokenLength, '/');
    
    result.uvIndex = ParseOBJIndexValue(token, vertIndexStart, vertIndexEnd);
    hasUVs = true;
    
    if(vertIndexEnd >= tokenLength)
        return result;
    
    vertIndexStart = vertIndexEnd + 1;
    vertIndexEnd = FindNextChar(vertIndexStart, tokenString, tokenLength, '/');
    
    result.normalIndex = ParseOBJIndexValue(token, vertIndexStart, vertIndexEnd);
    hasNormals = true;
    
    return result;
}