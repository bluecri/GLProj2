#ifndef __Normal_F_OBJ_H__
#define __Normal_F_OBJ_H__

#include "../../../stdafx.h"
#include "../../../Fobj.h"

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
		class NormalFObj : public FObj
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
			float getFrustumRadius();

			void setFrustumPos(glm::vec3& pos);
			void setFrustumPos(RigidbodyComponent * rigidbodyComponent);

			glm::vec3& getFrustumPosRef();
			void setFrustumCompensationPos(glm::vec3& compen_pos);
			glm::vec3& getFrustumCompensationPosRef();

		public:
			RESOURCE::Model * _model = nullptr;
			RESOURCE::Texture * _texture = nullptr;

		private:
			NormalFObj() {};
			int _renderBit;	// occlusion cull & frustum cull

			glm::vec3	_frustumPosCompensation;
			float		_frustumRadius;
			glm::vec3	_frustumPos;		// use this in frustum (this = posCompen + binded trasform wolrd mat in {DrawElem})
		};
	}
}


#endif