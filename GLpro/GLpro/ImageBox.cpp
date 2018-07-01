#include "ImageBox.h"

#include "src/Resource/TextureManager.h"
#include "src/Shader/ShaderManager.h"
#include "RenderManager.h"
#include "RBox.h"
#include "TextFObj.h"

std::vector<ImageBox*> ImageBox::preMadeImageBoxesVec;

ImageBox::ImageBox(int width, int height, int widthLT, int heightLT, const std::string & textureFileName, const std::string & type, SHADER::ShaderText * shaderText)
	: Box(width, height, widthLT, heightLT)
{
	_rBox = GRendermanager->getRRender<RENDER::RBox, SHADER::ShaderText>(shaderText);

	RENDER_TARGET::TEXT::TextFObj* tempTextFObj = new RENDER_TARGET::TEXT::TextFObj(textureFileName.c_str(), type.c_str(), width, height, widthLT, heightLT);
	drawElem = _rBox->addToDrawList(tempTextFObj);
}

ImageBox::ImageBox(int width, int height, int widthLT, int heightLT, RESOURCE::Texture * texture, SHADER::ShaderText * shaderText)
	: Box(width, height, widthLT, heightLT)
{
	_rBox = GRendermanager->getRRender<RENDER::RBox, SHADER::ShaderText>(shaderText);

	RENDER_TARGET::TEXT::TextFObj* tempTextFObj = new RENDER_TARGET::TEXT::TextFObj(texture, width, height, widthLT, heightLT);
	drawElem = _rBox->addToDrawList(tempTextFObj);
}

// UIEntity 부모자식관계 및 event 함수 바인딩 제외 Copy

ImageBox::ImageBox(const ImageBox & copy) : Box(copy._width, copy._height, copy._widthLT, copy._heightLT)
{
	_rBox = copy._rBox;
	RENDER_TARGET::TEXT::TextFObj* tempTextFObj = new RENDER_TARGET::TEXT::TextFObj(copy.drawElem->_texture, copy._width, copy._height, copy._widthLT, copy._heightLT);
	drawElem = _rBox->addToDrawList(tempTextFObj);
}

ImageBox::~ImageBox()
{

}

void ImageBox::setBRender(bool bRender)
{
	drawElem->setBRender(bRender);		// render flag(bool) modify
}

void ImageBox::modifyEmptySize(int width, int height)
{
	printf_s("ImageBox modifyEmptySize is not vaild. Use modifyImageBoxSize instead.\n");
}

void ImageBox::modifyImageBoxSize(int width, int height)
{
	drawElem->changeBoxSize(height, width);
}

void ImageBox::moveLTPosition(int widthLT, int heightLT)
{
	drawElem->setPos(glm::vec2(_widthLT, _heightLT));
}

void ImageBox::initPreMade()
{
	SHADER::ShaderText* shaderText = GShaderManager->m_addShader<SHADER::ShaderText>(ENUM_SHADER_TYPE::SHADER_TYPE_TEXT, "data/Shader/TextVertexShader.vertexshader", "data/Shader/TextVertexShader.fragmentshader");
	RESOURCE::Texture* blockTexture = GTextureManager->getTextureWithFileName("data/Texture/blockBit.bmp", "bmp");
	RESOURCE::Texture* bluegreyTexture = GTextureManager->getTextureWithFileName("data/Texture/bluegrey.dds", "dds");
	RESOURCE::Texture* ironTexture = GTextureManager->getTextureWithFileName("data/Texture/iron.dds", "dds");
	RESOURCE::Texture* paperTexture = GTextureManager->getTextureWithFileName("data/Texture/paperBit.bmp", "bmp");
	RESOURCE::Texture* waterTexture = GTextureManager->getTextureWithFileName("data/Texture/water.dds", "dds");

	ImageBox* tempImageBox = new ImageBox(100, 40, 40, 40, blockTexture, shaderText);
	tempImageBox->setBRender(false);
	preMadeImageBoxesVec.push_back(tempImageBox);

	tempImageBox = new ImageBox(100, 40, 40, 40, bluegreyTexture, shaderText);
	tempImageBox->setBRender(false);
	preMadeImageBoxesVec.push_back(tempImageBox);

	tempImageBox = new ImageBox(100, 40, 40, 40, ironTexture, shaderText);
	tempImageBox->setBRender(false);
	preMadeImageBoxesVec.push_back(tempImageBox);

	tempImageBox = new ImageBox(100, 40, 40, 40, paperTexture, shaderText);
	tempImageBox->setBRender(false);
	preMadeImageBoxesVec.push_back(tempImageBox);

	tempImageBox = new ImageBox(100, 40, 40, 40, waterTexture, shaderText);
	tempImageBox->setBRender(false);
	preMadeImageBoxesVec.push_back(tempImageBox);
}

