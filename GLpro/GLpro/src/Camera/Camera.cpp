#include "./Camera.h"

namespace CAMERA {

	;
	
	Camera::Camera(std::string name, int type, float fov, float speed, float keySpeed, float mouseSpeed, float horizontalFovValue, float verticalFovValue, float frontFrustrumValue, float backFrustrumValue)
		: Entity(name, nullptr, type)
	{
		_speed = speed;
		_mouseSpeed = mouseSpeed;
		_keySpeed = keySpeed;
		_fov = fov;
		_horizontalFovValue = horizontalFovValue;
		_verticalFovValue = verticalFovValue;
		_fovyRatio = _horizontalFovValue / _verticalFovValue;
		_frontFrustrumValue = frontFrustrumValue;
		_backFrustrumValue = backFrustrumValue;

		updateProjectionMatrix();
	}

	void Camera::updateProjectionMatrix()
	{
		_fovyRatio = _horizontalFovValue / _verticalFovValue;
		_camProjMat = glm::perspective(glm::radians(_fov), _fovyRatio, _frontFrustrumValue, _backFrustrumValue);
	}

	float Camera::getFov() {
		return _fov;
	}

	float Camera::getFovyRatio()
	{
		return _fovyRatio;
	}

	void Camera::setBRender(bool bRender)
	{
	}

	void Camera::setCollisionTest(bool bCollision)
	{
	}

	void Camera::doJobWithBeDeleted()
	{
		printf_s("camera is deleted\n");
	}

	void Camera::logicUpdate(float deltaTime, float acc)
	{
	}

	glm::mat4 Camera::getCamProjMat() const {
		return _camProjMat;
	}

	const glm::mat4 & Camera::getCamProjMatRef() const {
		return _camProjMat;
	}

	void Camera::SetWithLookAt(const glm::vec3 & pos, const glm::vec3 & lookat, const glm::vec3 & up)
	{
		_rigidbodyComponent->_transform->setVMatrixLookat(pos, lookat, up);
	}

	void Camera::SetWithLookAt(const glm::vec3 & lookat, const glm::vec3 & up)
	{
		_rigidbodyComponent->_transform->setVMatrixLookat(lookat, up);
	}

	void Camera::camAccQuaternionYaw(const float & degree)
	{
		_rigidbodyComponent->_transform->accQuaternionYaw(degree * _mouseSpeed);
	}

	void Camera::camAccQuaternionPitch(const float & degree)
	{
		_rigidbodyComponent->_transform->accQuaternionPitch(degree * _mouseSpeed);
	}

	void Camera::camAccQuaternionRoll(const float & degree)
	{
		_rigidbodyComponent->_transform->accQuaternionRoll(degree * _keySpeed);
	}

	glm::mat4 & Camera::getRecentVPMat()
	{
		return _vpMatRecentUsed;
	}

	glm::mat4 & Camera::getRecentViewMat()
	{
		return _viewMatRecentUsed;
	}

	void Camera::updateRecentVPAndViewMat()
	{
		const glm::mat4& camWorldMat = _rigidbodyComponent->_transform->getWorldMatRef();

		glm::vec3 tempEye = camWorldMat[3];
		glm::vec3 fowardVec = camWorldMat[2];
		glm::vec3 upVec = camWorldMat[1];

		_viewMatRecentUsed = glm::lookAt(tempEye, fowardVec + tempEye, upVec);
		_vpMatRecentUsed = _camProjMat * _viewMatRecentUsed;
	}

}


