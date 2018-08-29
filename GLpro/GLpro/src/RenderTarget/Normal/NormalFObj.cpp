#include "NormalFObj.h"

#include "../../Resource/Texture.h"
#include "../../Resource/Model.h"

#include "../../Resource/ModelManager.h"
#include "../../Resource/TextureManager.h"

#include "../../../RigidbodyComponent.h"

#include "../../../CollisionFuncStatic.h"

namespace RENDER_TARGET
{
	namespace NORMAL
	{
		NormalFObj::NormalFObj(const char * modelFileName, bool createEbo, const char * textureFileName, const char * textureType)
			: FObj(), VectorPElem()
		{
			_model = GModelManager->getModelWithFileName(modelFileName, createEbo);
			_texture = GTextureManager->getTextureWithFileName(textureFileName, textureType);

			_frustumPosCompensation = glm::vec3();
		}
		NormalFObj::NormalFObj(RESOURCE::Model * model, RESOURCE::Texture * texture)
			: FObj(), VectorPElem()
		{
			_model = model;
			_texture = texture;

			_frustumPosCompensation = glm::vec3();
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
			_sphereObForFrustum.updateSphereOb(radius);
		}
		float NormalFObj::getFrustumRadius()
		{
			return _sphereObForFrustum.getRadius();
		}
		/*
		void NormalFObj::setFrustumPos(glm::vec3& pos)
		{
			_sphereObForFrustum.updateSphereOb(pos);
		}
		*/
		void NormalFObj::setFrustumPos(RigidbodyComponent * rigidbodyComponent)
		{
			if (rigidbodyComponent->isDirtyForRender())
			{
				_sphereObForFrustum.updateSphereOb(_frustumPosCompensation + rigidbodyComponent->getWorldPosVec());
			}
		}
		float NormalFObj::getNearestDistToPoint(glm::vec3 & pos)
		{
			float retDist;
			CollisionFuncStatic::staticCheck_Dist_POINT_SPHERE(pos, _sphereObForFrustum, retDist);
			return retDist;
		}
		const glm::vec3 & NormalFObj::getFrustumPosConstRef() const
		{
			return _sphereObForFrustum.getCenterConstRef();
		}
		void NormalFObj::setFrustumCompensationPos(const glm::vec3 & compen_pos)
		{
			_frustumPosCompensation = compen_pos;
		}
		SphereOb & NormalFObj::getSphereObForFrustumRef()
		{
			return _sphereObForFrustum;
		}
		int NormalFObj::getOctreeElemIndex() const
		{
			return _octreeElemIdx;
		}
		void NormalFObj::setOctreeElemIdx(int idx)
		{
			_octreeElemIdx = idx;
		}
		glm::vec3 & NormalFObj::getFrustumCompensationPosRef()
		{
			return _frustumPosCompensation;
		}
	}
}


