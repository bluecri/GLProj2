#pragma once
#include "stdafx.h"
#include "Box.h"


namespace RESOURCE { class Texture; class TextureManager; }
namespace RENDER_TARGET { namespace TEXT { class TextFObj; } }
namespace RENDER { class RBox; }
namespace SHADER { class ShaderText; }


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
	ImageBox(int width, int height, int widthLT, int heightLT, const std::string &textureFileName, const std::string &type, SHADER::ShaderText* shaderText);
	ImageBox(int width, int height, int widthLT, int heightLT, RESOURCE::Texture* texture, SHADER::ShaderText* shaderText);

	// UIEntity 부모자식관계 및 event 함수 바인딩 제외 Copy
	ImageBox(const ImageBox& copy);

	virtual ~ImageBox();

	virtual void setBRender(bool bRender) override;

	virtual void modifyEmptySize(int width, int height);
	virtual void modifyImageBoxSize(int width, int height);
	virtual void moveLTPosition(int widthLT, int heightLT);

public:
	static void initPreMade();

public:
	static std::vector<ImageBox*> preMadeImageBoxesVec;

	std::shared_ptr<RENDER_TARGET::TEXT::TextFObj> drawElem;
	RENDER::RBox* _rBox;		//which rbox get(which shader? use same rbox)
};