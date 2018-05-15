#include "stdafx.h"
#include "TextFObj.h"

#include "src/Resource/Texture.h"
#include "src/Resource/TextureManager.h"

#include "../../TextBuffer.h"

RENDER_TARGET::TEXT::TextFObj::TextFObj(const char * textureFileName, const char * textureType, int lineN, int lengthN, int fontSize)
{
	_texture = GTextureManager->getTextureWithFileName(textureFileName, textureType);
	_textBuffer = new RESOURCE::TextBuffer(lineN, lengthN, fontSize);
	
}
void RENDER_TARGET::TEXT::TextFObj::changePrintStr(std::string & inStr)
{
	printStr = inStr;
	_textBuffer->setUVBufferWithString(printStr);
}

void RENDER_TARGET::TEXT::TextFObj::changeFrame(int lineN, int lengthN, int fontSize)
{
	_textBuffer->setPosBuffer(lineN, lengthN, fontSize);
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
