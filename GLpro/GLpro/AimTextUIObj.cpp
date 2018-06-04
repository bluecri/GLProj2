#include "stdafx.h"
#include "AimTextUIObj.h"

#include "src/Resource/TextureManager.h"
#include "src/Shader/ShaderManager.h"
#include "TextFObj.h"
#include "src/window.h"
#include "src/Camera/Camera.h"
#include "src/Camera/CameraManager.h"

glm::vec3 AimTextUIObj::_outAimColor = glm::vec3(1.0f, 0.0f, 0.0f);
glm::vec3 AimTextUIObj::_inAimColor = glm::vec3(0.0f, 1.0f, 0.0f);


AimTextUIObj::AimTextUIObj(int textSize)
	: TextBox(1, 1, glm::vec2(GWindow->_windowWidth / 2 - textSize/2, GWindow->_windowHeight/2 + textSize/2), textSize, GTextureManager->getTextureWithFileName("data/Texture/Holstein.DDS", "dds"), GShaderManager->m_addShader<SHADER::ShaderText>(ENUM_SHADER_TYPE::SHADER_TYPE_TEXT, "data/Shader/TextVertexShader.vertexshader", "data/Shader/TextVertexShader.fragmentshader"))
{
	//  glm::vec2(GWindow->_windowHeight / 2, GWindow->_windowWidth / 2)
	//modifyTextComment(std::string("+"));
	std::string aimStr = "";
	aimStr.append(1, 0);
	modifyTextComment(aimStr);
	setBRender(true);

	_textSizeCompensation = textSize / 2;
}

void AimTextUIObj::setAimPositionWithQuat(const glm::quat& curQuat, const glm::quat& targetQuat)
{
	// get current plane aim & camera aim
	glm::mat3 curMat3 = glm::toMat3(curQuat);
	glm::mat3 targetMat3 = glm::toMat3(targetQuat);

	// color to RED if aim direction is back screen space.
	if (glm::dot(curMat3[2], targetMat3[2]) < 0.0f)
		setAimColor(_outAimColor);
	else
		setAimColor(_inAimColor);

	const glm::vec3& planeNoraml = targetMat3[1];
	const glm::vec3& curVec = curMat3[2];
	const glm::vec3& angleCompVec = targetMat3[0];

	glm::vec3 pointOnTargatXZPlane = glm::normalize(curVec - planeNoraml * (glm::dot(curVec, planeNoraml)));
	float radHor = (glm::dot(pointOnTargatXZPlane, angleCompVec));		// radian (Camera X axis ~ plane dirextion(z))

	const glm::vec3& planeNoraml2 = targetMat3[0];
	const glm::vec3& angleCompVec2 = targetMat3[1];

	glm::vec3 pointOnTargatYZPlane = glm::normalize(curVec - planeNoraml2 * (glm::dot(curVec, planeNoraml2)));
	float radVert = (glm::dot(pointOnTargatYZPlane, angleCompVec2));	// radian (Camera Y axis ~ plane dirextion(z))

	CAMERA::Camera* mainCam = *(GCameraManager->GetMainCamera());
	float fov = mainCam->getFov();

	// get aim position to screen with MainCam Fovy & Fovy ratio
	float radVertPos = (GWindow->_windowHeight / 2) * radVert / (glm::radians(fov) / 2) + (GWindow->_windowHeight / 2);
	float radHorPos = (GWindow->_windowWidth / 2) * -radHor / (glm::radians(fov * mainCam->getFovyRatio()) / 2) + (GWindow->_windowWidth / 2);

	// modify aim position with Text Compensation
	_drawElement->first->setPos(glm::vec2(radHorPos - _textSizeCompensation, radVertPos + _textSizeCompensation));

}

void AimTextUIObj::setAimColor(glm::vec3 & color)
{
	_drawElement->first->setColor(color);
}
