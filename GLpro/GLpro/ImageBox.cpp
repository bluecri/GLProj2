#include "stdafx.h"
#include "ImageBox.h"

inline ImageBox::ImageBox(int width, int height, int widthLT, int heightLT, RESOURCE::Texture * texture, SHADER::ShaderText * shaderText) : Box(width, height, widthLT, heightLT)
{
	_rBox = GRendermanager->getRRender<RENDER::RBox, SHADER::ShaderText>(shaderText);

	_textFObj = new RENDER_TARGET::TEXT::TextFObj(texture, width, height, widthLT, heightLT);
	_rBox->addToDrawList(_textFObj);
}
