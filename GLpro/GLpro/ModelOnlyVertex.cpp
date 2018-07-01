#include "ModelOnlyVertex.h"


namespace RESOURCE
{
	ModelOnlyVertex::ModelOnlyVertex(const std::vector<glm::vec3>& vertices, bool createEbo)
	{
		_bEbo = createEbo;

		genVao();
		bind();

		_out_vertices = vertices;

		if (_bEbo)
		{
			createStructVertexAndIndice();
		}
		else
		{
			createBuffer();
		}

		createVboOrEbo(_bEbo);

		unbind();
	}

	ModelOnlyVertex::~ModelOnlyVertex()
	{
		if (_bEbo)
		{
			glDeleteBuffers(1, &_ebo);
			glDeleteBuffers(1, &_vbo);
		}
		else
		{
			glDeleteBuffers(1, &_vbo);
		}

		glDeleteVertexArrays(1, &_vao);
	}

	void ModelOnlyVertex::bind() const
	{
		glBindVertexArray(_vao);
	}

	void ModelOnlyVertex::unbind() const
	{
		glBindVertexArray(0);
	}

	void ModelOnlyVertex::genVao()
	{
		glGenVertexArrays(1, &_vao);
	}

	void ModelOnlyVertex::render()
	{
		if (_bEbo)
		{
			glDrawElements(
				GL_TRIANGLES,      // mode
				getGLCount(),    // count
				GL_UNSIGNED_SHORT,   // type
				(void*)0         // element array buffer offset
			);
		}
		else
		{
			glDrawArrays(GL_TRIANGLES, 0, getGLCount());
		}

	}

	int ModelOnlyVertex::getGLCount()
	{
		if (_bEbo)
		{
			return static_cast<int>(_elemVertices.size());
		}
		else
		{
			return static_cast<int>(_vertices.size());
		}
	}

	bool ModelOnlyVertex::isEBO()
	{
		return _bEbo;
	}

	// {vertices, uvs, normals} -> {structVertices}
	void ModelOnlyVertex::createBuffer()
	{
		Struct_OnlyVertex tempVertexWithTexture;

		_vertices.reserve(_out_vertices.size());

		for (int i = 0; i < _out_vertices.size(); i++) {
			tempVertexWithTexture.vertice = _out_vertices[i];
			_vertices.push_back(tempVertexWithTexture);
		}
	}

	// {vertices} -> {structVertices, indices}
	void ModelOnlyVertex::createStructVertexAndIndice()
	{
		createVBOWithOnlyVertex(_out_vertices, _elemVertices, _vertices);
	}

	//{structVertices, indices} -> vbo, ebo
	void ModelOnlyVertex::createVboOrEbo(bool createEbo)
	{
		if (createEbo)
		{
			// EBO buffer bind
			glGenBuffers(1, &_ebo);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, _elemVertices.size() * sizeof(_elemVertices[0]), _elemVertices.data(), GL_STATIC_DRAW);
		}

		// VBO buffer bind
		glGenBuffers(1, &_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, _vbo);
		glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(_vertices[0]), _vertices.data(), GL_STATIC_DRAW);


		glEnableVertexAttribArray(0);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Struct_OnlyVertex), (void*)(0));

	}
}
