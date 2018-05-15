#pragma once

#include "stdafx.h"

namespace RESOURCE
{
	class TextBuffer;
	class Texture;
}

namespace RENDER_TARGET
{
	namespace TEXT
	{
		class TextFObj
		{
		public:
			RESOURCE::TextBuffer * _textBuffer = nullptr;
			RESOURCE::Texture * _texture = nullptr;

			TextFObj(const char* textureFileName, const char* textureType, int lineN, int lengthN, int fontSize);
			~TextFObj() {	};

			void changePrintStr(std::string& inStr);
			void changeFrame(int lineN, int lengthN, int fontSize);

			void setTemporary(float lifeTime);
			void setBindToScreen(glm::vec2& pos, glm::vec2& deltaPos);
			void setPos(glm::vec2& pos);
			void setDeltaPos(glm::vec2& deltaPos);

		public:
			bool _bTemporary;
			float _lifeTime;		// if 0 -> will be deleted.

			bool _bBindToScreen;
			glm::vec2 _pos;
			glm::vec2 _deltaPos;

		private:
			TextFObj() {};

			std::string printStr;
		};
	}
}
