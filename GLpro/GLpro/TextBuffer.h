#pragma once

#include "../../stdafx.h"

#define ASCII_LEN_NUM 128

namespace RESOURCE
{
	class TextBuffer
	{
	public:
		TextBuffer(int lineN, int lengthN, int fontSize);
		TextBuffer(int width, int height);

		virtual ~TextBuffer();

		void bind();
		void unbind() const;

		void setUVBufferWithString(std::string& printStr);	// can be faster(변경문자만 변경)
		void updateTextVBO(int lineN, int lengthN, int fontSize);	// can be faster(변경문자만 변경)
		
		void updateBoxVBO(int width, int height);	// box
		
		void render();
		int getGLCount();

		int getLineN() {return _lineN; }
		int getLenN() {	return _lengthN; }
		int getFontSize() {	return _fontSize; }

	private:
		void genVao();
		virtual void createBuffer();
		void updateVBO();
		void updateUVVBO();
		void privateSetPosBuffer(int lineN, int lengthN, int fontSize);

		void privateSetBoxPosBuffer(int width, int height);	//box

	private:
		GLuint	_vao;		//vertex array

		GLuint	_vbo;		// vertex buffer
		GLuint	_uvbo;		// element buffer

		bool _bUpdateVBO;
		bool _bUpdateUVBO;
		GLsizeiptr _prevVBOSize;
		GLsizeiptr _prevUVBOSize;

		std::vector<glm::vec2> _vertexes;
		std::vector<glm::vec2> _printUVs;		// print에 사용될 uv vector
		std::vector<glm::vec2> _preMadeUVs;		// 미리 texture의 각 문자에 해당되는 uvs

		int _lineN;
		int _lengthN;
		int _fontSize;

		// if box
		int _height;
		int _width;

		bool _bBox;		// whether only 1 box vertex
	};
}
