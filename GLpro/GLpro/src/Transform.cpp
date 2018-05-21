#include "Transform.h"

Transform::Transform(int entityID, const glm::mat4 &modelMatrix, const glm::mat4 &rotateMatrix, const glm::mat4 &scaleMatrix) 
	: _entityID(entityID), _localModelMatrix(modelMatrix), _localRotateMatrix(_localRotateMatrix), _localScaleMatrix(_localScaleMatrix)
{
	_velocity = glm::vec3();
	_bVelocity = false;
	_parentTransformPtr = nullptr;
	_childTransformPtrList = std::list<Transform*>();
	_bDirty = true;		// update local -> world	:: 없으면 child로 신규 생성시 world update X
}

const glm::mat4 & Transform::getWorldMatRef() const
{
	return _worldTotalMatrix;
}


glm::mat4 Transform::getWorldMat() const
{
	return _worldTotalMatrix;
}

glm::vec3 Transform::getWorldPosVec() const
{
	return _worldTotalMatrix[3];
}

const glm::mat4& Transform::getModelMatrixConstRef() const
{
	return _localModelMatrix;
}

glm::mat4 Transform::getModelMatrix() const
{
	return _localModelMatrix;
}

glm::vec3 Transform::getModelVec() const
{
	glm::vec3 ret = _localModelMatrix[3];
	return ret;
}

const glm::mat4 & Transform::getLocalRotationMatrixConstRef() const
{
	return _localRotateMatrix;
}

glm::mat4 Transform::getLocalRotationMatrix() const
{
	return _localRotateMatrix;
}

const glm::mat4 & Transform::getLocalScaleMatrixConstRef() const
{
	return _localScaleMatrix;
}

glm::mat4 Transform::getLocalScaleMatrix() const
{
	return _localScaleMatrix;
}

glm::quat Transform::getLocalQuarternion() const
{
	return glm::quat_cast(_localRotateMatrix);
}

void Transform::setModelMatrix(const glm::mat4 &localModelMatrix)
{
	_localModelMatrix = localModelMatrix;
}

void Transform::setRotationMatrix(const glm::mat4 &rotateMat)
{
	_localRotateMatrix = rotateMat;
}

void Transform::setScaleMatrix(const glm::mat4 &localScaleMat)
{
	_localScaleMatrix = localScaleMat;
}

void Transform::setModelMatrix(const glm::vec3 &localModelVec)
{
	_localModelMatrix = glm::translate(glm::mat4(), localModelVec);
}

void Transform::setRotationMatrix(const glm::vec3 &rotateVec)
{
	_localRotateMatrix =  glm::toMat4(glm::quat(rotateVec));
}

void Transform::setScaleMatrix(const glm::vec3 &localScaleVec)
{
	_localScaleMatrix = glm::scale(glm::mat4(), localScaleVec);
}

void Transform::setRotationMatrix(const glm::quat &quat)
{
	_localRotateMatrix = glm::toMat4(quat);
}
void Transform::setVMatrixLookat(const glm::vec3 & lookat, const glm::vec3 & up)
{
	glm::vec3 modelVec = _localModelMatrix[3];
	_localRotateMatrix = glm::lookAt(modelVec, modelVec + lookat, up);
	_localRotateMatrix[3][0] = 0;
	_localRotateMatrix[3][1] = 0;
	_localRotateMatrix[3][2] = 0;
}

void Transform::setVMatrixLookat(const glm::vec3 & pos, const glm::vec3 & lookat, const glm::vec3 & up)
{
	_localRotateMatrix = glm::lookAt(pos, pos + lookat, up);
	_localRotateMatrix[3][0] = 0;
	_localRotateMatrix[3][1] = 0;
	_localRotateMatrix[3][2] = 0;
}

void Transform::accModelMatrix(const glm::mat4 &localAccModelMatrix)
{
	_localModelMatrix[3][0] += localAccModelMatrix[3][0];
	_localModelMatrix[3][1] += localAccModelMatrix[3][1];
	_localModelMatrix[3][2] += localAccModelMatrix[3][2];
}

void Transform::accRotationMatrix(const glm::mat4 &localAccRotateMat)
{
	_localRotateMatrix = localAccRotateMat * _localRotateMatrix;
}

void Transform::accScaleMatrix(const glm::mat4 &scaleMat)
{
	_localScaleMatrix[0][0] *= scaleMat[0][0];
	_localScaleMatrix[1][1] *= scaleMat[1][1];
	_localScaleMatrix[2][2] *= scaleMat[2][2];
}

void Transform::accModelMatrix(const glm::vec3 &modelVec)
{
	_localModelMatrix = glm::translate(_localModelMatrix, modelVec);
}

void Transform::accRotationMatrix(const float &degree, glm::vec3 &rotateAxis)
{
	_localRotateMatrix = glm::rotate(_localRotateMatrix, glm::radians(degree), rotateAxis);
}

void Transform::accScaleMatrix(const glm::vec3 &scaleVec)
{
	_localScaleMatrix = glm::scale(_localScaleMatrix, scaleVec);
}

void Transform::accRotationMatrix(const glm::quat &quat)
{
	glm::toMat4(quat) * _localRotateMatrix;
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
	auto it = std::find_if(_childTransformPtrList.begin(), _childTransformPtrList.end(), [id](Transform* &elem) {
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