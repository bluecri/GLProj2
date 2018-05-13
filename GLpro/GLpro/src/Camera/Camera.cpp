#include "./Camera.h"

namespace CAMERA {

	Camera::Camera(std::string name, int type, float fov, float speed, float mouseSpeed,
		float horizontalFovValue, float verticalFovValue, float frontFrustrumValue, float backFrustrumValue)
		: Entity(name, type)
	{
		_speed = speed;
		_mouseSpeed = mouseSpeed;
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

}


