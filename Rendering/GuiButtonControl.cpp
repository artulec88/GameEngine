#include "stdafx.h"
#include "GuiButtonControl.h"
#include "Line.h"
#include "Renderer.h"

#include <sstream>

Rendering::Controls::GuiButtonControl::GuiButtonControl(const std::string& text, const Text::Font* font, math::Real fontSize, const Texture* iconTexture,
	const math::Vector2D& screenPosition, const math::Angle& screenRotationAngle, const math::Vector2D& scale, math::Real maxLineLength, const Color& textColor, const Color& outlineColor, const math::Vector2D& offset,
	bool isCentered /* = false */, math::Real characterWidth /* = 0.5f */, math::Real characterEdgeTransitionWidth /* = 0.1f */, math::Real borderWidth /* = 0.4f */,
	math::Real borderEdgeTransitionWidth /* = 0.1f */) :
	GuiControl(screenPosition, screenRotationAngle, scale),
	m_mesh(nullptr),
	m_text(text),
	m_font(font),
	m_fontSize(fontSize),
	m_iconTexture(iconTexture),
	m_iconTransformationMatrix(screenPosition, screenRotationAngle, math::Vector2D(0.05f, 0.05f)),
	m_color(textColor),
	m_outlineColor(outlineColor),
	m_maxLineLength(maxLineLength),
	m_linesCount(0),
	m_isCentered(isCentered),
	m_offset(offset),
	m_characterWidth(characterWidth),
	m_characterEdgeTransitionWidth(characterEdgeTransitionWidth),
	m_borderWidth(borderWidth),
	m_borderEdgeTransitionWidth(borderEdgeTransitionWidth)
{
	if (m_font != NULL && !text.empty())
	{
		// TODO: Investigate more thoroughly what happens exactly when text == "". Some errors seem to appear when we don't call SetText for an empty string.
		SetText(text);
	}
	else
	{
		// Replace all such calls to create new meshes with reference to one single mesh (probably MeshIDs::SIMPLE_QUAD).
		math::Vector2D quadVertexPositions[] = { math::Vector2D(-REAL_ONE, REAL_ONE), math::Vector2D(REAL_ONE, REAL_ONE), math::Vector2D(-REAL_ONE, -REAL_ONE), math::Vector2D(REAL_ONE, -REAL_ONE) };
		math::Vector2D quadTexCoords[] = { math::Vector2D(REAL_ZERO, REAL_ZERO), math::Vector2D(REAL_ZERO, REAL_ONE), math::Vector2D(REAL_ONE, REAL_ZERO), math::Vector2D(REAL_ONE, REAL_ONE) };
		m_mesh = std::make_unique<Mesh>(quadVertexPositions, quadTexCoords, 4, GL_TRIANGLE_STRIP);
	}
}


Rendering::Controls::GuiButtonControl::~GuiButtonControl()
{
	//SAFE_DELETE(m_colorEffect);
	SAFE_DELETE(m_iconTexture);
}

