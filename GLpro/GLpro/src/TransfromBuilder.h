#ifndef __TRANSFORM_BUILDER_H__
#define __TRANSFORM_BUILDER_H__

#include "./Transform.h"

class TransformBuilder
{
public:
	/*
	TransformBuilder();

	Transform& setModelMat(const glm::mat4 &modelMat);

	Transform& setModelMat(const glm::vec3 &modelVec);

	Transform& setRotMat(const glm::mat4 &rotateMatrix);

	Transform& setRotMat(const glm::vec3 &angleVec);

	Transform& setScaleMat(const glm::mat4 &scaleMatrix);

	Transform& setScaleMat(const glm::vec3 &scaleVec);

	Transform& addChildTrnasform(Transform *childTransform);

	Transform& addParentTrnasform(Transform *parentTransform);

	Transform& setType(int type);

	Transform* constructPtr();
	*/

private:
	glm::mat4 _modelMatrix;
	glm::mat4 _rotateMatrix;
	glm::mat4 _scaleMatrix;
	std::vector<Transform*> _childTransformPtrList;
	Transform* _parentTransformPtr;
};
#endif
