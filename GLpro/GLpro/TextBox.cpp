#include "TextBox.h"

#include "src/Resource/TextureManager.h"
#include "src/Shader/ShaderManager.h"
#include "TextFObj.h"
#include "TextBuffer.h"
#include "RenderManager.h"
#include "RBox.h"
#include "RText.h"

std::vector<TextBox*> TextBox::preMadeTextBoxesVec;

TextBox::TextBox(int line, int len, glm::vec2 pos, int fontSize, const std::string & textTextureFileName, const std::string & type, SHADER::ShaderText * shaderText)
	: Box(len * static_cast<int>(fontSize), line * static_cast<int>(fontSize), static_cast<int>(pos.x), static_cast<int>(pos.y))	// pos & size calc
{
	_rText = GRendermanager->getRRender<RENDER::RText, SHADER::ShaderText>(shaderText);
	RENDER_TARGET::TEXT::TextFObj* tempTextFObj = new RENDER_TARGET::TEXT::TextFObj(textTextureFileName.c_str(), type.c_str(), line, len, pos, fontSize);

	/*	textbox vertex gen process
	glm::vec2 vertex_up_left = glm::vec2(len * _fontSize / 2, -line * _fontSize);
	glm::vec2 vertex_up_right = glm::vec2(len * _fontSize / 2 + _fontSize, -line * _fontSize);
	glm::vec2 vertex_down_right = glm::vec2(len * _fontSize / 2 + _fontSize, -line * _fontSize - _fontSize);
	glm::vec2 vertex_down_left = glm::vec2(len * _fontSize / 2, -line * _fontSize - _fontSize);
	*/

	_drawElement = _rText->addToDrawList(tempTextFObj, nullptr);
}

TextBox::TextBox(int line, int len, glm::vec2 pos, int fontSize, RESOURCE::Texture * texture, SHADER::ShaderText * shaderText)
	: Box(len * static_cast<int>(fontSize), line * static_cast<int>(fontSize), static_cast<int>(pos.x), static_cast<int>(pos.y))
{
	_rText = GRendermanager->getRRender<RENDER::RText, SHADER::ShaderText>(shaderText);
	RENDER_TARGET::TEXT::TextFObj* tempTextFObj = new RENDER_TARGET::TEXT::TextFObj(texture, line, len, pos, fontSize);

	_drawElement = _rText->addToDrawList(tempTextFObj, nullptr);
}

TextBox::TextBox(const TextBox & copy) : Box(copy._width, copy._height, copy._widthLT, copy._heightLT)
{
	_rText = copy._rText;
	auto copyShared = copy._drawElement;
	
	RENDER_TARGET::TEXT::TextFObj* copyTextFObj = copyShared->first;
	RESOURCE::TextBuffer* copyTextBuffer = copyTextFObj->_textBuffer;
	RENDER_TARGET::TEXT::TextFObj* tempTextFObj = new RENDER_TARGET::TEXT::TextFObj(copyTextFObj->_texture, copyTextBuffer->getLineN(), copyTextBuffer->getLenN(), copyTextFObj->_pos, copyTextBuffer->getFontSize());
	_drawElement = _rText->addToDrawList(tempTextFObj, nullptr);
}

TextBox::~TextBox()
{
}

void TextBox::setBRender(bool bRender)
{
	_drawElement->first->setBRender(bRender);		// render flag(bool) modify
}

void TextBox::modifyEmptySize(int width, int height)
{
	// pos 변경은 FObj에서
	printf_s("TextBox modifyEmptySize is not vaild\n");
}

void TextBox::modifyTextBoxSize(int line, int len, int fontSize)
{
	_drawElement->first->changeFrame(line, len, fontSize);
}

void TextBox::modifyTextComment(std::string & str)
{
	_drawElement->first->changePrintStr(str);
}

void TextBox::moveLTPosition(int widthLT, int heightLT)
{
	_drawElement->first->setPos(glm::vec2(_widthLT, _heightLT));
}

void TextBox::initPreMade()
{
	SHADER::ShaderText* shaderText = GShaderManager->m_addShader<SHADER::ShaderText>(ENUM_SHADER_TYPE::SHADER_TYPE_TEXT, "data/Shader/TextVertexShader.vertexshader", "data/Shader/TextVertexShader.fragmentshader");
	RESOURCE::Texture* asciiTexture = GTextureManager->getTextureWithFileName("data/Texture/Holstein.DDS", "dds");

	TextBox* tempTextBox = new TextBox(6, 80, glm::vec2(300, 300), 16, asciiTexture, shaderText);
	tempTextBox->setBRender(false);
	preMadeTextBoxesVec.push_back(tempTextBox);

	tempTextBox = new TextBox(1, 20, glm::vec2(300, 300), 20, asciiTexture, shaderText);
	tempTextBox->setBRender(false);
	preMadeTextBoxesVec.push_back(tempTextBox);

	tempTextBox = new TextBox(1, 80, glm::vec2(300, 300), 16, asciiTexture, shaderText);
	tempTextBox->setBRender(false);
	preMadeTextBoxesVec.push_back(tempTextBox);
}