void Rendering::Controls::GuiButtonControl::SetText(const std::string& text)
{
	CHECK_CONDITION_RETURN_VOID_RENDERING(!text.empty(), Utility::Logging::DEBUG, "Cannot create a mesh for the GUI button control. No text is set.");
	//CHECK_CONDITION_RETURN_VOID_ALWAYS_RENDERING(m_font != NULL, Utility::Logging::DEBUG,
	//	"Cannot set text (\"", text, "\") for the GUI control. No font is set. GUI control uses only icons for rendering.");
	std::vector<Text::Line> lines;
	Text::Line currentLine(m_font->GetSpaceWidth(), m_fontSize, m_maxLineLength);
	Text::Word currentWord(m_fontSize);
	bool added;
	for (std::string::const_iterator textItr = text.begin(); textItr != text.end(); ++textItr)
	{
		const int asciiCode = static_cast<int>((*textItr));
		if (asciiCode == Text::Font::SPACE_ASCII_CODE)
		{
			added = currentLine.AttemptToAddWord(currentWord);
			if (!added)
			{
				lines.push_back(currentLine);
				currentLine.Clear();
				currentLine.AttemptToAddWord(currentWord);
			}
			currentWord.Clear();
			continue;
		}
		currentWord.AddCharacter(m_font->GetCharacter(asciiCode));
	}
	// Completing the lines structure
	added = currentLine.AttemptToAddWord(currentWord);
	if (!added)
	{
		lines.push_back(currentLine);
		currentLine.Clear();
		currentLine.AttemptToAddWord(currentWord);
	}
	lines.push_back(currentLine);

	// Creating quad vertices based on the lines vector
	m_linesCount = static_cast<int>(lines.size());
	math::Real minCursorX = REAL_MAX;
	math::Real maxCursorX = REAL_MIN;
	math::Real cursorY = REAL_ZERO;
	std::vector<math::Vector2D> positions;
	std::vector<math::Vector2D> textureCoordinates;
	for (std::vector<Text::Line>::const_iterator lineItr = lines.begin(); lineItr != lines.end(); ++lineItr)
	{
		math::Real cursorX = (m_isCentered) ? (m_maxLineLength - lineItr->GetLineLength()) / 2.0f : REAL_ZERO;
		if (minCursorX > cursorX)
		{
			minCursorX = cursorX;
		}
		for (std::vector<Text::Word>::const_iterator wordItr = lineItr->GetWords().begin(); wordItr != lineItr->GetWords().end(); ++wordItr)
		{
			for (std::vector<Text::Character>::const_iterator characterItr = wordItr->GetCharacters().begin(); characterItr != wordItr->GetCharacters().end(); ++characterItr)
			{
				// Adding positions
				const math::Real x = cursorX + (characterItr->GetOffset().x * m_fontSize);
				const math::Real y = cursorY + (characterItr->GetOffset().y * m_fontSize);
				const math::Real maxX = x + (characterItr->GetSize().x * m_fontSize);
				const math::Real maxY = y + (characterItr->GetSize().y * m_fontSize);
				const math::Real properX = (2.0f * x) - 1.0f;
				const math::Real properY = (-2.0f * y) + 1.0f;
				const math::Real properMaxX = (2.0f * maxX) - 1.0f;
				const math::Real properMaxY = (-2.0f * maxY) + 1.0f;

				//CRITICAL_LOG_RENDERING(tempCount++, ")\n\t(x, y) = (", x, ", ", y, ");\n\t(maxX, maxY) = (", maxX, ", ", maxY,
				//	");\n\t(properX, properY) = (", properX, ", ", properY, ");\n\t(properMaxX, properMaxY) = (", properMaxX, ", ", properMaxY, ");");

				positions.emplace_back(properX, properY); textureCoordinates.push_back(characterItr->GetTextureCoords()); // 0
				positions.emplace_back(properMaxX, properY); textureCoordinates.emplace_back(characterItr->GetMaxTextureCoords().x, characterItr->GetTextureCoords().y); // 4
				positions.emplace_back(properMaxX, properMaxY); textureCoordinates.push_back(characterItr->GetMaxTextureCoords()); // 2
				positions.emplace_back(properMaxX, properMaxY); textureCoordinates.push_back(characterItr->GetMaxTextureCoords()); // 2
				positions.emplace_back(properX, properMaxY); textureCoordinates.emplace_back(characterItr->GetTextureCoords().x, characterItr->GetMaxTextureCoords().y); // 1
				positions.emplace_back(properX, properY); textureCoordinates.push_back(characterItr->GetTextureCoords()); // 0

				cursorX += characterItr->GetXAdvance() * m_fontSize;
			}
			cursorX += m_font->GetSpaceWidth() * m_fontSize;
		}
		if (maxCursorX < cursorX)
		{
			maxCursorX = cursorX;
		}
		cursorY += Text::Font::LINE_HEIGHT * m_fontSize;
	}
	MoveAABR(minCursorX, cursorY, maxCursorX, REAL_ZERO);
	if (m_mesh == nullptr)
	{
		m_mesh = std::make_unique<Mesh>(&positions[0], &textureCoordinates[0], positions.size(), GL_TRIANGLES);
	}
	else
	{
		m_mesh->ReplaceData(MeshBufferTypes::POSITIONS, positions.data(), positions.size());
		m_mesh->ReplaceData(MeshBufferTypes::TEXTURE_COORDINATES, textureCoordinates.data(), textureCoordinates.size());
	}
}

void Rendering::Controls::GuiButtonControl::Draw(const Shader* guiControlShader, const Renderer& renderer) const
{
	if (m_font != NULL)
	{
		m_font->Bind(GetScreenPosition(), m_offset, m_color, m_outlineColor, m_characterWidth, m_characterEdgeTransitionWidth, m_borderWidth, m_borderEdgeTransitionWidth);
		m_mesh->Draw();
	}
	else if (m_iconTexture != NULL)
	{
		DELOCUST_LOG_RENDERING("Rendering button with icon");
		//renderer.Render(*m_mesh, NULL, math::Transform(), guiControlShader);
		guiControlShader->Bind();
		m_iconTexture->Bind(0);
		guiControlShader->SetUniformMatrix("guiTransformationMatrix", m_iconTransformationMatrix);
		guiControlShader->SetUniformi("guiTexture", 0);
		m_mesh->Draw();
	}
}

void Rendering::Controls::GuiButtonControl::ApplyColorEffect(const math::effects::Effect<Color>& effect)
{
	effect.Apply(&m_color);
}

void Rendering::Controls::GuiButtonControl::ApplyOutlineColorEffect(const math::effects::Effect<Color>& effect)
{
	effect.Apply(&m_outlineColor);
}

void Rendering::Controls::GuiButtonControl::ApplyOffsetEffect(const math::effects::Effect<math::Vector2D>& effect)
{
	effect.Apply(&m_offset);
}

void Rendering::Controls::GuiButtonControl::ApplyCharacterWidthEffect(const math::effects::Effect<math::Real>& effect)
{
	effect.Apply(&m_characterWidth);
}

void Rendering::Controls::GuiButtonControl::ApplyCharacterEdgeTransitionWidthEffect(const math::effects::Effect<math::Real>& effect)
{
	effect.Apply(&m_characterEdgeTransitionWidth);
}

void Rendering::Controls::GuiButtonControl::ApplyBorderWidthEffect(const math::effects::Effect<math::Real>& effect)
{
	effect.Apply(&m_borderWidth);
}

void Rendering::Controls::GuiButtonControl::ApplyBorderEdgeTransitionWidthEffect(const math::effects::Effect<math::Real>& effect)
{
	effect.Apply(&m_borderEdgeTransitionWidth);
}