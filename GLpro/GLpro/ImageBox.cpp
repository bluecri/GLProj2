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

	_textFObj = new RENDER_TARGET::TEXT::TextFObj(textureFileName.c_str(), type.c_str(), width, height, widthLT, heightLT);
	_rBox->addToDrawList(_textFObj);
}

ImageBox::ImageBox(int width, int height, int widthLT, int heightLT, RESOURCE::Texture * texture, SHADER::ShaderText * shaderText)
	: Box(width, height, widthLT, heightLT)
{
	_rBox = GRendermanager->getRRender<RENDER::RBox, SHADER::ShaderText>(shaderText);

	_textFObj = new RENDER_TARGET::TEXT::TextFObj(texture, width, height, widthLT, heightLT);
	_rBox->addToDrawList(_textFObj);
}

// UIEntity 부모자식관계 및 event 함수 바인딩 제외 Copy

inline ImageBox::ImageBox(const ImageBox & copy) : Box(copy._width, copy._height, copy._widthLT, copy._heightLT)
{
	_rBox = copy._rBox;
	_textFObj = new RENDER_TARGET::TEXT::TextFObj(copy._textFObj->_texture, copy._width, copy._height, copy._widthLT, copy._heightLT);
	_rBox->addToDrawList(_textFObj);
}

inline ImageBox::~ImageBox()
{
	_rBox->destructor(_textFObj);		//destroy render Target in Renderer's container
	delete _textFObj;
}

inline void ImageBox::setBRender(bool bRender)
{
	_textFObj->setBRender(bRender);		// render flag(bool) modify
}

inline void ImageBox::modifyEmptySize(int width, int height)
{
	printf_s("ImageBox modifyEmptySize is not vaild. Use modifyImageBoxSize instead.\n");
}

inline void ImageBox::modifyImageBoxSize(int width, int height)
{
	_textFObj->changeBoxSize(height, width);
}

inline void ImageBox::moveLTPosition(int widthLT, int heightLT)
{
	_textFObj->setPos(glm::vec2(_widthLT, _heightLT));
}

inline void ImageBox::initPreMade()
{
	SHADER::ShaderText* shaderText = GShaderManager->m_addShader<SHADER::ShaderText>(ENUM_SHADER_TYPE::SHADER_TYPE_TEXT, "data/Shader/TextVertexShader.vertexshader", "data/Shader/TextVertexShader.fragmentshader");
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

