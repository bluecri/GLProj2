#include "stdafx.h"
#include "RText.h"
#include "src\Shader\ShaderText.h"
#include "TextFObj.h"
#include "RigidbodyComponent.h"
#include "src/Resource/Texture.h"
#include "TextBuffer.h"

RENDER::RText::RText(SHADER::ShaderText * shaderObj)
{
	_shaderObj = shaderObj;
}

// weak_ptr로 변환하여 사용.
std::shared_ptr<RENDER::RText::DrawElement> RENDER::RText::addToDrawList(RENDER_TARGET::TEXT::TextFObj * _textFObj, RigidbodyComponent * rigidbodyComponent)
{
	auto elem = std::make_shared<DrawElement>(_textFObj, rigidbodyComponent);
	_textDrawElemContainer.push_back(elem);
	return elem;
}

void RENDER::RText::update(CAMERA::Camera * cam)
{
	_targetCamera = cam;
}

void RENDER::RText::draw(float deltaTime)
{
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	_shaderObj->bind();
	for (auto it = _textDrawElemContainer.begin(); it != _textDrawElemContainer.end();		)
	{
		RENDER_TARGET::TEXT::TextFObj* _textFObj = (*it)->first;
		RigidbodyComponent* rigitComponent = (*it)->second;

		// check text lifetime
		if (_textFObj->_bTemporary)
		{
			if (_textFObj->_lifeTime < 0.0f)
			{
				// todo : remove this
				it = _textDrawElemContainer.erase(it);
				continue;
			}
			_textFObj->_lifeTime -= deltaTime;
		}

		_textFObj->_textBuffer->bind();

		glActiveTexture(GL_TEXTURE4);
		_textFObj->_texture->bind();
		_shaderObj->loadInt(_shaderObj->m_text2DUniformID, 4);

		_textFObj->_pos += (_textFObj->_deltaPos) * deltaTime;
		_shaderObj->loadVector2(_shaderObj->m_movedVec2ID, _textFObj->_pos);

		// text bind to object OR to screen
		if (_textFObj->_bBindToScreen)
		{
			// textFObj->_pos += (textFObj->_deltaPos) * deltaTime;
			// _shaderObj->loadVector2(_shaderObj->m_movedVec2ID, textFObj->_pos);
		}
		else
		{
			// use textFObj && rigitComponent & camera
			// check rigidComponent != nullptr
		}

		_textFObj->_textBuffer->render();

		_textFObj->_texture->unbind();
		_textFObj->_textBuffer->unbind();

		it++;
	}

	_shaderObj->unbind();

	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);

	return;
}

void RENDER::RText::chageShader(SHADER::ShaderText * other)
{
	_shaderObj = other;
}

SHADER::ShaderText * RENDER::RText::getShader() const
{
	return _shaderObj;
}

void RENDER::RText::destructor(std::shared_ptr<DrawElement> shared)
{
	DrawElement* ptr = shared.get();
	for (auto it = _textDrawElemContainer.begin(); it != _textDrawElemContainer.end();)
	{
		if ((*it).get() == ptr)
		{
			_textDrawElemContainer.erase(it);
			return;
		}
		++it;
	}
}
