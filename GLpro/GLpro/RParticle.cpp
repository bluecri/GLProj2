#include "stdafx.h"
#include "RParticle.h"
#include "src/Camera/Camera.h"
#include "src/Camera/CameraManager.h"
#include "src\Shader\ShaderParticle.h"
#include "src/Resource/Texture.h"
#include "RigidbodyComponent.h"
#include "ParticleFObj.h"
#include "ParticleBuffer.h"
#include "ParticleStruct.h"
#include "ParticleCreateInfo.h"

namespace RENDER
{
	
	RParticle::RParticle(RParticle::TYPE_SHADER * shaderObj)
	{
		_shaderObj = shaderObj;
		_targetCamera = GCameraManager->GetMainCamera();
	}

	std::shared_ptr<RParticle::DrawElement> RParticle::addToDrawList(FObjElem* particleFObj, RigidbodyComponent * rigidbodyComponent)
	{
		ParticleCreateInfo* particleCreateInfo = new ParticleCreateInfo(true);
		particleCreateInfo->init(rigidbodyComponent);

		auto elem = std::shared_ptr<RParticle::DrawElement>(new RParticle::DrawElement(particleFObj, particleCreateInfo), [](auto ptr) {
			delete ptr->first;
			delete ptr->second;
			delete ptr;
		});
		_particleDrawElemContainer.push_back(elem);
		return elem;
	}

	void RParticle::update(CAMERA::Camera ** cam)
	{
		_targetCamera = cam;
	}

	void RParticle::draw(float deltaTime)
	{
		beforeDraw(deltaTime);		//update particle
		CAMERA::Camera* cam = *_targetCamera;

		_shaderObj->bind();

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		for (auto it = _particleDrawElemContainer.begin(); it != _particleDrawElemContainer.end(); )
		{
			RENDER_TARGET::PARTICLE::ParticleFObj* particleFObj = (*it)->first;
			// ParticleCreateInfo* particleCreateInfo = (*it)->second;
			// RParticle에서는 particle 생성시에만 bind된 rigidbody position을 사용한다.

			if (!particleFObj->isRender())
			{
				++it;
				continue;
			}

			particleFObj->_particleBuffer->bind();

			glActiveTexture(GL_TEXTURE3);
			particleFObj->_texture->bind();
			_shaderObj->loadInt(_shaderObj->m_textureID, 3);

			glm::mat4& cameraViewMat = cam->getRecentViewMat();
			glm::mat4& cameraVPMat = cam->getRecentVPMat();
			_shaderObj->loadVector3(_shaderObj->m_cameraRight_worldspace_ID, cameraViewMat[0][0], cameraViewMat[1][0], cameraViewMat[2][0]);
			_shaderObj->loadVector3(_shaderObj->m_cameraUp_worldspace_ID, cameraViewMat[0][1], cameraViewMat[1][1], cameraViewMat[2][1]);
			_shaderObj->loadMatrix4(_shaderObj->m_viewProjMatrixID, cameraVPMat);

			particleFObj->_particleBuffer->render();

			particleFObj->_texture->unbind();
			particleFObj->_particleBuffer->unbind();

			it++;
		}

		_shaderObj->unbind();

		glDisable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);
	}

	void RParticle::chageShader(RParticle::TYPE_SHADER * other)
	{
		_shaderObj = other;
	}

	RParticle::TYPE_SHADER * RParticle::getShader() const
	{
		return _shaderObj;
	}

	/*
	*	particle Fobj에서 빈 particle을 particle info를 통해 particle generate.
	*
	*/
	void RParticle::beforeDraw(float deltaTime)
	{
		glm::vec3 camPosVec = (*_targetCamera)->getRigidbodyComponent()->getWorldPosVec();
		for (auto it = _particleDrawElemContainer.begin(); it != _particleDrawElemContainer.end(); )
		{
			RENDER_TARGET::PARTICLE::ParticleFObj* particleFObj = (*it)->first;
			ParticleCreateInfo* particleCreateInfo = (*it)->second;

			// check delete & remain time
			if (particleFObj->isBDeleted())
			{
				float& remainDelTime = particleFObj->getDeleteRemainTimeRef();
				if (remainDelTime < 0.0f)
				{
					// do delete
					it = _particleDrawElemContainer.erase(it);
					continue;
				}
				remainDelTime -= deltaTime;
			}
			else if (!particleFObj->isRender())
			{
				++it;
				continue;
			}
			else
			{
				// not generate if deleted
				// create new particles [ get new particle & modify info with particleCreateInfo ]
				particleCreateInfo->genNewParticles(particleFObj);
			}

			// update exist particles
			for (auto elem : particleFObj->_particleContainer)
			{
				// cam distance update
				elem->update(deltaTime, camPosVec);
			}

			// sort with distance
			particleFObj->sortContainerByDist();

			// fill ParticleBuffer (pos buffer & color buffer)
			particleFObj->orderFillParticleBuffer();

			++it;
		}

	}
}
