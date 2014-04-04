#pragma once

#include "Math\Vector.h"

#include "Utility\Log.h"

#include <map>
#include <string>

namespace Rendering
{

class MappedValues
{
/* ==================== Non-static member variables begin ==================== */
private:
	std::map<std::string, Math::Vector3D> vec3DMap;
	std::map<std::string, Math::Real> realMap;
/* ==================== Non-static member variables end ==================== */


/* ==================== Constructors and destructors begin ==================== */
public:
	MappedValues(void) { }
	virtual ~MappedValues(void) { }
/* ==================== Constructors and destructors end ==================== */


/* ==================== Non-static member functions begin ==================== */
public:
	inline void AddVector3D(const std::string& name, const Math::Vector3D& vec)
	{
		ASSERT(vec3DMap.find(name) == vec3DMap.end());
		vec3DMap.insert(std::pair<std::string, Math::Vector3D>(name, vec));
	}

	inline void AddReal(const std::string& name, const Math::Real value)
	{
		ASSERT(realMap.find(name) == realMap.end());
		realMap.insert(std::pair<std::string, Math::Real>(name, value));
	}

	inline Math::Vector3D GetVec3D(const std::string& name) const
	{
		std::map<std::string, Math::Vector3D>::const_iterator itr = vec3DMap.find(name);
		if (itr == vec3DMap.end()) // vector not found
		{
			stdlog(Utility::Warning, LOGPLACE, "Vector with name \"%s\" has not been found", name.c_str());
			return Math::Vector3D(0, 0, 0);
		}
		return itr->second;
	}

	inline Math::Real GetReal(const std::string& name) const
	{
		std::map<std::string, Math::Real>::const_iterator itr = realMap.find(name);
		if (itr == realMap.end()) // number not found
		{
			stdlog(Utility::Warning, LOGPLACE, "Real number with name \"%s\" has not been found", name.c_str());
			return 0;
		}
		return itr->second;
	}
/* ==================== Non-static member functions end ==================== */
}; /* end class MappedValues */

} /* end namespace Rendering */