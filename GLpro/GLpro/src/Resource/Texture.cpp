#include "./Texture.h"

namespace RESOURCE
{
	Texture::Texture(std::string fileName, std::string type)
	{
		_fileName = fileName;
		if (type.compare("DDS"))
		{
			_textureID = loadDDS(fileName.c_str());
		}
		else
		{
			_textureID = loadBMP_custom(fileName.c_str());
		}
	}

	Texture::~Texture()
	{
		glDeleteTextures(1, &_textureID);
	}			

	void Texture::bind() const
	{
		glBindTexture(GL_TEXTURE_2D, _textureID);
	}

	void Texture::unbind() const
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	const std::string& Texture::getFileName()
	{
		return _fileName;
	}
}
