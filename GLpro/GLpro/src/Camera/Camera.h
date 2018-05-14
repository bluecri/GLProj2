#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "../Transform.h"
#include "../Entity.h"

namespace CAMERA 
{
	class Camera : public Entity
	{
	public:
		glm::mat4 _camProjMat;
		bool _bProjection = true;

		float _speed; // 3.0f == 3 units / second
		float _mouseSpeed; // default 0.005f

		float _fov;	//default 45.0f degree
		float _horizontalFovValue;	//4:3 window ratio
		float _verticalFovValue;
		float _frontFrustrumValue;	//0.1f
		float _backFrustrumValue;	//100.0f

		Camera(std::string name, int type, float fov, float speed = 0.1f, float mouseSpeed = 0.005f,
			float horizontalFovValue = 4.0f, float verticalFovValue = 3.0f, float frontFrustrumValue = 0.1f, float backFrustrumValue = 100.0f);

		void updateProjectionMatrix();

		glm::mat4 getCamProjMat() const {
			return _camProjMat;
		}

		const glm::mat4& getCamProjMatRef() const {
			return _camProjMat;
		}

		void SetWithLookAt(const glm::vec3 &lookat, const glm::vec3 &up)
		{
			_rigidbodyComponent->_transform->setMVMatrixLookat(lookat, up);
		}

		virtual ~Camera() {}

		glm::mat4& getRecentVPMat();
		glm::mat4& getRecentViewMat();
		void updateRecentVPAndViewMat();

	public:
		glm::mat4 _vpMatRecentUsed;
		glm::mat4 _viewMatRecentUsed;
	};

}

#endif