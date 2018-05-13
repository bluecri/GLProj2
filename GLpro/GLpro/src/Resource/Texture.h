#ifndef __TEXTURE__H__
#define __TEXTURE__H__

#include "../../stdafx.h"

namespace RESOURCE 
{
	class Texture
	{
	public:
		Texture(std::string fileName, std::string type);
		virtual ~Texture();

		void bind() const;
		void unbind() const;
		const std::string& getFileName();

	private:
		GLuint _textureID;
		std::string _fileName;
	};
}

#endif