#include "stdafx.h"
#include "GuiButtonControl.h"
#include "Line.h"
#include "Renderer.h"

#include <sstream>

Rendering::Controls::GuiButtonControl::GuiButtonControl(const std::string& text, const Text::Font* font, Math::Real fontSize, const Texture* iconTexture, const Math::Vector2D& screenPosition,
	Math::Real maxLineLength, const Math::Vector3D& textColor, const Math::Vector3D& outlineColor, const Math::Vector2D& offset, bool isCentered /* = false */, Math::Real characterWidth /* = 0.5f */,
	Math::Real characterEdgeTransitionWidth /* = 0.1f */, Math::Real borderWidth /* = 0.4f */, Math::Real borderEdgeTransitionWidth /* = 0.1f */) :
	GuiControl(screenPosition),
	m_text(text),
	m_font(font),
	m_fontSize(fontSize),
	m_iconTexture(iconTexture),
	m_iconTransformationMatrix(screenPosition, Math::Vector2D(0.05f, 0.05f)),
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
	if (m_font != NULL)
	{
		// TODO: Investigate more thoroughly what happens exactly when text == "". Some errors seem to appear when we don't call SetText for an empty string.
		SetText(text);
	}
	else
	{
		Math::Vector2D quadVertexPositions[] = { Math::Vector2D(-REAL_ONE, REAL_ONE), Math::Vector2D(REAL_ONE, REAL_ONE), Math::Vector2D(-REAL_ONE, -REAL_ONE), Math::Vector2D(REAL_ONE, -REAL_ONE) };
		m_mesh = std::make_unique<GuiMesh>(quadVertexPositions, 4);
	}
}


Rendering::Controls::GuiButtonControl::~GuiButtonControl()
{
	//SAFE_DELETE(m_colorEffect);
	SAFE_DELETE(m_iconTexture);
}

void Rendering::Controls::GuiButtonControl::SetText(const std::string& text)
{
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
	Math::Real minCursorX = REAL_MAX;
	Math::Real maxCursorX = REAL_MIN;
	Math::Real cursorY = REAL_ZERO;
	std::vector<Math::Vector2D> positions;
	std::vector<Math::Vector2D> textureCoordinates;
	for (std::vector<Text::Line>::const_iterator lineItr = lines.begin(); lineItr != lines.end(); ++lineItr)
	{
		Math::Real cursorX = (m_isCentered) ? (m_maxLineLength - lineItr->GetLineLength()) / 2.0f : REAL_ZERO;
		if (minCursorX > cursorX)
		{
			minCursorX = cursorX;
		}
		for (std::vector<Text::Word>::const_iterator wordItr = lineItr->GetWords().begin(); wordItr != lineItr->GetWords().end(); ++wordItr)
		{
			for (std::vector<Text::Character>::const_iterator characterItr = wordItr->GetCharacters().begin(); characterItr != wordItr->GetCharacters().end(); ++characterItr)
			{
				// Adding positions
				const Math::Real x = cursorX + (characterItr->GetOffset().GetX() * m_fontSize);
				const Math::Real y = cursorY + (characterItr->GetOffset().GetY() * m_fontSize);
				const Math::Real maxX = x + (characterItr->GetSize().GetX() * m_fontSize);
				const Math::Real maxY = y + (characterItr->GetSize().GetY() * m_fontSize);
				const Math::Real properX = (2.0f * x) - 1.0f;
				const Math::Real properY = (-2.0f * y) + 1.0f;
				const Math::Real properMaxX = (2.0f * maxX) - 1.0f;
				const Math::Real properMaxY = (-2.0f * maxY) + 1.0f;

				//CRITICAL_LOG_RENDERING(tempCount++, ")\n\t(x, y) = (", x, ", ", y, ");\n\t(maxX, maxY) = (", maxX, ", ", maxY,
				//	");\n\t(properX, properY) = (", properX, ", ", properY, ");\n\t(properMaxX, properMaxY) = (", properMaxX, ", ", properMaxY, ");");

				positions.emplace_back(properX, properY); textureCoordinates.push_back(characterItr->GetTextureCoords()); // 0
				positions.emplace_back(properMaxX, properY); textureCoordinates.emplace_back(characterItr->GetMaxTextureCoords().GetX(), characterItr->GetTextureCoords().GetY()); // 4
				positions.emplace_back(properMaxX, properMaxY); textureCoordinates.push_back(characterItr->GetMaxTextureCoords()); // 2
				positions.emplace_back(properMaxX, properMaxY); textureCoordinates.push_back(characterItr->GetMaxTextureCoords()); // 2
				positions.emplace_back(properX, properMaxY); textureCoordinates.emplace_back(characterItr->GetTextureCoords().GetX(), characterItr->GetMaxTextureCoords().GetY()); // 1
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
		m_mesh = std::make_unique<TextMesh>(&positions[0], &textureCoordinates[0], positions.size());
	}
	else
	{
		(static_cast<TextMesh*>(m_mesh.get()))->ReplaceData(&positions[0], &textureCoordinates[0], static_cast<int>(positions.size()));
	}
}

void Rendering::Controls::GuiButtonControl::Draw(const Rendering::Renderer& renderer) const
{
	if (m_font != NULL)
	{
		const Shader& shader = renderer.GetGuiTextShader();
		shader.Bind();
		shader.SetUniformVector2D("translation", GetScreenPosition());
		shader.SetUniformVector2D("offset", m_offset);
		shader.SetUniformVector3D("textColor", m_color);
		shader.SetUniformVector3D("outlineColor", m_outlineColor);
		shader.SetUniformf("characterWidth", m_characterWidth);
		shader.SetUniformf("characterEdgeTransitionDistance", m_characterEdgeTransitionWidth);
		shader.SetUniformf("borderWidth", m_borderWidth);
		shader.SetUniformf("borderEdgeTransitionDistance", m_borderEdgeTransitionWidth);
		shader.SetUniformi("fontAtlas", 0);

		m_font->Bind();
		m_mesh->Draw();
	}
	else if (m_iconTexture != NULL)
	{
		DELOCUST_LOG_RENDERING("Rendering button with icon");
		const Shader& shader = renderer.GetGuiShader();
		shader.Bind();
		m_iconTexture->Bind(0);
		shader.SetUniformMatrix("guiTransformationMatrix", m_iconTransformationMatrix);
		shader.SetUniformi("guiTexture", 0);
		m_mesh->Draw();
	}
}