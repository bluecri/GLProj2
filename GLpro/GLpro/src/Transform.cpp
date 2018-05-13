#include "Transform.h"

Transform::Transform(int entityID, const glm::mat4 &modelMatrix, const glm::mat4 &rotateMatrix, const glm::mat4 &scaleMatrix) 
	: _entityID(entityID), _modelMatrix(modelMatrix), _rotateMatrix(_rotateMatrix), _scaleMatrix(_scaleMatrix)
{
	_parentTransformPtr = nullptr;
	_childTransformPtrList = std::list<Transform*>();
}

const glm::mat4& Transform::getModelMatrixConstRef() const
{
	return _modelMatrix;
}

glm::mat4 Transform::getModelMatrix() const
{
	return _modelMatrix;
}

glm::vec3 Transform::getModelVec() const
{
	glm::vec3 ret = _modelMatrix[3];
	return ret;
}

const glm::mat4 & Transform::getRotationMatrixConstRef() const
{
	return _rotateMatrix;
}

glm::mat4 Transform::getRotationMatrix() const
{
	return _rotateMatrix;
}

const glm::mat4 & Transform::getScaleMatrixConstRef() const
{
	return _scaleMatrix;
}

glm::mat4 Transform::getScaleMatrix() const
{
	return _scaleMatrix;
}

glm::quat Transform::getQuarternion() const
{
	return glm::quat_cast(_rotateMatrix);
}

void Transform::setModelMatrix(const glm::mat4 &modelMat)
{
	_modelMatrix = modelMat;
}

void Transform::setRotationMatrix(const glm::mat4 &rotateMat)
{
	_rotateMatrix = rotateMat;
}

void Transform::setScaleMatrix(const glm::mat4 &scaleMat)
{
	_scaleMatrix = scaleMat;
}

void Transform::setModelMatrix(const glm::vec3 &modelVec)
{
	_modelMatrix = glm::translate(glm::mat4(), modelVec);
}

void Transform::setRotationMatrix(const glm::vec3 &rotateVec)
{
	_rotateMatrix =  glm::toMat4(glm::quat(rotateVec));
}

void Transform::setScaleMatrix(const glm::vec3 &scaleVec)
{
	_scaleMatrix = glm::scale(glm::mat4(), scaleVec);
}

void Transform::setRotationMatrix(const glm::quat &quat)
{
	_rotateMatrix = glm::toMat4(quat);
}

void Transform::setMVMatrixLookat(const glm::vec3 & lookat, const glm::vec3 & up)
{
	glm::vec3 modelVec = _modelMatrix[3];
	_rotateMatrix = glm::lookAt(modelVec, modelVec + lookat, up);
	_rotateMatrix[3][0] = 0;
	_rotateMatrix[3][1] = 0;
	_rotateMatrix[3][2] = 0;
}

void Transform::accModelMatrix(const glm::mat4 &modelMat)
{
	_modelMatrix[3][0] += modelMat[3][0];
	_modelMatrix[3][1] += modelMat[3][1];
	_modelMatrix[3][2] += modelMat[3][2];
}

void Transform::accRotationMatrix(const glm::mat4 &rotateMat)
{
	_rotateMatrix = rotateMat * _rotateMatrix;
}

void Transform::accScaleMatrix(const glm::mat4 &scaleMat)
{
	_scaleMatrix[0][0] *= scaleMat[0][0];
	_scaleMatrix[1][1] *= scaleMat[1][1];
	_scaleMatrix[2][2] *= scaleMat[2][2];
}

void Transform::accModelMatrix(const glm::vec3 &modelVec)
{
	_modelMatrix = glm::translate(_modelMatrix, modelVec);
}

void Transform::accRotationMatrix(const float &degree, glm::vec3 &rotateAxis)
{
	_rotateMatrix = glm::rotate(_rotateMatrix, glm::radians(degree), rotateAxis);
}

void Transform::accScaleMatrix(const glm::vec3 &scaleVec)
{
	_scaleMatrix = glm::scale(_scaleMatrix, scaleVec);
}

void Transform::accRotationMatrix(const glm::quat &quat)
{
	glm::toMat4(quat) * _rotateMatrix;
}

Transform * Transform::getParentTransformPtr()
{
	return _parentTransformPtr;
}

Transform * Transform::getChildTransformWithID(int id)
{
	for (auto elem : _childTransformPtrList)
	{
		if (elem->_entityID == id)
		{
			return elem;
		}
	}
	return nullptr;

}

Transform * Transform::detachParentTransform()
{
	Transform* ret = _parentTransformPtr;
	ret->detachChildTransformWithID(_entityID);
	return ret;
}

Transform * Transform::detachChildTransformWithID(int id)
{
	auto it = std::find_if(_childTransformPtrList.begin(), _childTransformPtrList.end(), [id](const Transform* &elem) {
		return elem->GetEntityID() == id;
	});

	if (it != _childTransformPtrList.end())
	{
		auto ret = (*it);
		
		(*it)->_parentTransformPtr = nullptr;
		_childTransformPtrList.erase(it);

		return ret;
	}
	
	return nullptr;
}


void Transform::attachParentTransform(Transform * parentTransform)
{
	parentTransform->attachChildTransform(this);
	return;
}

void Transform::attachChildTransform(Transform * childTransform)
{
	_childTransformPtrList.push_back(childTransform);
	childTransform->_parentTransformPtr = this;
	return;
}