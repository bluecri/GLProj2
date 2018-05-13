#include "./CameraBuilder.h"

namespace CAMERA
{

	CameraBuilder::CameraBuilder(Transform *transform)
	{
		_camTransform = transform;
		_camProjMat;
		_bProjection = true;

		_speed = 0.1f; // 3.0f == 3 units / second
		_mouseSpeed; // default 0.005f

		_fov = 45.0f;	//default 45.0f degree
		_horizontalFovValue = 4.0f;	//4:3 window ratio
		_verticalFovValue = 3.0f;
		_frontFrustrumValue = 0.1f;	//0.1f
		_backFrustrumValue = 100.0f;	//100.0f
	}

	Camera& CameraBuilder::setFov(float fov, float horizontalFov, float verticalFov)
	{
		_fov = fov;
		_horizontalFovValue = horizontalFov;
		_verticalFovValue = verticalFov;
	}

	Camera& CameraBuilder::setFrustrum(float frontFrustrum, float backFrustrum)
	{
		_frontFrustrumValue = frontFrustrum;
		_backFrustrumValue = backFrustrum;
	}

	Camera& CameraBuilder::setSpeed(int moveSpeed, int mouseSpeed)
	{
		_speed = moveSpeed;
		_mouseSpeed = mouseSpeed;
	}

	Camera& CameraBuilder::setIsProjection(bool bProjection)
	{
		_bProjection = bProjection;
	}

	Camera* CameraBuilder::constructPtr()
	{
		return new Camera(_camTransform, _fov, _speed, _mouseSpeed, _horizontalFovValue, _verticalFovValue, _frontFrustrumValue, _backFrustrumValue);
	}
}