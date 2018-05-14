#include "./TransfromBuilder.h"
/*
TransformBuilder::TransformBuilder() {
	_modelMatrix = glm::mat4();
	_rotateMatrix = glm::mat4();
	_scaleMatrix = glm::mat4();
	_parentTransformPtr = NULL;
	_childTransformPtrList = std::vector<Transform*>();
}

Transform& TransformBuilder::setModelMat(const glm::mat4 &modelMat) {
	_modelMatrix = modelMat;
}

Transform& TransformBuilder::setModelMat(const glm::vec3 &modelVec) {
	_modelMatrix = glm::translate(glm::mat4(), modelVec);
}

Transform& TransformBuilder::setRotMat(const glm::mat4 &rotateMatrix) {
	_rotateMatrix = rotateMatrix;
}

Transform& TransformBuilder::setRotMat(const glm::vec3 &angleVec) {
	_rotateMatrix = glm::toMat4(glm::quat(angleVec));
}

Transform& TransformBuilder::setScaleMat(const glm::mat4 &scaleMatrix) {
	_scaleMatrix = scaleMatrix;
}

Transform& TransformBuilder::setScaleMat(const glm::vec3 &scaleVec) {
	_scaleMatrix = glm::scale(glm::mat4(), scaleVec);
}

/*
Transform& TransformBuilder::addChildTrnasform(Transform *childTransform) {
	_childTransformPtrList.push_back(childTransform);
	//childTransform->_parentTransformPtr = this;
}

Transform& TransformBuilder::addParentTrnasform(Transform *parentTransform) {
	_parentTransformPtr = parentTransform;
	//parentTransform->_childTransformPtrVec.push_back(this);
}

Transform* TransformBuilder::constructPtr() {
	//Transform * ret = new Transform(_modelMatrix, _rotateMatrix, _scaleMatrix, _parentTransformPtr, _childTransformPtrList);
	Transform * ret = new Transform(_modelMatrix, _rotateMatrix, _scaleMatrix);
	ret->attachParentTransform(_parentTransformPtr);	//attach parent

	std::for_each(_childTransformPtrList.begin(), _childTransformPtrList.end(), [=](Transform* childPtr) {
		ret->attachChildTransform(childPtr);
	});
}

*/