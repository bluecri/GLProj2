#ifndef __TEXTURE__H__
#define __TEXTURE__H__

#include "../../3rdParty/TextureLoader/textureLoader.h"

#include <string>

namespace RESOURCE {
	class Texture {
		public:
			Texture(std::string fileName, std::string type) {
				_fileName = fileName;
				if (type.compare("DDS"))
				{
					_textureID = loadDDS(fileName.c_str());
				}
				else
				{
					_textureID = loadBMP_custom(fileName.c_str());
				}
			};

			virtual ~Texture() {
				glDeleteTextures(1, &_textureID);
			};
			void bind() const;
			void unbind() const;
			const std::string& getFileName();

		private:
			GLuint _textureID;
			std::string _fileName;
	};

}

#endif