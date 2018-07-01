#ifndef VBOINDEXER_H
#define VBOINDEXER_H

#include <glm/glm.hpp>
#include <string.h> 
#include <vector>
#include <map>

#include "./Vertex.h"

void createVBOWithVertex(
	std::vector<glm::vec3> & in_vertices,
	std::vector<glm::vec2> & in_uvs,
	std::vector<glm::vec3> & in_normals,

	std::vector<unsigned short> & out_indices,
	std::vector<Struct_Vertex> &outStructVertexVec
);

void createVBOWithOnlyVertex(std::vector<glm::vec3>& in_vertices, std::vector<unsigned short>& out_indices, std::vector<Struct_OnlyVertex>& outVertexVithTextureVec);

bool is_near(float v1, float v2);
bool getSimilarVertexIndex_fast(
	Struct_Vertex & packed,
	std::map<Struct_Vertex, unsigned short> & VertexToOutIndex,
	unsigned short & result
);

bool getSimilarOnlyWIthVertexIndex_fast(Struct_OnlyVertex & packed, std::map<Struct_OnlyVertex, unsigned short>& VertexToOutIndex, unsigned short & result);

/*
void indexVBO_TBN(
	std::vector<glm::vec3> & in_vertices,
	std::vector<glm::vec2> & in_uvs,
	std::vector<glm::vec3> & in_normals,
	std::vector<glm::vec3> & in_tangents,
	std::vector<glm::vec3> & in_bitangents,

	std::vector<unsigned short> & out_indices,
	std::vector<glm::vec3> & out_vertices,
	std::vector<glm::vec2> & out_uvs,
	std::vector<glm::vec3> & out_normals,
	std::vector<glm::vec3> & out_tangents,
	std::vector<glm::vec3> & out_bitangents
);
*/
#endif