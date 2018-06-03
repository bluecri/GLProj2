#include "stdafx.h"
#include "AimTextUIObj.h"

#include "src/Resource/TextureManager.h"
#include "src/Shader/ShaderManager.h"
#include "src/window.h"

AimTextUIObj::AimTextUIObj()
	: TextBox(1, 1, glm::vec2(GWindow->_windowWidth / 2 - 19, GWindow->_windowHeight/2 + 19), 38, GTextureManager->getTextureWithFileName("data/Texture/Holstein.DDS", "dds"), GShaderManager->m_addShader<SHADER::ShaderText>(ENUM_SHADER_TYPE::SHADER_TYPE_TEXT, "data/Shader/TextVertexShader.vertexshader", "data/Shader/TextVertexShader.fragmentshader"))
{
	//  glm::vec2(GWindow->_windowHeight / 2, GWindow->_windowWidth / 2)
	//modifyTextComment(std::string("+"));
	std::string aimStr = "";
	aimStr.append(1, 0);
	modifyTextComment(aimStr);
	setBRender(true);
}

void AimTextUIObj::setAimPositionWithQuat(const glm::quat& curQuat, const glm::quat& targetQuat)
{
	glm::mat3 curMat3 = glm::toMat3(curQuat);
	glm::mat3 targetMat3 = glm::toMat3(targetQuat);

	// pointOnTargatPlane = point(curZVec) - planeNUnitVector(targetZVec) * dist(point ~ plane(targetZVec))
	glm::vec3 pointOnTargatPlane = glm::normalize(curMat3[2] - targetMat3[2] * (glm::dot(curMat3[2], targetMat3[2])));

	float angleHor = glm::acos(glm::dot(pointOnTargatPlane, targetMat3[0]));
	float angleVert = glm::acos(glm::dot(pointOnTargatPlane, targetMat3[1]));

	printf_s("%f %f\n", angleHor, angleVert);
}
