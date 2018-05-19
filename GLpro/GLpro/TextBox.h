#pragma once

#include "stdafx.h"

#include "Box.h"
#include "src/Resource/TextureManager.h"
#include "src/Shader/ShaderManager.h"
#include "RenderManager.h"
#include "TextFObj.h"
#include "RBox.h"
#include "RText.h"
#include "TextBuffer.h"

namespace RESOURCE { class Texture; class TextureManager; }
namespace RENDER { class RBox; }


enum ENUM_PREMADE_TEXTBOX
{
	PREMADE_TEXTBOX_CHATLOG,
	PREMADE_TEXTBOX_INPUT,
	PREMADE_TEXTBOX_CHATINPUT,
	PREMADE_TEXTBOX_SIZE
};



class TextBox : public Box
{
public:
	TextBox(int line, int len, glm::vec2 pos, int fontSize, const std::string &textTextureFileName, const std::string &type, SHADER::ShaderText* shaderText)
		: Box(len * fontSize, line * fontSize, pos.x, pos.y)	// pos & size calc
	{
		_rText = GRendermanager->getRRender<RENDER::RText, SHADER::ShaderText>(shaderText);
		_textFObj = new RENDER_TARGET::TEXT::TextFObj(textTextureFileName.c_str(), type.c_str(), line, len, pos, fontSize);

		/*	textbox vertex gen process
		glm::vec2 vertex_up_left = glm::vec2(len * _fontSize / 2, -line * _fontSize);
		glm::vec2 vertex_up_right = glm::vec2(len * _fontSize / 2 + _fontSize, -line * _fontSize);
		glm::vec2 vertex_down_right = glm::vec2(len * _fontSize / 2 + _fontSize, -line * _fontSize - _fontSize);
		glm::vec2 vertex_down_left = glm::vec2(len * _fontSize / 2, -line * _fontSize - _fontSize);
		*/

		_rText->addToDrawList(_textFObj, nullptr);
	}

	TextBox(int line, int len, glm::vec2 pos, int fontSize, RESOURCE::Texture * texture, SHADER::ShaderText* shaderText)
		: Box(len * fontSize, line * fontSize, pos.x, pos.y)
	{
		_rText = GRendermanager->getRRender<RENDER::RText, SHADER::ShaderText>(shaderText);
		_textFObj = new RENDER_TARGET::TEXT::TextFObj(texture, line, len, pos, fontSize);
		
		_rText->addToDrawList(_textFObj, nullptr);
	}

	TextBox(const TextBox& copy) : Box(copy._width, copy._height, copy._widthLT, copy._heightLT)
	{
		_rText = copy._rText;
		RESOURCE::TextBuffer* copyTextBuffer = _textFObj->_textBuffer;
		_textFObj = new RENDER_TARGET::TEXT::TextFObj(copy._textFObj->_texture, copyTextBuffer->getLineN(), copyTextBuffer->getLenN(), _textFObj->_pos, copyTextBuffer->getFontSize());
		drawElement = _rText->addToDrawList(_textFObj, nullptr);
	}

	virtual ~TextBox()
	{
		auto shared = drawElement.lock();
		_rText->destructor(shared);		//destroy render Target in Renderer's container
		delete _textFObj;
	}

	virtual void setBRender(bool bRender) override
	{
		_textFObj->setRender(bRender);		// render flag(bool) modify
	};

	virtual void modifyEmptySize(int width, int height)
	{
		// pos 변경은 FObj에서
		printf_s("TextBox modifyEmptySize is not vaild\n");
	}

	virtual void modifyTextBoxSize(int line, int len, int fontSize)
	{
		_textFObj->changeFrame(line, len, fontSize);
	}

	virtual void modifyTextComment(std::string& str)
	{
		_textFObj->changePrintStr(str);
	}

	virtual void moveLTPosition(int widthLT, int heightLT)
	{
		_textFObj->setPos(glm::vec2(_widthLT, _heightLT));
	}

public:
	static void initPreMade()
	{
		SHADER::ShaderText* shaderText = GShaderManager->m_addShader<SHADER::ShaderText>(ENUM_SHADER_TYPE::SHADER_TYPE_TEXT, "data/Shader/TextVertexShader.vertexshader", "TextVertexShader.fragmentshader");
		RESOURCE::Texture* asciiTexture = GTextureManager->getTextureWithFileName("data/Texture/Holstein.DDS", "dds");

		preMadeTextBoxesVec.push_back(new TextBox(6, 80, glm::vec2(300, 300), 16, asciiTexture, shaderText));
		preMadeTextBoxesVec.push_back(new TextBox(1, 20, glm::vec2(300, 300), 20, asciiTexture, shaderText));
		preMadeTextBoxesVec.push_back(new TextBox(1, 80, glm::vec2(300, 300), 16, asciiTexture, shaderText));
	}

public:
	static std::vector<TextBox*> preMadeTextBoxesVec;

	std::weak_ptr<RENDER::RText::RTextDrawElement> drawElement;		// draw elem from renderer ( send to renderer for remove )
	RENDER_TARGET::TEXT::TextFObj * _textFObj;						// fobj
	RENDER::RText* _rText;									// which renderer is used
};