#ifndef __CAMERA_BUILDER_H__
#define __CAMERA_BUILDER_H__

#include "./Camera.h"
#include "../TransfromBuilder.h"

namespace CAMERA
{
	class CameraBuilder
	{
	public:
		CameraBuilder(Transform *transform);

		Camera* constructPtr();
		Camera& setFov(float fov, float horizontalFov, float verticalFov);
		Camera& setFrustrum(float frontFrustrum, float backFrustrum);
		Camera& setSpeed(int moveSpeed, int mouseSpeed);
		Camera& setIsProjection(bool isProjection);

	private:
		Transform * _camTransform;
		glm::mat4 _camProjMat;
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