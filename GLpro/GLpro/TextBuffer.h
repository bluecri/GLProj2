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
		void setUVBufferWithString(std::string& printStr);	// can be faster(���湮�ڸ� ����)
		void setPosBuffer(int lineN, int lengthN, int fontSize);	// can be faster(���湮�ڸ� ����)
		
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
		std::vector<glm::vec2> _printUVs;		// print�� ���� uv vector
		std::vector<glm::vec2> _preMadeUVs;		// �̸� texture�� �� ���ڿ� �ش�Ǵ� uvs
		
		int _lineN;
		int _lengthN;
		int _fontSize;
	};

}