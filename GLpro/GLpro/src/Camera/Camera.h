#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "../Entity.h"

namespace CAMERA 
{
	/*
	*	camera class
	*	�⺻ quaternion�� ����ϸ�, view matrix�� draw ���� worldMat->lookat���� ��ȯ�����Ͽ� ���
	*/
	class Camera : public Entity
	{
	public:
		Camera(std::string name, ENUM_ENTITY_TYPE type, float fov, float speed = 0.1f, float keySpeed = 0.5f, float mouseSpeed = 0.005f,
			float horizontalFovValue = 4.0f, float verticalFovValue = 3.0f, float frontFrustrumValue = 0.1f, float backFrustrumValue = 100.0f);
		virtual ~Camera() {}

		void SetWithLookAt(const glm::vec3 &pos, const glm::vec3 &lookat, const glm::vec3 &up);
		void SetWithLookAt(const glm::vec3 &lookat, const glm::vec3 &up);

		void camAccQuaternionYaw(const float &degree);
		void camAccQuaternionPitch(const float &degree);
		void camAccQuaternionRoll(const float &degree);

		glm::mat4& getRecentVPMat();
		glm::mat4& getRecentViewMat();
		void updateRecentVPAndViewMat();

		int sphereInFrustum(glm::vec3 & center, float & radius);

		glm::mat4 getCamProjMat() const;
		const glm::mat4& getCamProjMatRef() const;
		void updateProjectionMatrix();

		float getFov();
		float getFovyRatio();

	private:
		// Entity��(��) ���� ��ӵ�
		virtual void setBRender(bool bRender) override;
		virtual void setCollisionTest(bool bCollision) override;
		virtual void doJobWithBeDeleted() override;
		virtual void logicUpdate(float deltaTime, float acc) override;

	private:
		float		_speed;				// 3.0f == 3 units / second
		float		_mouseSpeed;			// default 0.005f
		float		_keySpeed;				// 0.5

		glm::mat4	_camProjMat;
		bool		_bProjection = true;
		float		_fov;					//default 45.0f degree
		float		_fovyRatio;

		float		_horizontalFovValue;	//4:3 window ratio
		float		_verticalFovValue;
		float		_frontFrustrumValue;	//0.1f
		float		_backFrustrumValue;	//100.0f

		glm::mat4	_vpMatRecentUsed;
		glm::mat4	_viewMatRecentUsed;
		glm::vec4	_frustum[6];
		glm::mat4	_tempClip;
	};
}

#endif