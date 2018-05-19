#include "stdafx.h"
#include "TextFObj.h"

#include "src/Resource/Texture.h"
#include "src/Resource/TextureManager.h"

#include "../../TextBuffer.h"

RENDER_TARGET::TEXT::TextFObj::TextFObj(const char * textureFileName, const char * textureType, int lineN, int lengthN, glm::vec2 pos, int fontSize)
{
	_bBox = false;
	_pos = pos;
	_texture = GTextureManager->getTextureWithFileName(textureFileName, textureType);
	_textBuffer = new RESOURCE::TextBuffer(lineN, lengthN, fontSize);
	
	if (_texture == nullptr || _textBuffer == nullptr)
	{
		_texture = nullptr;
		_textBuffer = nullptr;
		printf_s("[LOG] : _texture, _textBuffer nullptr\n");
	}
}
RENDER_TARGET::TEXT::TextFObj::TextFObj(RESOURCE::Texture * texture, int lineN, int lengthN, glm::vec2 pos, int fontSize)
{
	_bBox = false;
	_pos = pos;
	_texture = texture;
	_textBuffer = new RESOURCE::TextBuffer(lineN, lengthN, fontSize);

	if (_texture == nullptr || _textBuffer == nullptr)
	{
		_texture = nullptr;
		_textBuffer = nullptr;
		printf_s("[LOG] : _texture, _textBuffer nullptr\n");
	}
}
RENDER_TARGET::TEXT::TextFObj::TextFObj(const char * textureFileName, const char * textureType, int width, int height, int widthLT, int heightLT)
{
	_bBox = true;		// ´ÜÀÏ box
	_pos.x = widthLT;
	_pos.y = heightLT;

	_texture = GTextureManager->getTextureWithFileName(textureFileName, textureType);
	_textBuffer = new RESOURCE::TextBuffer(width, height);

	if (_texture == nullptr || _textBuffer == nullptr)
	{
		_texture = nullptr;
		_textBuffer = nullptr;
		printf_s("[LOG] : _texture, _textBuffer nullptr .. empty box\n");
	}
}

RENDER_TARGET::TEXT::TextFObj::TextFObj(RESOURCE::Texture * texture, int width, int height, int widthLT, int heightLT)
{
	_bBox = true;
	_pos.x = widthLT;
	_pos.y = heightLT;

	_texture = texture;
	_textBuffer = new RESOURCE::TextBuffer(width, height);

	if (_texture == nullptr || _textBuffer == nullptr)
	{
		_texture = nullptr;
		_textBuffer = nullptr;
		printf_s("[LOG] : _texture, _textBuffer nullptr .. empty box\n");
	}
}

void RENDER_TARGET::TEXT::TextFObj::changePrintStr(std::string & inStr)
{
	printStr = inStr;
	_textBuffer->setUVBufferWithString(printStr);
}

void RENDER_TARGET::TEXT::TextFObj::changeFrame(int lineN, int lengthN, int fontSize)
{
	_textBuffer->updateTextVBO(lineN, lengthN, fontSize);
}

void RENDER_TARGET::TEXT::TextFObj::changeBoxSize(int height, int width)
{
	_textBuffer->updateBoxVBO(height, width);
}

// common

inline void RENDER_TARGET::TEXT::TextFObj::setRender(bool bRender) {
	_bRender = bRender;
}


void RENDER_TARGET::TEXT::TextFObj::setTemporary(float lifeTime)
{
	_bTemporary = true;
	_lifeTime = lifeTime;
}

void RENDER_TARGET::TEXT::TextFObj::setBindToScreen(glm::vec2 & pos, glm::vec2 & deltaPos)
{
	_bBindToScreen = true;
	_pos = pos;
	_deltaPos = deltaPos;
}

void RENDER_TARGET::TEXT::TextFObj::setPos(glm::vec2 & pos)
{
	_pos = pos;
}

void RENDER_TARGET::TEXT::TextFObj::setDeltaPos(glm::vec2 & deltaPos)
{
	_deltaPos = deltaPos;
}
