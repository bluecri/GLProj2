#include "NormalFObj.h"

#include "../../Resource/Texture.h"
#include "../../Resource/Model.h"

#include "../../Resource/ModelManager.h"
#include "../../Resource/TextureManager.h"

#include "../../../RigidbodyComponent.h"
namespace RENDER_TARGET
{
	namespace NORMAL
	{
		NormalFObj::NormalFObj(const char * modelFileName, bool createEbo, const char * textureFileName, const char * textureType)
			: FObj()
		{
			_model = GModelManager->getModelWithFileName(modelFileName, createEbo);
			_texture = GTextureManager->getTextureWithFileName(textureFileName, textureType);

			_frustumRadius = 1.0f;
			_frustumPosCompensation = glm::vec3();
			_frustumPos = glm::vec3();
		}
		NormalFObj::NormalFObj(RESOURCE::Model * model, RESOURCE::Texture * texture)
			: FObj()
		{
			_model = model;
			_texture = texture;

			_frustumRadius = 1.0f;
			_frustumPosCompensation = glm::vec3();
			_frustumPos = glm::vec3();
		}
		bool NormalFObj::isFrustumCulled()
		{
			return _renderBit & FRUSTUM_CULL_BIT;
		}
		void NormalFObj::onFrustumCull()
		{
			_renderBit |= FRUSTUM_CULL_BIT;
		}
		void NormalFObj::offFrustumCull()
		{
			_renderBit &= FRUSTUM_CULL_INVBIT;
		}
		bool NormalFObj::isOcclusionCulled()
		{
			return _renderBit & OCCLUSION_CULL_BIT;
		}
		void NormalFObj::onOcclusionCull()
		{
			_renderBit |= OCCLUSION_CULL_BIT;
		}
		void NormalFObj::offOcclusionCull()
		{
			_renderBit &= OCCLUSION_CULL_INVBIT;
		}
		bool NormalFObj::hasTestingOcclusionCull()
		{
			return _renderBit & OCCLUSION_CULL_TEST_BIT;
		}

		//should call when hasTestingOcclusionCull() is true

		bool NormalFObj::getOcclusionCullTestAndReset()
		{
			_renderBit &= OCCLUSION_CULL_TEST_BIT;	// reset test valid
													//todo : return cull result
													//	void onOcclusionCull() or offOcclusionCull
			return false;
		}
		void NormalFObj::setOcclusionCullTest()
		{
			_renderBit |= OCCLUSION_CULL_TEST_BIT;	// set test valid
		}
		bool NormalFObj::getRenderCull()
		{
			if ((_renderBit | DRAW_COMP_BIT) != 0)
				return false;
			return true;
		}
		void NormalFObj::setFrustumRadius(float radius)
		{
			_frustumRadius = radius;
		}
		float NormalFObj::getFrustumRadius()
		{
			return _frustumRadius;
		}
		void NormalFObj::setFrustumPos(glm::vec3 & pos)
		{
			_frustumPos = pos;
		}
		void NormalFObj::setFrustumPos(RigidbodyComponent * rigidbodyComponent)
		{
			_frustumPos = _frustumPosCompensation + rigidbodyComponent->getWorldPosVec();
		}
		glm::vec3 & NormalFObj::getFrustumPosRef()
		{
			return _frustumPos;
		}
		void NormalFObj::setFrustumCompensationPos(glm::vec3 & compen_pos)
		{
			_frustumPosCompensation = compen_pos;
		}
		glm::vec3 & NormalFObj::getFrustumCompensationPosRef()
		{
			return _frustumPosCompensation;
		}
	}
}


