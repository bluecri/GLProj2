#pragma once

#include "../../stdafx.h"

#define ASCII_LEN_NUM 128

namespace RESOURCE
{
	class TextBuffer
	{
	public:
		TextBuffer(int lineN, int lengthN, int fontSize);

		virtual ~TextBuffer();

		void bind();
		void setUVBufferWithString(std::string& printStr);	// can be faster(변경문자만 변경)
		void setPosBuffer(int lineN, int lengthN, int fontSize);	// can be faster(변경문자만 변경)
		
		void unbind() const;

		int getGLCount();

	private:
		void genVao();
		virtual void createBuffer();
		void updatePosVBO();
		void updateUVVBO();

	private:
		GLuint	_vao;		//vertex array

		GLuint	_posVBO;		// vertex buffer
		GLuint	_uvVBO;		// element buffer

		bool _bUpdatePosVBO;
		bool _bUpdateUVVBO;
		GLsizeiptr _prevPosVBOSize;
		GLsizeiptr _prevUVVBOSize;

		std::vector<glm::vec2> _vertexes;
		std::vector<glm::vec2> _printUVs;		// print에 사용될 uv vector
		std::vector<glm::vec2> _preMadeUVs;		// 미리 texture의 각 문자에 해당되는 uvs
		
		int _lineN;
		int _lengthN;
		int _fontSize;
	};

}