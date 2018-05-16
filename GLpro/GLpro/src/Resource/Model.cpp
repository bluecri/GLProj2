#include "./Model.h"

namespace RESOURCE 
{
	Model::Model(const std::string & fileName, bool createEbo)
	{
		_bEbo = createEbo;

		genVao();
		bind();

		loadObjFile(fileName);

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
	Model::Model(std::vector<glm::vec3>& vertices, std::vector<glm::vec2>& uvs, std::vector<glm::vec3>& normals, bool createEbo)
	{
		_bEbo = createEbo;
		_out_vertices = vertices;
		_out_uvs = uvs;
		_out_normals = normals;

		genVao();
		bind();

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

	Model::~Model()
	{
		if (_bEbo)
		{ 
			glDeleteBuffers(1, &_ebo);
		}
		else
		{
			glDeleteBuffers(1, &_vbo);
		}

		glDeleteVertexArrays(1, &_vao);
	}

	void Model::bind() const
	{
		glBindVertexArray(_vao);
	}

	void Model::unbind() const
	{
		glBindVertexArray(0);
	}

	void Model::genVao()
	{
		glGenVertexArrays(1, &_vao);
	}

	int Model::getGLCount()
	{
		if (_bEbo)
		{
			return out_indices.size();
		}
		else
		{
			return _vertices.size();
		}
	}

	bool Model::isEBO()
	{
		return _bEbo;
	}

	// {vertices, uvs, normals} -> {structVertices}
	void Model::createBuffer()
	{
		Struct_Vertex tempVertexWithTexture;

		for (int i = 0; i < _out_vertices.size(); i++) {
			tempVertexWithTexture.vertice = _out_vertices[i];
			tempVertexWithTexture.uv = _out_uvs[i];
			tempVertexWithTexture.normal = _out_normals[i];
			_vertices.push_back(tempVertexWithTexture);
		}
	}

	// {vertices, uvs, normals} -> {structVertices, indices}
	void Model::createStructVertexAndIndice()
	{
		createVBOWithVertex(_out_vertices, _out_uvs, _out_normals, out_indices, _vertices);
	}

	// {file} -> {vertices, uvs, normals}
	bool Model::loadObjFile(const std::string &fileName)
	{
		bool res = loadOBJ(fileName.c_str(), _out_vertices, _out_uvs, _out_normals);
		if (!res) {
			printf("%s load obj fail\n", fileName);
			return false;
		}
		return true;
	}

	//{structVertices, indices} -> vbo, ebo
	void Model::createVboOrEbo(bool createEbo)
	{
		if (createEbo)
		{
			// EBO buffer bind
			glGenBuffers(1, &_ebo);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, out_indices.size() * sizeof(out_indices[0]), out_indices.data(), GL_STATIC_DRAW);
		}
		else
		{
			// VBO buffer bind
			glGenBuffers(1, &_vbo);
			glBindBuffer(GL_ARRAY_BUFFER, _vbo);
			glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(_vertices[0]), _vertices.data(), GL_STATIC_DRAW);
		}
		
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Struct_Vertex), (void*)(0));
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Struct_Vertex), (void*)(sizeof(float) * 3));
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Struct_Vertex), (void*)(sizeof(float) * 5));
				
	}
}
