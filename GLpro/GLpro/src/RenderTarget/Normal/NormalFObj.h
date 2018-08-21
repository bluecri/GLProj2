#ifndef __Normal_F_OBJ_H__
#define __Normal_F_OBJ_H__

#include "../../../stdafx.h"
#include "../../../Fobj.h"

#include "../../../SphereOb.h"

#include "../../../VectorPElem.h"


#define FRUSTUM_CULL_BIT			(1 << 0)
#define OCCLUSION_CULL_BIT			(1 << 1)
#define OCCLUSION_CULL_TEST_BIT		(1 << 2)
#define DRAW_COMP_BIT				(3)

#define FRUSTUM_CULL_INVBIT			~(1 << 0)
#define OCCLUSION_CULL_INVBIT		~(1 << 1)
#define OCCLUSION_CULL_TEST_INVBIT	~(1 << 2)

namespace RESOURCE
{
	class Model;
	class Texture;
}

class RigidbodyComponent;

namespace RENDER_TARGET
{
	namespace NORMAL
	{
		class NormalFObj : public FObj, public VectorPElem
		{
		public:
			NormalFObj(const char* modelFileName, bool createEbo, const char* textureFileName, const char* textureType);
			NormalFObj(RESOURCE::Model * model, RESOURCE::Texture * texture);
			~NormalFObj() {	};

			bool isFrustumCulled();
			void onFrustumCull();
			void offFrustumCull();

			bool isOcclusionCulled();
			void onOcclusionCull();
			void offOcclusionCull();
			bool hasTestingOcclusionCull();

			//should call when hasTestingOcclusionCull() is true
			bool getOcclusionCullTestAndReset();
			void setOcclusionCullTest();

			bool getRenderCull();

			void setFrustumRadius(float radius);
			float& getFrustumRadiusRef();

			//void setFrustumPos(glm::vec3& pos);
			void setFrustumPos(RigidbodyComponent * rigidbodyComponent);
			float getNearestDistToPoint(glm::vec3& pos);

			glm::vec3& getFrustumPosRef();
			glm::vec3& getFrustumCompensationPosRef();
			void setFrustumCompensationPos(glm::vec3& compen_pos);

			SphereOb& getSphereObForFrustumRef();

			int		getOctreeElemIndex();
			void	setOctreeElemIdx(int idx);
		public:
			RESOURCE::Model * _model = nullptr;
			RESOURCE::Texture * _texture = nullptr;

		private:
			NormalFObj() {};
			int _renderBit;	// occlusion cull & frustum cull
			int		_octreeElemIdx;					// Octree.h's OctreeElem index.

			glm::vec3	_frustumPosCompensation;
			SphereOb	_sphereObForFrustum;		// use this in frustum (this = posCompen + binded trasform wolrd mat in {DrawElem})
		};
	}
}


#endif