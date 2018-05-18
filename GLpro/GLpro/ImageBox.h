#pragma once

#include "Box.h"
#include "src/Resource/TextureManager.h"
#include "TextFObj.h"
#include "RBox.h"

namespace RESOURCE { class Texture; class TextureManager; }
namespace RENDER { class RBox;}

class ImageBox : public Box
{
public:
	ImageBox(int width, int height, int widthLT, int heightLT, const std::string &textureFileName, const std::string &type) : Box(width, height, widthLT, heightLT)
	{
		textFObj = new RENDER_TARGET::TEXT::TextFObj(textureFileName.c_str(), type.c_str(), width, height, widthLT, heightLT);
		rBox->addToDrawList(textFObj);
	}

	virtual ~ImageBox()
	{
		rBox->destructor(textFObj);
		delete textFObj;
	}

	virtual void singleSetBRender(bool bRender)
	{
		textFObj->setRender(bRender);
	};


private:
	RENDER_TARGET::TEXT::TextFObj * textFObj;
	RENDER::RBox* rBox;		//which rbox get(which shader? use same rbox)
};