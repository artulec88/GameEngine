#ifndef __RENDERING_CUBE_SHADOW_MAP_H__
#define __RENDERING_CUBE_SHADOW_MAP_H__

#include "Rendering.h"

namespace rendering
{

	class CubeShadowMap
	{
		/* ==================== Constructors and destructors begin ==================== */
	public:
		CubeShadowMap();
		~CubeShadowMap();
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
		void Init(unsigned int windowWidth, unsigned int windowHeight);

		void BindForWriting(GLenum cubeFace) const;

		void BindForReading(unsigned int textureUnit) const;
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
	private:
		GLuint m_fbo;
		GLuint m_renderBuffer;
		GLuint m_shadowMap;
		GLuint m_depth;
		/* ==================== Non-static member variables end ==================== */
	}; /* end class CubeShadowMap */

} /* end namespace rendering */

#endif /* __RENDERING_CUBE_SHADOW_MAP_H__ */
