#ifndef __SKYBOX_VERTEX_GROUP__H__
#define __SKYBOX_VERTEX_GROUP__H__

namespace RESOURCE {
	class Model;
}
/*
skybox vertex group : has distance and ModelWithNoIndiceNoFile.
*/
namespace RENDER_TARGET
{
	namespace SKYBOX
	{
		class SkyboxModel
		{
		public:
			SkyboxModel(float skybox_distance = 40.0f);
			virtual ~SkyboxModel();

			void bind();
			void unbind();

		public:
			RESOURCE::Model *_skbModel;
			float _skybox_distance;

		private:
			void makeModelWithDistance();
		};
	}
}
#endif