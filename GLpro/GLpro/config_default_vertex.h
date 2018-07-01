#pragma once

#include "stdafx.h"

const glm::vec3 defaultVertex_QuadScreeen_arr[] = 
{
	glm::vec3(-1.0f, -1.0f, 0.0f),
	glm::vec3(1.0f, -1.0f, 0.0f),
	glm::vec3(-1.0f,  1.0f, 0.0f),
	glm::vec3(-1.0f,  1.0f, 0.0f),
	glm::vec3(1.0f, -1.0f, 0.0f),
	glm::vec3(1.0f,  1.0f, 0.0f)
};

const std::vector<glm::vec3> defaultVertex_QuadScreeen =
std::vector<glm::vec3>(defaultVertex_QuadScreeen_arr, defaultVertex_QuadScreeen_arr + sizeof(defaultVertex_QuadScreeen_arr) / sizeof(defaultVertex_QuadScreeen_arr[0]));