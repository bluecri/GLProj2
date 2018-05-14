#ifndef __CAMERA_BUILDER_H__
#define __CAMERA_BUILDER_H__

#include <string>

namespace CAMERA { class Camera; };


namespace CAMERA
{
	class CameraBuilder
	{
	public:
		CameraBuilder(std::string& name, int type);

		Camera* constructPtr();
		CameraBuilder& setFov(float fov, float horizontalFov, float verticalFov);
		CameraBuilder& setFrustrum(float frontFrustrum, float backFrustrum);
		CameraBuilder& setSpeed(float moveSpeed, float mouseSpeed);
		CameraBuilder& setIsProjection(bool isProjection);

	private:
		std::string& _name;
		int _type;
		bool _bProjection = true;

		float _speed; // 3.0f == 3 units / second
		float _mouseSpeed; // default 0.005f

		float _fov;	//default 45.0f degree
		float _horizontalFovValue;	//4:3 window ratio
		float _verticalFovValue;
		float _frontFrustrumValue;	//0.1f
		float _backFrustrumValue;	//100.0f
	};
}
#endif