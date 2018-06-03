#include "stdafx.h"
#include "TextBuffer.h"

RESOURCE::TextBuffer::TextBuffer(int lineN, int lengthN, int fontSize)
	: _lineN(lineN), _lengthN(lengthN), _fontSize(fontSize), _bUpdateVBO(false), _bUpdateUVBO(false),
	_prevVBOSize(0), _prevUVBOSize(0)
{
	_bBox = false;
	
	genVao();
	bind();
	createBuffer();
	unbind();
}

RESOURCE::TextBuffer::TextBuffer(int width, int height)
{
	_bBox = true;

	genVao();
	bind();
	createBuffer();
	unbind();
}

RESOURCE::TextBuffer::~TextBuffer()
{
	glDeleteBuffers(1, &_vbo);
	glDeleteBuffers(1, &_uvbo);

	glDeleteVertexArrays(1, &_vao);
}

void RESOURCE::TextBuffer::bind()
{
	glBindVertexArray(_vao);

	if (_bUpdateVBO)
	{
		updateVBO();
		_bUpdateVBO = false;
	}
	if (_bUpdateUVBO)
	{
		updateUVVBO();
		_bUpdateUVBO = false;
	}

	return;
}

void RESOURCE::TextBuffer::setUVBufferWithString(std::string & printStr)
{
	if (_bBox)
	{
		printf_s("[LOG] : TextBuffer::setUVBufferWithString is called in bBox\n");
		return;
	}
	_printUVs.clear();

	for (int i = 0; i < printStr.length(); i++)
	{
		int alpha = (int)printStr[i] * 6;	// pre maden uv # = 6
		for (int k = 0; k < 6; k++)
		{
			_printUVs.push_back(_preMadeUVs[alpha + k]);
		}
	}

	//	updateUVVBO(); -> 실제 사용될 시에(glBindVertexArray) update한다.
	_bUpdateUVBO = true;

	return;
}

void RESOURCE::TextBuffer::updateTextVBO(int lineN, int lengthN, int fontSize)
{
	if (_bBox)
	{
		printf_s("[LOG] : TextBuffer::updateTextVBO is called in bBox\n");
		return;
	}
	_vertexes.clear();

	// Fill _vertexes vector (static)
	privateSetPosBuffer(lineN, lengthN, fontSize);

	//	updateVBO(); -> 실제 사용될 시에(glBindVertexArray) update한다.
	_bUpdateVBO = true;

	return;
}

void RESOURCE::TextBuffer::updateBoxVBO(int width, int height)
{
	if (!_bBox)
	{
		printf_s("[LOG] : :TextBuffer::updateBoxVBO is called in not bBox\n");
		return;
	}
	privateSetBoxSizeBuffer(width, height);
	_bUpdateVBO = true;
}

void RESOURCE::TextBuffer::unbind() const
{
	glBindVertexArray(0);
}

void RESOURCE::TextBuffer::render()
{
	glDrawArrays(GL_TRIANGLES, 0, getGLCount());
}

int RESOURCE::TextBuffer::getGLCount()
{
	// print가능한 모든 칸 수(_printUVs 초과시) vs print할 uv수
	return min(_printUVs.size(), _vertexes.size());
}

void RESOURCE::TextBuffer::genVao()
{
	glGenVertexArrays(1, &_vao);
}

