#include "stdafx.h"
#include "RParticle.h"
#include "src/Camera/Camera.h"
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
	}

	std::shared_ptr<RParticle::DrawElement> RParticle::addToDrawList(FObjElem* particleFObj, RigidbodyComponent * rigidbodyComponent)
	{
		ParticleCreateInfo* particleCreateInfo = new ParticleCreateInfo();
		particleCreateInfo->_bindedTransform = rigidbodyComponent->_transform;

		auto elem = std::make_shared<DrawElement>(particleFObj, particleCreateInfo);
		_particleDrawElemContainer.push_back(elem);
		return elem;
	}

	void RParticle::update(CAMERA::Camera * cam)
	{
		_targetCamera = cam;
	}

	void RParticle::draw(float deltaTime)
	{
		beforeDraw(deltaTime);		//update particle

		_shaderObj->bind();
		for (auto it = _particleDrawElemContainer.begin(); it != _particleDrawElemContainer.end(); )
		{
			RENDER_TARGET::PARTICLE::ParticleFObj* particleFObj = (*it)->first;
			// ParticleCreateInfo* particleCreateInfo = (*it)->second;
			// RParticle에서는 particle 생성시에만 bind된 rigidbody position을 사용한다.

			particleFObj->_particleBuffer->bind();

			glActiveTexture(GL_TEXTURE3);
			particleFObj->_texture->bind();
			_shaderObj->loadInt(_shaderObj->m_textureID, 3);

			glm::mat4& cameraViewMat = _targetCamera->getRecentViewMat();
			glm::mat4& cameraVPMat = _targetCamera->getRecentVPMat();
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

	void RParticle::destructor(std::shared_ptr<DrawElement> shared)
	{
		DrawElement* ptr = shared.get();
		for (auto it = _particleDrawElemContainer.begin(); it != _particleDrawElemContainer.end();)
		{
			if ((*it).get() == ptr)
			{
				delete it->get()->second;		//remove particleCreateInfo
				_particleDrawElemContainer.erase(it);
				return;
			}
			++it;
		}
	}

	void RParticle::beforeDraw(float deltaTime)
	{
		glm::vec3 camPosVec = _targetCamera->_rigidbodyComponent->_transform->getWorldPosVec();
		for (auto it = _particleDrawElemContainer.begin(); it != _particleDrawElemContainer.end(); )
		{
			RENDER_TARGET::PARTICLE::ParticleFObj* particleFObj = (*it)->first;
			ParticleCreateInfo* particleCreateInfo = (*it)->second;

			// update exist particles
			for (auto elem : particleFObj->_particleContainer)
			{
				// campos
				elem->update(deltaTime, camPosVec);
			}
			// create new particles [ get new particle & modify info with particleCreateInfo ]
			particleCreateInfo->genNewParticles(particleFObj);

			// sort with distance

			particleFObj->sortContainerByDist();

			// fill ParticleBuffer (pos buffer & color buffer)
			particleFObj->orderFillParticleBuffer();
		}

	}
}
