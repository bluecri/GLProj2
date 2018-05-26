#include "Transform.h"

Transform::Transform(int entityID, const glm::mat4 &modelMatrix, const glm::mat4 &rotateMatrix, const glm::mat4 &scaleMatrix) 
	: _entityID(entityID), _localModelMatrix(modelMatrix), _localRotateMatrix(rotateMatrix), _localScaleMatrix(scaleMatrix)
{
	_velocity = glm::vec3();
	_bMove = true;				
	_bVelocity = false;			// bVelocity == use original velocity for move
	_parentTransformPtr = nullptr;
	_childTransformPtrList = std::list<Transform*>();
	_bDirty = true;		// update local -> world	:: 없으면 child로 신규 생성시 world update X
	_maxZSpeed = 3.0f;	// 100 velocity
	_mass = 1.0f;
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
	for(int i=0; i<3; i++)
		_localModelMatrix[3][i] += modelVec[i];
}

void Transform::translateModelMatrix(const glm::vec3 &modelVec)
{
	_localModelMatrix = glm::translate(_localModelMatrix, modelVec);
}

void Transform::setVelocity(const glm::vec3 & velocity)
{
	_velocity = velocity;
}

glm::vec3 Transform::getVelocity() const
{
	return _velocity;
}

glm::vec3 & Transform::getVelocityRef()
{
	return _velocity;
}

void Transform::speedAdd(float add)
{
	_speed = max(-_maxZSpeed, min(_maxZSpeed, _speed + add));
}

void Transform::speedSet(float speed)
{
	_speed = max(-_maxZSpeed, min(speed, speed));
}

float Transform::getSpeed() {
	return _speed;
}

float Transform::getMaxSpeed() {
	return _maxZSpeed;
}

void Transform::setMaxSpeed(float maxSpeed)
{
	_maxZSpeed = maxSpeed;
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

void Transform::resetDirty()
{
	_bDirty = false;
}

bool Transform::isDirty()
{
	return _bDirty;
}

void Transform::setDirty()
{
	_bDirty = true;
}

void Transform::setMove(bool bMove)
{
	_bMove = bMove;
}

void Transform::setMass(float mass)
{
	_mass = mass;
}

float Transform::getMass() {
	return _mass;
}

void Transform::updateWorldMatrix(float deltaTime)
{
	if (_parentTransformPtr != nullptr)		// skip if child (will be visited by tree traversal.
	{
		return;
	}

	if (!_bMove)
	{
		for (auto childTransformPtr : _childTransformPtrList)
		{
			childTransformPtr->updateWIthNoDirtyParent(deltaTime);
		}
		return;
	}

	if (_bDirty)
	{
		updateLocalWithVelocityOrSpeed(deltaTime);
		_worldTotalMatrix = (_localModelMatrix * _localRotateMatrix * _localScaleMatrix);
		for (auto childTransformPtr : _childTransformPtrList)
		{
			childTransformPtr->updateWithDirtyParent(deltaTime, _worldTotalMatrix);
		}
		return;
	}

	if (bUpdateLocalWithVelocityOrSpeed(deltaTime)) {
		_bDirty = true;
		_worldTotalMatrix = (_localModelMatrix * _localRotateMatrix * _localScaleMatrix);
		for (auto childTransformPtr : _childTransformPtrList)
		{
			childTransformPtr->updateWithDirtyParent(deltaTime, _worldTotalMatrix);
		}
		return;
	}

	for (auto childTransformPtr : _childTransformPtrList)
	{
		childTransformPtr->updateWIthNoDirtyParent(deltaTime);
	}

}

//for debug

void Transform::printLocalModel()
{
	std::cout << "model vec" << std::endl;
	for (int i = 0; i < 3; i++)
		std::cout << _localModelMatrix[3][i] << ", ";
	std::cout << std::endl;
}

void Transform::printLocalRotMat()
{
	std::cout << "rotation mat" << std::endl;
	for (int i = 0; i < 4; i++)
	{
		for (int k = 0; k < 4; k++)
		{
			std::cout << _localRotateMatrix[i][k] << ", ";
		}
		std::cout << std::endl;
	}
}

void Transform::printWorldMat()
{
	std::cout << "world mat" << std::endl;
	for (int i = 0; i < 4; i++)
	{
		for (int k = 0; k < 4; k++)
		{
			std::cout << _worldTotalMatrix[i][k] << ", ";
		}
		std::cout << std::endl;
	}
}

// if velocity is not 0 -> return true
bool Transform::bUpdateLocalWithVelocityOrSpeed(float deltaTime)
{
	if (_bVelocity)
	{
		for (int i = 0; i < 3; i++)
		{
			_localModelMatrix[3][i] += _velocity[i] * deltaTime;
		}
		for (int i = 0; i < 3; i++)
		{
			if (_velocity[i] > glm::epsilon<float>())
			{
				return true;
			}
		}
		return false;
	}

	// use speed
	_velocity = _localRotateMatrix[2] * _speed;
	//_velocity = glm::vec3(0.0f, 0.0f, 1.0f) * _speed;
	for (int i = 0; i < 3; i++)
	{
		_localModelMatrix[3][i] += _velocity[i] * deltaTime;
	}

	if (std::fabs(_speed) > glm::epsilon<float>())
	{
		return true;
	}
	return false;
}

void Transform::updateLocalWithVelocityOrSpeed(float deltaTime)
{
	if (_bVelocity)
	{
		for (int i = 0; i < 3; i++)
		{
			_localModelMatrix[3][i] += _velocity[i] * deltaTime;
		}
		
		return;
	}

	// use speed
	_velocity = _localRotateMatrix[2] * _speed;
	for (int i = 0; i < 3; i++)
	{
		_localModelMatrix[3][i] += _velocity[i] * deltaTime;
	}

	return;
}

void Transform::updateWithDirtyParent(float deltaTime, glm::mat4 & _parentWorldMat)
{
	if (!_bMove)
	{
		for (auto childTransformPtr : _childTransformPtrList)
		{
			childTransformPtr->updateWIthNoDirtyParent(deltaTime);
		}
		
		return;
	}

	_bDirty = true;
	_worldTotalMatrix = _parentWorldMat * (_localModelMatrix * _localRotateMatrix * _localScaleMatrix);
	for (auto childTransformPtr : _childTransformPtrList)
	{
		childTransformPtr->updateWithDirtyParent(deltaTime, _worldTotalMatrix);
	}
}

void Transform::updateWIthNoDirtyParent(float deltaTime)
{
	if (!_bMove)
	{
		for (auto childTransformPtr : _childTransformPtrList)
		{
			childTransformPtr->updateWIthNoDirtyParent(deltaTime);
		}
		return;
	}

	if (_bDirty)	// this transform is dirty.. dirty propagation.
	{
		bUpdateLocalWithVelocityOrSpeed(deltaTime);
		_worldTotalMatrix = _parentTransformPtr->getWorldMatRef() * (_localModelMatrix * _localRotateMatrix * _localScaleMatrix);

		for (auto childTransformPtr : _childTransformPtrList)
		{
			childTransformPtr->updateWithDirtyParent(deltaTime, _worldTotalMatrix);
		}
		return;
	}

	// no dirty.
	for (auto childTransformPtr : _childTransformPtrList)
	{
		childTransformPtr->updateWIthNoDirtyParent(deltaTime);
	}
}
