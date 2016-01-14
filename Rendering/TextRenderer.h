#ifndef __RENDERING_TEXT_RENDERER_H__
#define __RENDERING_TEXT_RENDERER_H__

#include "GameComponent.h"
#include "Mesh.h"
#include "Shader.h"
#include "Math\Matrix.h"
#include "Math\Vector.h"
#include <string>

namespace Rendering
{

class Material;
class Renderer;
class Transform;

namespace Text
{
	enum Alignment
	{
		LEFT = 0,
		RIGHT,
		CENTER,
		//JUSTIFY // TODO: Implement JUSTIFY alignment
	}; /* end enum Alignment */
} /* end namespace Text */

class TextRenderer
{
/* ==================== Constructors and destructors begin ==================== */
public:
	TextRenderer(Texture* fontTexture, Math::Real defaultFontSize = 32.0f);
	virtual ~TextRenderer(void);
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static member functions begin ==================== */
public:
	void DrawString(Text::Alignment alignment, int y, const std::string& str, Renderer* renderer) const;
	void DrawString(Text::Alignment alignment, int y, const std::string& str, Renderer* renderer, Math::Real fontSize) const;
	void DrawString(Text::Alignment alignment, int y, const std::string& str, Renderer* renderer, const Math::Vector3D& fontColor) const;
	void DrawString(Text::Alignment alignment, int y, const std::string& str, Renderer* renderer, Math::Real fontSize, const Math::Vector3D& fontColor) const;
	void DrawString(int x, int y, const std::string& str, Renderer* renderer) const;
	void DrawString(int x, int y, const std::string& str, Renderer* renderer, Math::Real fontSize) const;
	void DrawString(int x, int y, const std::string& str, Renderer* renderer, const Math::Vector3D& fontColor) const;
	void DrawString(int x, int y, const std::string& str, Renderer* renderer, Math::Real fontSize, const Math::Vector3D& fontColor) const;

	//void SetFontSize(Math::Real fontSize);
/* ==================== Non-static member functions end ==================== */

/* ==================== Non-static member variables begin ==================== */
private:
	Material* m_fontMaterial;
	Math::Real m_defaultFontSize;
	Math::Vector3D m_defaultFontColor;
	Shader m_textShader;

	Math::Real m_windowWidth;
	Math::Real m_windowHeight;

	Transform m_transform;
	Math::Matrix4D m_projection;

	GLuint m_vaoID;
	GLuint m_texCoordBuffer;
	GLuint m_vertexBuffer;
/* ==================== Non-static member variables end ==================== */
}; /* end class TextRenderer */

} /* end namespace Rendering */

#endif /* __RENDERING_TEXT_RENDERER_H__ */