#include "./Texture.h"

namespace RESOURCE
{
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
