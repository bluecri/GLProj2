#pragma once

#include "stdafx.h"

#include "Box.h"
#include "RText.h"


namespace RESOURCE { class Texture; class TextureManager; }
namespace RENDER { class RBox; }
namespace SHADER { class ShaderText; }

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
	TextBox(int line, int len, glm::vec2 pos, int fontSize, const std::string &textTextureFileName, const std::string &type, SHADER::ShaderText* shaderText);

	TextBox(int line, int len, glm::vec2 pos, int fontSize, RESOURCE::Texture * texture, SHADER::ShaderText* shaderText);

	TextBox(const TextBox& copy);

	virtual ~TextBox();

	virtual void setBRender(bool bRender) override;;

	virtual void modifyEmptySize(int width, int height);

	virtual void modifyTextBoxSize(int line, int len, int fontSize);

	virtual void modifyTextComment(std::string& str);

	virtual void moveLTPosition(int widthLT, int heightLT);

public:
	static void initPreMade();

public:
	static std::vector<TextBox*> preMadeTextBoxesVec;

	std::weak_ptr<RENDER::RText::DrawElement> drawElement;		// draw elem from renderer ( send to renderer for remove )
	RENDER_TARGET::TEXT::TextFObj * _textFObj;						// fobj
	RENDER::RText* _rText;									// which renderer is used
};