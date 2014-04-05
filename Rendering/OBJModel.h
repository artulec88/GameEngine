#pragma once

#include "IndexedModel.h"
#include "Math\Vector.h"
#include <vector>
#include <string>

namespace Rendering
{

class OBJIndex
{
public:
    unsigned int vertexIndex;
    unsigned int uvIndex;
    unsigned int normalIndex;
    
	bool operator==(const OBJIndex& r) const { return vertexIndex == r.vertexIndex; }
    bool operator<(const OBJIndex& r) const { return vertexIndex < r.vertexIndex; }
}; /* end class OBJIndex */

// TODO: Create a base class for all models (OBJModel class should inherit from this base class)
class OBJModel
{
/* ==================== Constructors and destructors begin ==================== */
public:
    OBJModel(const std::string& fileName);
	~OBJModel();
/* ==================== Constructors and destructors end ==================== */

/* ==================== Static functions begin ==================== */
private:
	static bool CompareOBJIndexPtr(const OBJIndex* a, const OBJIndex* b);
	static unsigned int FindNextChar(unsigned int start, const char* str, unsigned int length, char token);
	static unsigned int ParseOBJIndexValue(const std::string& token, unsigned int start, unsigned int end);
	static float ParseOBJFloatValue(const std::string& token, unsigned int start, unsigned int end);
	static std::vector<std::string> SplitString(const std::string &s, char delim);

	static Math::Vector2D ParseOBJVec2(const std::string& line);
	static Math::Vector3D ParseOBJVec3(const std::string& line);
/* ==================== Static functions end ==================== */
    
/* ==================== Non-static member functions begin ==================== */
public:
    IndexedModel ToIndexedModel();
private:
    unsigned int FindLastVertexIndex(const std::vector<OBJIndex*>& indexLookup, const OBJIndex* currentIndex, const IndexedModel& result) const;
    void CreateOBJFace(const std::string& line);
    OBJIndex ParseOBJIndex(const std::string& token);

    //Vector2f ParseOBJVec2(const std::string& line);
    //Vector3f ParseOBJVec3(const std::string& line);
/* ==================== Non-static member functions begin ==================== */


/* ==================== Non-static member variables begin ==================== */
public:
    std::vector<OBJIndex> OBJIndices;
    std::vector<Math::Vector3D> vertices;
    std::vector<Math::Vector2D> uvs;
    std::vector<Math::Vector3D> normals;
    bool hasUVs;
    bool hasNormals;
/* ==================== Non-static member variables end ==================== */
}; /* end class OBJModel */

} /* end namespace Rendering */