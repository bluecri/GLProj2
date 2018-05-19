#pragma once

#include "Box.h"
#include "src/Resource/TextureManager.h"
#include "RenderManager.h"
#include "src/Shader/ShaderManager.h"
#include "TextFObj.h"
#include "RBox.h"

namespace RESOURCE { class Texture; class TextureManager; }
namespace RENDER { class RBox;}

enum ENUM_PREMADE_IMAGEBOX
{
	PREMADE_IMAGEBOX_BLOCK,
	PREMADE_IMAGEBOX_BLUEGREY,
	PREMADE_IMAGEBOX_IRON,
	PREMADE_IMAGEBOX_PAPER,
	PREMADE_IMAGEBOX_WATER
};

class ImageBox : public Box
{
public:
	ImageBox(int width, int height, int widthLT, int heightLT, const std::string &textureFileName, const std::string &type, SHADER::ShaderText* shaderText) : Box(width, height, widthLT, heightLT)
	{
		_rBox = GRendermanager->getRRender<RENDER::RBox, SHADER::ShaderText>(shaderText);

		_textFObj = new RENDER_TARGET::TEXT::TextFObj(textureFileName.c_str(), type.c_str(), width, height, widthLT, heightLT);
		_rBox->addToDrawList(_textFObj);
	}

	ImageBox(int width, int height, int widthLT, int heightLT, RESOURCE::Texture* texture, SHADER::ShaderText* shaderText);

	// UIEntity 부모자식관계 및 event 함수 바인딩 제외 Copy
	ImageBox(const ImageBox& copy) : Box(copy._width, copy._height, copy._widthLT, copy._heightLT)
	{
		_rBox = copy._rBox;
		_textFObj = new RENDER_TARGET::TEXT::TextFObj(copy._textFObj->_texture, _width, _height, _widthLT, _heightLT);
		_rBox->addToDrawList(_textFObj);
	}

	virtual ~ImageBox()
	{
		_rBox->destructor(_textFObj);		//destroy render Target in Renderer's container
		delete _textFObj;
	}

	virtual void setBRender(bool bRender) override
	{
		_textFObj->setRender(bRender);		// render flag(bool) modify
	};

	virtual void modifyEmptySize(int width, int height)
	{
		printf_s("ImageBox modifyEmptySize is not vaild. Use modifyImageBoxSize instead.\n");
	}

	virtual void modifyImageBoxSize(int width, int height)
	{
		_textFObj->changeBoxSize(height, width);
	}

	virtual void moveLTPosition(int widthLT, int heightLT)
	{
		_textFObj->setPos(glm::vec2(_widthLT, _heightLT);
	}

public:
	static void initPreMade()
	{
		SHADER::ShaderText* shaderText = GShaderManager->m_addShader<SHADER::ShaderText>(ENUM_SHADER_TYPE::SHADER_TYPE_TEXT, "data/Shader/TextVertexShader.vertexshader", "TextVertexShader.fragmentshader");
		RESOURCE::Texture* blockTexture = GTextureManager->getTextureWithFileName("data/Texture/block.dds", "dds");
		RESOURCE::Texture* bluegreyTexture = GTextureManager->getTextureWithFileName("data/Texture/block.dds", "dds");
		RESOURCE::Texture* ironTexture = GTextureManager->getTextureWithFileName("data/Texture/block.dds", "dds");
		RESOURCE::Texture* paperTexture = GTextureManager->getTextureWithFileName("data/Texture/block.dds", "dds");
		RESOURCE::Texture* waterTexture = GTextureManager->getTextureWithFileName("data/Texture/block.dds", "dds");

		preMadeImageBoxesVec.push_back(new ImageBox(100, 40, 40, 40, blockTexture, shaderText));
		preMadeImageBoxesVec.push_back(new ImageBox(100, 40, 40, 40, bluegreyTexture, shaderText));
		preMadeImageBoxesVec.push_back(new ImageBox(100, 40, 40, 40, ironTexture, shaderText));
		preMadeImageBoxesVec.push_back(new ImageBox(100, 40, 40, 40, paperTexture, shaderText));
		preMadeImageBoxesVec.push_back(new ImageBox(100, 40, 40, 40, waterTexture, shaderText));
	}

public:
	static std::vector<ImageBox*> preMadeImageBoxesVec;

	RENDER_TARGET::TEXT::TextFObj * _textFObj;
	RENDER::RBox* _rBox;		//which rbox get(which shader? use same rbox)
};