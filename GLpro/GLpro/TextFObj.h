#pragma once

#include "stdafx.h"
#include "Fobj.h"

namespace RESOURCE
{
	class TextBuffer;
	class Texture;
}

namespace RENDER_TARGET
{
	namespace TEXT
	{
		class TextFObj : public FObj
		{
		public:
			RESOURCE::TextBuffer * _textBuffer = nullptr;
			RESOURCE::Texture * _texture = nullptr;

			TextFObj(const char* textureFileName, const char* textureType, int lineN, int lengthN, glm::vec2 pos, int fontSize);
			TextFObj(RESOURCE::Texture * texture, int lineN, int lengthN, glm::vec2 pos, int fontSize);

			// for box(RBox)
			TextFObj(const char* textureFileName, const char* textureType, int width, int height, int widthLT, int heightLT);
			TextFObj(RESOURCE::Texture * texture, int width, int height, int widthLT, int heightLT);
			~TextFObj() {	};

			void changePrintStr(std::string& inStr);
			void changeFrame(int lineN, int lengthN, int fontSize);

			void setTemporary(float lifeTime);
			void setBindToScreen(glm::vec2& pos, glm::vec2& deltaPos);
			void setPos(glm::vec2& pos);
			void setDeltaPos(glm::vec2& deltaPos);

			// for box
			void changeBoxSize(int height, int width);

		public:
			bool _bTemporary;
			float _lifeTime;		// if 0 -> will be deleted.

			bool _bBindToScreen;
			glm::vec2 _pos;			// print position
			glm::vec2 _deltaPos;

		private:
			TextFObj() {};

			std::string printStr;
			bool _bBox;
			bool _bRender;
		};

	}
}
