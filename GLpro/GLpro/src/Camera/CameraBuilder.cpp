#include "./CameraBuilder.h"
#include "./Camera.h"

namespace CAMERA
{
	CameraBuilder& CameraBuilder::setFov(float fov, float horizontalFov, float verticalFov)
	{
		_fov = fov;
		_horizontalFovValue = horizontalFov;
		_verticalFovValue = verticalFov;

		return *this;
	}

	CameraBuilder& CameraBuilder::setFrustrum(float frontFrustrum, float backFrustrum)
	{
		_frontFrustrumValue = frontFrustrum;
		_backFrustrumValue = backFrustrum;
		return *this;
	}

	CameraBuilder& CameraBuilder::setSpeed(float moveSpeed, float mouseSpeed)
	{
		_speed = moveSpeed;
		_mouseSpeed = mouseSpeed;
		return *this;
	}

	CameraBuilder& CameraBuilder::setIsProjection(bool bProjection)
	{
		_bProjection = bProjection;
		return *this;
	}

	CameraBuilder::CameraBuilder(std::string & name) : _name(name)
	{
		//_type = type;

		_bProjection = true;

		_speed = 0.1f; // 3.0f == 3 units / second
		_mouseSpeed = 0.02f; // default 0.005f
		_keySpeed = 0.8f;

		_fov = 45.0f;	//default 45.0f degree
		_horizontalFovValue = 4.0f;	//4:3 window ratio
		_verticalFovValue = 3.0f;
		_frontFrustrumValue = 0.1f;	//0.1f
		_backFrustrumValue = 200.0f;	//150.0f
	}

	Camera* CameraBuilder::constructPtr()
	{
		return new Camera(_name, ENUM_ENTITY_CAMERA, _fov, _speed, _keySpeed, _mouseSpeed, _horizontalFovValue, _verticalFovValue, _frontFrustrumValue, _backFrustrumValue);
	}
}