void RESOURCE::TextBuffer::createBuffer()
{
	if (_bBox)
	{
		// one box UV ( 00, 10, 11, 01 )
		glm::vec2 uv_up_left = glm::vec2(0, 0);
		glm::vec2 uv_up_right = glm::vec2(1, 0);
		glm::vec2 uv_down_right = glm::vec2(1, 1);
		glm::vec2 uv_down_left = glm::vec2(0, 1);

		_printUVs.push_back(uv_up_left);
		_printUVs.push_back(uv_down_left);
		_printUVs.push_back(uv_up_right);

		_printUVs.push_back(uv_down_right);
		_printUVs.push_back(uv_up_right);
		_printUVs.push_back(uv_down_left);

		privateSetBoxSizeBuffer(_width, _height);
	}
	else
	{
		// Fill _preMadeUVs vector
		for (unsigned int charIndex = 0; charIndex < ASCII_LEN_NUM; charIndex++) {
			float uv_x = (charIndex % 16) / 16.0f;
			float uv_y = (charIndex / 16) / 16.0f;

			glm::vec2 uv_up_left = glm::vec2(uv_x, uv_y);
			glm::vec2 uv_up_right = glm::vec2(uv_x + 1.0f / 16.0f, uv_y);
			glm::vec2 uv_down_right = glm::vec2(uv_x + 1.0f / 16.0f, (uv_y + 1.0f / 16.0f));
			glm::vec2 uv_down_left = glm::vec2(uv_x, (uv_y + 1.0f / 16.0f));

			_preMadeUVs.push_back(uv_up_left);
			_preMadeUVs.push_back(uv_down_left);
			_preMadeUVs.push_back(uv_up_right);

			_preMadeUVs.push_back(uv_down_right);
			_preMadeUVs.push_back(uv_up_right);
			_preMadeUVs.push_back(uv_down_left);
		}

		privateSetPosBuffer(_lineN, _lengthN, _fontSize);
	}

	glGenBuffers(1, &_vbo);
	glGenBuffers(1, &_uvbo);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	_prevVBOSize = _vertexes.size() * sizeof(glm::vec2);		// save prev size for glBindBuffer(NULL)
	_prevUVBOSize = _printUVs.size() * sizeof(glm::vec2);

	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glBufferData(GL_ARRAY_BUFFER, _prevVBOSize, &_vertexes[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, _uvbo);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glBufferData(GL_ARRAY_BUFFER, _prevUVBOSize, _printUVs.data(), GL_STATIC_DRAW);
	//glBufferData(GL_ARRAY_BUFFER, _prevUVBOSize, static_cast<void*>(&_printUVs), GL_STATIC_DRAW);
}


void RESOURCE::TextBuffer::updateVBO()
{
	// should be called when glBindVertexArray(_vao) is already called
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	//glBufferData(GL_ARRAY_BUFFER, _prevVBOSize, NULL, GL_STATIC_DRAW);
	_prevVBOSize = _vertexes.size() * sizeof(glm::vec2);
	glBufferData(GL_ARRAY_BUFFER, _prevVBOSize, &_vertexes[0], GL_STATIC_DRAW);
}

void RESOURCE::TextBuffer::updateUVVBO()
{
	// should be called when glBindVertexArray(_vao) is already called
	glBindBuffer(GL_ARRAY_BUFFER, _uvbo);
	//glBufferData(GL_ARRAY_BUFFER, _prevUVBOSize, NULL, GL_STATIC_DRAW);
	_prevUVBOSize = _printUVs.size() * sizeof(glm::vec2);
	glBufferData(GL_ARRAY_BUFFER, _prevUVBOSize, &_printUVs[0], GL_STATIC_DRAW);
}

// text vertices create function with line, length, fontsize
void RESOURCE::TextBuffer::privateSetPosBuffer(int lineN, int lengthN, int fontSize)
{
	// Fill _vertexes vector (static)
	for (int line = 0; line < _lineN; line++) {
		for (int len = 0; len < _lengthN; len++) {
			glm::vec2 vertex_up_left = glm::vec2(len * _fontSize / 2, -line * _fontSize);
			glm::vec2 vertex_up_right = glm::vec2(len * _fontSize / 2 + _fontSize, -line * _fontSize);
			glm::vec2 vertex_down_right = glm::vec2(len * _fontSize / 2 + _fontSize, -line * _fontSize - _fontSize);
			glm::vec2 vertex_down_left = glm::vec2(len * _fontSize / 2, -line * _fontSize - _fontSize);

			_vertexes.push_back(vertex_up_left);
			_vertexes.push_back(vertex_down_left);
			_vertexes.push_back(vertex_up_right);

			_vertexes.push_back(vertex_down_right);
			_vertexes.push_back(vertex_up_right);
			_vertexes.push_back(vertex_down_left);
		}
	}
}

// 1 box vertex create function with width and height
void RESOURCE::TextBuffer::privateSetBoxSizeBuffer(int width, int height)
{
	glm::vec2 vertex_up_left = glm::vec2(0, 0);
	glm::vec2 vertex_up_right = glm::vec2(width, 0);
	glm::vec2 vertex_down_right = glm::vec2(width, -height);
	glm::vec2 vertex_down_left = glm::vec2(0, -height);

	_vertexes.push_back(vertex_up_left);
	_vertexes.push_back(vertex_down_left);
	_vertexes.push_back(vertex_up_right);

	_vertexes.push_back(vertex_down_right);
	_vertexes.push_back(vertex_up_right);
	_vertexes.push_back(vertex_down_left);
}
