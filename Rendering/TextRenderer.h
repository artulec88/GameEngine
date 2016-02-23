#ifndef __RENDERING_TEXT_RENDERER_H__
#define __RENDERING_TEXT_RENDERER_H__

#include "Mesh.h"
#include "Shader.h"
#include "Renderer.h"
#include "Math\Matrix.h"
#include "Math\Vector.h"
#include "Math\Transform.h"
#include <string>

namespace Rendering
{

class Material;
class Renderer;

class TextRenderer
{
/* ==================== Constructors and destructors begin ==================== */
public:
	RENDERING_API TextRenderer(Renderer* renderer, Texture* fontTexture, Math::Real defaultFontSize = 32.0f);
	RENDERING_API virtual ~TextRenderer(void);
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static member functions begin ==================== */
public:
	RENDERING_API void RenderString(Text::Alignment alignment, int y, const std::string& str, Renderer* renderer) const;
	RENDERING_API void RenderString(Text::Alignment alignment, int y, const std::string& str, Renderer* renderer, Math::Real fontSize) const;
	RENDERING_API void RenderString(Text::Alignment alignment, int y, const std::string& str, Renderer* renderer, const Math::Vector3D& fontColor) const;
	RENDERING_API void RenderString(Text::Alignment alignment, int y, const std::string& str, Renderer* renderer, Math::Real fontSize, const Math::Vector3D& fontColor) const;
	RENDERING_API void RenderString(int x, int y, const std::string& str, Renderer* renderer) const;
	RENDERING_API void RenderString(int x, int y, const std::string& str, Renderer* renderer, Math::Real fontSize) const;
	RENDERING_API void RenderString(int x, int y, const std::string& str, Renderer* renderer, const Math::Vector3D& fontColor) const;
	RENDERING_API void RenderString(int x, int y, const std::string& str, Renderer* renderer, Math::Real fontSize, const Math::Vector3D& fontColor) const;

	//void SetFontSize(Math::Real fontSize);
/* ==================== Non-static member functions end ==================== */

/* ==================== Non-static member variables begin ==================== */
private:

	Math::Transform m_transform;
	Math::Matrix4D m_projection;

	GLuint m_vaoID;
	GLuint m_textTextureCoordBuffer;
	GLuint m_textVertexBuffer;
/* ==================== Non-static member variables end ==================== */
}; /* end class TextRenderer */

} /* end namespace Rendering */

#endif /* __RENDERING_TEXT_RENDERER_H__ */