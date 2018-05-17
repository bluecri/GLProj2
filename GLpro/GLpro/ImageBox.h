#pragma once

#include "Box.h"
#include "src/Resource/TextureManager.h"
namespace RESOURCE { class Texture; class TextureManager; }

class ImageBox : public Box
{
public:
	ImageBox(int width, int height, int widthLT, int heightLT, const std::string &textureFileName, const std::string &type) : Box(width, height, widthLT, heightLT)
	{
		texture = GTextureManager->getTextureWithFileName(textureFileName, type);
	}

	ImageBox(int width, int height, int widthLT, int heightLT) : Box(width, height, widthLT, heightLT)
	{
		// non texture
	}

	virtual void render(float deltaTime)
	{

	}

private:
	textFObj
};