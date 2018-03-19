#pragma once
#include <glm/glm.hpp>

struct Struct_VertexWithTexture {
	glm::vec3 vertice;
	glm::vec2 uv;
	glm::vec3 normal;

	bool operator<(const Struct_VertexWithTexture other) const
	{
		return memcmp((void*)this, (void*)&other, sizeof(Struct_VertexWithTexture)) > 0;
	}
};
