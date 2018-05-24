#include "./Camera.h"

namespace CAMERA {

	Camera::Camera(std::string name, int type, float fov, float speed, float mouseSpeed,
		float horizontalFovValue, float verticalFovValue, float frontFrustrumValue, float backFrustrumValue)
		: Entity(name, type)
	{
		_speed = speed;
		_mouseSpeed = mouseSpeed;
		_fov = fov;
		_horizontalFovValue = horizontalFovValue;
		_verticalFovValue = verticalFovValue;
		_frontFrustrumValue = frontFrustrumValue;
		_backFrustrumValue = backFrustrumValue;

		updateProjectionMatrix();
	};
	
	void Camera::updateProjectionMatrix()
	{
		_camProjMat = glm::perspective(glm::radians(_fov), _horizontalFovValue / _verticalFovValue, _frontFrustrumValue, _backFrustrumValue);
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
		_viewMatRecentUsed = _rigidbodyComponent->_transform->getWorldMat();
		_vpMatRecentUsed = _camProjMat * _viewMatRecentUsed;
	}

}


