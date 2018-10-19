#include "./Camera.h"

namespace CAMERA {
	
	Camera::Camera(std::string name, ENUM_ENTITY_TYPE type, float fov, float speed, float keySpeed, float mouseSpeed, float horizontalFovValue, float verticalFovValue, float frontFrustrumValue, float backFrustrumValue)
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
		_rigidbodyComponent->setVMatrixLookat(pos, lookat, up);
	}

	void Camera::SetWithLookAt(const glm::vec3 & lookat, const glm::vec3 & up)
	{
		_rigidbodyComponent->setVMatrixLookat(lookat, up);
	}

	void Camera::camAccQuaternionYaw(const float & degree)
	{
		_rigidbodyComponent->accQuaternionYaw(degree * _mouseSpeed);
	}

	void Camera::camAccQuaternionPitch(const float & degree)
	{
		_rigidbodyComponent->accQuaternionPitch(degree * _mouseSpeed);
	}

	void Camera::camAccQuaternionRoll(const float & degree)
	{
		_rigidbodyComponent->accQuaternionRoll(degree * _keySpeed);
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
		const glm::mat4& camWorldMat = _rigidbodyComponent->getWorldMatRef();

		glm::vec3 tempEye = camWorldMat[3];
		glm::vec3 fowardVec = camWorldMat[2];
		glm::vec3 upVec = camWorldMat[1];

		_viewMatRecentUsed = glm::lookAt(tempEye, fowardVec + tempEye, upVec);
		_vpMatRecentUsed = _camProjMat * _viewMatRecentUsed;

		// update frustum 6 plane
		// reference : http://www.racer.nl/reference/vfc_markmorley.htm
		_tempClip = _viewMatRecentUsed * _camProjMat;

		/*
		for (int i = 0; i < 3; i++)
		{
			for (int k = -1; k < 2; k+= 2)
			{
				for (int p = 0; p < 4; p++)
				{
					_frustrum[i * 2 + k][p] = _tempClip[3 + 4 * p] + k * _tempClip[4 * p + i];
				}
			}
		}
		*/
		_tempClip = glm::transpose(_tempClip);

		_frustum[0] = _tempClip[3] + _tempClip[0];		// left
		_frustum[1] = _tempClip[3] - _tempClip[0];		// right
		_frustum[2] = _tempClip[3] + _tempClip[1];		// bottom
		_frustum[3] = _tempClip[3] - _tempClip[1];		// top
		_frustum[4] = _tempClip[3] + _tempClip[2];		// near
		_frustum[5] = _tempClip[3] - _tempClip[2];		// far

		//for (int i = 0; i < 6; i++)
			//_frustum[i] = glm::normalize(_frustum[i]);
	}

	int Camera::sphereInFrustum(glm::vec3& center, float& radius2)
	{
		int cnt = 0;
		float dist;
		for (int i = 0; i < 6; i++)
		{
			dist = _frustum[i][0] * center.x + _frustum[i][1] * center.y + _frustum[i][2] * center.z + _frustum[i][3];
			
			// out plane
			if (dist <= -radius2)
				return -1;

			// completely in plane (not partial)
			if(dist > radius2)
				cnt++;
		}
		if (cnt == 6)
			return 1;

		return 0;
	}
}
