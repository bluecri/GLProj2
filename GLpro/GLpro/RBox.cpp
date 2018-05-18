#include "stdafx.h"
#include "RBox.h"
#include "src/Shader/ShaderText.h"
#include "TextFObj.h"
#include "TextBuffer.h"
#include "src/Resource/Texture.h"

RENDER::RBox::RBox(SHADER::ShaderText * shaderObj)
{
	_shaderObj = shaderObj;
}

RBoxDrawElem RENDER::RBox::addToDrawList(RBoxDrawElem normalFObj)
{
	auto elem = normalFObj;
	_boxDrawElemContainer.push_back(elem);
	return elem;
}

void RENDER::RBox::update(CAMERA::Camera * cam)
{
	// none camera
}

void RENDER::RBox::draw(float deltaTime)
{
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	_shaderObj->bind();
	for (auto it = _boxDrawElemContainer.begin(); it != _boxDrawElemContainer.end(); )
	{
		
		RBoxDrawElem textFObj = (*it);

		if (!textFObj->getRender())
		{
			continue;
		}

		textFObj->_textBuffer->bind();
		glActiveTexture(GL_TEXTURE4);
		textFObj->_texture->bind();
		_shaderObj->loadInt(_shaderObj->m_text2DUniformID, 4);
		
		// 원래 용도는 움직이는 text. => position으로 사용
		_shaderObj->loadVector2(_shaderObj->m_movedVec2ID, textFObj->_pos);

		textFObj->_textBuffer->render();

		textFObj->_texture->unbind();
		textFObj->_textBuffer->unbind();

		it++;
	}

	_shaderObj->unbind();

	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
}

void RENDER::RBox::chageShader(SHADER::ShaderText * other)
{
	_shaderObj = other;
}

SHADER::ShaderText * RENDER::RBox::getShader() const
{
	return _shaderObj;
}

void RENDER::RBox::destructor(RBoxDrawElem delElem)
{
	RBoxDrawElem ptr = delElem;
	for (auto it = _boxDrawElemContainer.begin(); it != _boxDrawElemContainer.end();)
	{
		if ((*it) == ptr)
		{
			_boxDrawElemContainer.erase(it);
			return;
		}
		++it;
	}
}
