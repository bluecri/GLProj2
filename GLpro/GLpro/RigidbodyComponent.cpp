#include "stdafx.h"

#include <glm/gtx/matrix_decompose.hpp>

#include "src/Entity.h"
#include "RigidbodyComponent.h"

int RigidbodyComponent::_sMaskBDirty = DIRTY_MASK_RIGID;

RigidbodyComponent::RigidbodyComponent(Entity * bindedEntity, const glm::mat4 &modelMatrix, const glm::mat4 &quaternion, const glm::mat4 &scaleMatrix)
	: _bindedEntity(bindedEntity), _localModelMatrix(modelMatrix), _localQuaternion(quaternion), _localScaleMatrix(scaleMatrix)
{
	_bdoDelete = false;
	_velocity = glm::vec3();
	_bMove = true;
	_bVelocity = false;			// bVelocity == use original velocity for move
	_parentRigidbodyComponentPtr = nullptr;
	_childRigidbodyComponentPtrList = std::list<RigidbodyComponent*>();
	setDirty();		// update local -> world	:: 없으면 child로 신규 생성시 world update X
	//_maxZSpeed = 100.0f;	// 100 velocity
	_mass = 1.0f;

	_bTargetQuat = false;
	_targetQuat = glm::quat();
}

RigidbodyComponent::~RigidbodyComponent()
{
}

const glm::mat4 & RigidbodyComponent::getWorldMatRef() const
{
	return _worldTotalMatrix;
}


glm::mat4 RigidbodyComponent::getWorldMat() const
{
	return _worldTotalMatrix;
}

glm::vec3 RigidbodyComponent::getWorldPosVec() const
{
	return _worldTotalMatrix[3];
}

void RigidbodyComponent::setWorldMat(glm::mat4 & inMat)
{
	_worldTotalMatrix = inMat;
}

void RigidbodyComponent::setWorldPosVec(glm::vec3 & inPos)
{
	_worldTotalMatrix[3][0] = inPos[0];
	_worldTotalMatrix[3][1] = inPos[1];
	_worldTotalMatrix[3][2] = inPos[2];
}

const glm::mat4& RigidbodyComponent::getModelMatrixConstRef() const
{
	return _localModelMatrix;
}

glm::mat4 RigidbodyComponent::getModelMatrix() const
{
	return _localModelMatrix;
}

glm::vec3 RigidbodyComponent::getModelVec() const
{
	glm::vec3 ret = _localModelMatrix[3];
	return ret;
}

void RigidbodyComponent::setLocalMatWithWorldMat(const glm::mat4 & worldMat)
{
	setDirty();

	// do matrix decomposition (Model -> scale, rotate, transition

	for (int i = 0; i < 3; i++)
		_localModelMatrix[3][i] = worldMat[3][i];

	for (int i = 0; i < 3; i++)
	{
		glm::vec3 len = glm::vec3(worldMat[i][0], worldMat[i][1], worldMat[i][2]);
		_localScaleMatrix[i][i] = glm::length(len);
	}

	glm::mat3 localRotateMatrix;
	for (int i = 0; i < 3; i++) {
		for (int k = 0; k < 3; k++)
		{
			localRotateMatrix[i][k] = worldMat[i][k] / _localScaleMatrix[i][i];
		}
	}

	_localQuaternion = glm::toQuat(localRotateMatrix);

	return;
}


glm::mat4 RigidbodyComponent::getLocalRotationMatrix() const
{
	return glm::toMat4(_localQuaternion);
}

glm::quat RigidbodyComponent::getLocalQuarternion() const
{
	return _localQuaternion;
}

const glm::quat & RigidbodyComponent::getLocalQuarternionRef() const
{
	return _localQuaternion;
}

const glm::mat4 & RigidbodyComponent::getLocalScaleMatrixConstRef() const
{
	return _localScaleMatrix;
}

glm::mat4 RigidbodyComponent::getLocalScaleMatrix() const
{
	return _localScaleMatrix;
}
/*
glm::quat RigidbodyComponent::getLocalQuarternion() const
{
return glm::quat_cast(_localRotateMatrix);
}*/

void RigidbodyComponent::setModelMatrix(const glm::mat4 &localModelMatrix)
{
	setDirty();
	_localModelMatrix = localModelMatrix;
}

void RigidbodyComponent::setQuaternion(const glm::mat4 &rotateMat)
{
	setDirty();
	_localQuaternion = glm::toQuat(rotateMat);
}

void RigidbodyComponent::setScaleMatrix(const glm::mat4 &localScaleMat)
{
	setDirty();
	_localScaleMatrix = localScaleMat;
}

void RigidbodyComponent::setModelMatrix(const glm::vec3 &localModelVec)
{
	setDirty();
	_localModelMatrix = glm::translate(glm::mat4(), localModelVec);
}

void RigidbodyComponent::setQuaternion(const glm::vec3 &rotateVec)
{
	setDirty();
	_localQuaternion = glm::quat(rotateVec);
}

void RigidbodyComponent::setScaleMatrix(const glm::vec3 &localScaleVec)
{
	setDirty();
	_localScaleMatrix = glm::scale(glm::mat4(), localScaleVec);
}

void RigidbodyComponent::setQuaternion(const glm::quat &quat)
{
	setDirty();
	_localQuaternion = quat;
}

void RigidbodyComponent::setVMatrixLookat(const glm::vec3 & lookat, const glm::vec3 & up)
{
	setDirty();
	glm::vec3 modelVec = _localModelMatrix[3];
	glm::mat4 localRotateMatrix = glm::lookAt(modelVec, modelVec + lookat, up);

	_localQuaternion = glm::toQuat(localRotateMatrix);
}

void RigidbodyComponent::setVMatrixLookat(const glm::vec3 & pos, const glm::vec3 & lookat, const glm::vec3 & up)
{
	setDirty();
	glm::mat4 localRotateMatrix = glm::lookAt(pos, pos + lookat, up);

	_localQuaternion = glm::toQuat(localRotateMatrix);
}

void RigidbodyComponent::accModelMatrix(const glm::mat4 &localAccModelMatrix)
{
	setDirty();
	_localModelMatrix[3][0] += localAccModelMatrix[3][0];
	_localModelMatrix[3][1] += localAccModelMatrix[3][1];
	_localModelMatrix[3][2] += localAccModelMatrix[3][2];
}

void RigidbodyComponent::accQuaternion(const glm::mat4 &localAccRotateMat)
{
	setDirty();
	_localQuaternion = glm::toQuat(localAccRotateMat) * _localQuaternion;
}

void RigidbodyComponent::accScaleMatrix(const glm::mat4 &scaleMat)
{
	setDirty();
	_localScaleMatrix[0][0] *= scaleMat[0][0];
	_localScaleMatrix[1][1] *= scaleMat[1][1];
	_localScaleMatrix[2][2] *= scaleMat[2][2];
}

void RigidbodyComponent::accModelMatrix(const glm::vec3 &modelVec)
{
	setDirty();
	for (int i = 0; i<3; i++)
		_localModelMatrix[3][i] += modelVec[i];
}

void RigidbodyComponent::translateModelMatrix(const glm::vec3 &modelVec)
{
	setDirty();
	glm::vec3 moveVec = _localQuaternion * modelVec;
	_localModelMatrix = glm::translate(_localModelMatrix, moveVec);
	//_localModelMatrix = glm::translate(_localModelMatrix, modelVec);
}

void RigidbodyComponent::setVelocity(const glm::vec3 & velocity)
{
	_velocity = velocity;
}

glm::vec3 RigidbodyComponent::getVelocity() const
{
	return _velocity;
}

glm::vec3 & RigidbodyComponent::getVelocityRef()
{
	return _velocity;
}

void RigidbodyComponent::speedAdd(float add)
{
	_speed += add;
}

void RigidbodyComponent::speedSet(float speed)
{
	_speed = speed;
}

float RigidbodyComponent::getSpeed() {
	return _speed;
}

// rotate axis == model rotation axis
void RigidbodyComponent::accQuaternion(const float &degree, glm::vec3 &rotateAxis)
{
	setDirty();
	_localQuaternion = glm::rotate(_localQuaternion, glm::radians(degree), rotateAxis);
}

void RigidbodyComponent::accScaleMatrix(const glm::vec3 &scaleVec)
{
	setDirty();
	_localScaleMatrix = glm::scale(_localScaleMatrix, scaleVec);
}

void RigidbodyComponent::accQuaternion(const glm::quat &quat)
{
	setDirty();
	_localQuaternion = quat * _localQuaternion;
}

void RigidbodyComponent::accQuaternionYaw(const float & degree)
{
	glm::vec3 axis = glm::vec3(0.0f, 1.0f, 0.0f);
	accQuaternion(degree, axis);
}

void RigidbodyComponent::accQuaternionPitch(const float & degree)
{
	glm::vec3 axis = glm::vec3(1.0f, 0.0f, 0.0f);
	accQuaternion(degree, axis);
}

void RigidbodyComponent::accQuaternionRoll(const float & degree)
{
	glm::vec3 axis = glm::vec3(0.0f, 0.0f, 1.0f);
	accQuaternion(degree, axis);
}

// rotate to targetQuat with maxAngle
// reference : http://www.opengl-tutorial.org/kr/intermediate-tutorials/tutorial-17-quaternions/
void RigidbodyComponent::accQuaternionMix(RigidbodyComponent* targetRigidbodyComponent, float maxAngle, float deltaSpeed)
{
	glm::quat& targetQuat = targetRigidbodyComponent->_localQuaternion;
	if (maxAngle < 0.001f) {
		// No rotation allowed. Prevent dividing by 0 later.
		return;
	}

	float cosTheta = dot(_localQuaternion, targetQuat);

	// q1 and targetQuat are already equal.
	// Force targetQuat just to be sure
	if (cosTheta > 0.9999f) {
		_localQuaternion = targetQuat;
		return;
	}

	// Avoid taking the long path around the sphere
	glm::quat testLocalQuat = _localQuaternion;
	if (cosTheta < 0) {
		testLocalQuat = testLocalQuat*-1.0f;
		cosTheta *= -1.0f;
	}

	float angle = acos(cosTheta);

	// If there is only a 2&deg; difference, and we are allowed 5&deg;,
	// then we arrived.
	if (angle < maxAngle) {
		_localQuaternion = targetQuat;
		return;
	}

	float fT = maxAngle / angle * deltaSpeed;
	angle = maxAngle;

	quat res = (sin((1.0f - fT) * angle) * testLocalQuat + sin(fT * angle) * targetQuat) / sin(angle);
	res = normalize(res);
	_localQuaternion = res;

	return;
}

RigidbodyComponent * RigidbodyComponent::getParentRigidbodyComponentPtr()
{
	return _parentRigidbodyComponentPtr;
}

RigidbodyComponent * RigidbodyComponent::getChildRigidbodyComponentWithID(int id)
{
	for (auto elem : _childRigidbodyComponentPtrList)
	{
		if (elem->_bindedEntity->getID() == id)
		{
			return elem;
		}
	}
	return nullptr;

}

RigidbodyComponent * RigidbodyComponent::detachParentRigidbodyComponent()
{
	RigidbodyComponent* ret = _parentRigidbodyComponentPtr;
	ret->detachChildRigidbodyComponentWithID(_bindedEntity->getID());
	return ret;
}

RigidbodyComponent * RigidbodyComponent::detachChildRigidbodyComponentWithID(int id)
{
	auto it = std::find_if(_childRigidbodyComponentPtrList.begin(), _childRigidbodyComponentPtrList.end(), [id](RigidbodyComponent* &elem) {
		return elem->_bindedEntity->getID() == id;
	});

	if (it != _childRigidbodyComponentPtrList.end())
	{
		auto ret = (*it);

		(*it)->_parentRigidbodyComponentPtr = nullptr;
		_childRigidbodyComponentPtrList.erase(it);

		return ret;
	}

	return nullptr;
}


void RigidbodyComponent::attachParentRigidbodyComponent(RigidbodyComponent * parentRigidbodyComponent)
{
	parentRigidbodyComponent->attachChildRigidbodyComponent(this);
	return;
}

void RigidbodyComponent::attachChildRigidbodyComponent(RigidbodyComponent * childRigidbodyComponent)
{
	_childRigidbodyComponentPtrList.push_back(childRigidbodyComponent);
	childRigidbodyComponent->_parentRigidbodyComponentPtr = this;
	return;
}

void RigidbodyComponent::resetAndSwapDirty()
{
	_dirtyBit = (_dirtyBit & DIRTY_MASK_LOGIC) << DIRTYBIT_PROPAGATION;
}

bool RigidbodyComponent::isDirtyForRender()
{
	return _bDirtyRender;
}

void RigidbodyComponent::setDirtyForRender(bool bDirty)
{
	_bDirtyRender = bDirty;
}

bool RigidbodyComponent::isDirtyByRigidLoopUpdate()
{
	return (_dirtyBit & DIRTY_MASK_RIGID);
}

bool RigidbodyComponent::isDirtyByLogicLoopUpdate()
{
	return (_dirtyBit & DIRTY_MASK_LOGIC);
}

bool RigidbodyComponent::isDirtyAll()
{
	return (_dirtyBit != 0);
}

/*
 * Set Dirty bit when modified.
 * If Rigidbody is modified then set DIRTY_MASK_RIGID
 * If logic modified rigidbody then set DIRTY_MASK_LOGIC
 */
void RigidbodyComponent::setDirty()
{
	_dirtyBit = _dirtyBit | _sMaskBDirty;
	setDirtyForRender(true);
}

void RigidbodyComponent::changeSetDirtyBitToRigid()
{
	_sMaskBDirty = DIRTY_MASK_RIGID;
}

void RigidbodyComponent::changeSetDirtyBitToLogic()
{
	_sMaskBDirty = DIRTY_MASK_LOGIC;
}

void RigidbodyComponent::setMove(bool bMove)
{
	_bMove = bMove;
}

void RigidbodyComponent::setBTargetQuat(bool bTargetQuat)
{
	_bTargetQuat = bTargetQuat;
}

void RigidbodyComponent::setTargetQuat(glm::quat & targetQuat)
{
	_targetQuat = targetQuat;
}

Entity * RigidbodyComponent::getBindedEntity()
{
	return _bindedEntity;
}

void RigidbodyComponent::setBDeleted() {
	_bdoDelete = true;
}

bool RigidbodyComponent::isBDeleted() {
	return _bdoDelete;
}

void RigidbodyComponent::setMass(float mass)
{
	_mass = mass;
}

float RigidbodyComponent::getMass() {
	return _mass;
}

void RigidbodyComponent::updateWorldMatrix(float deltaTime)
{
	if (_parentRigidbodyComponentPtr != nullptr)		// skip if child (will be visited by tree traversal.
	{
		return;
	}

	if (!_bMove)
	{
		for (auto childRigidbodyComponentPtr : _childRigidbodyComponentPtrList)
		{
			childRigidbodyComponentPtr->updateWIthNoDirtyParent(deltaTime);
		}
		return;
	}

	if (_dirtyBit)
	{
		updateLocalWithVelocityOrSpeed(deltaTime);
		//_worldTotalMatrix = (_localModelMatrix * glm::toMat4(_localQuaternion) * _localScaleMatrix);
		calcWorldMatWithLocalPriv();

		for (auto childRigidbodyComponentPtr : _childRigidbodyComponentPtrList)
		{
			childRigidbodyComponentPtr->updateWithDirtyParent(deltaTime, _worldTotalMatrix);
		}
		return;
	}

	if (bUpdateLocalWithVelocityOrSpeed(deltaTime)) {
		_dirtyBit = true;
		//_worldTotalMatrix = (_localModelMatrix * glm::toMat4(_localQuaternion) * _localScaleMatrix);
		calcWorldMatWithLocalPriv();

		for (auto childRigidbodyComponentPtr : _childRigidbodyComponentPtrList)
		{
			childRigidbodyComponentPtr->updateWithDirtyParent(deltaTime, _worldTotalMatrix);
		}
		return;
	}

	for (auto childRigidbodyComponentPtr : _childRigidbodyComponentPtrList)
	{
		childRigidbodyComponentPtr->updateWIthNoDirtyParent(deltaTime);
	}

}

//for debug

void RigidbodyComponent::printLocalModel()
{
	std::cout << "model vec" << std::endl;
	for (int i = 0; i < 3; i++)
		std::cout << _localModelMatrix[3][i] << ", ";
	std::cout << std::endl;
}

void RigidbodyComponent::printLocalRotMat()
{
	std::cout << "rotation mat" << std::endl;
	for (int i = 0; i < 4; i++)
	{
		for (int k = 0; k < 4; k++)
		{
			//std::cout << _localRotateMatrix[i][k] << ", ";
		}
		//std::cout << std::endl;
	}
}

void RigidbodyComponent::printWorldMat()
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

// if velocity is not 0 OR rotated -> return true
bool RigidbodyComponent::bUpdateLocalWithVelocityOrSpeed(float deltaTime)
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
	//_velocity = glm::vec3(0.0f, 0.0f, 1.0f) * _speed;
	_velocity = _localQuaternion * glm::vec3(0.0f, 0.0f, _speed);
	/*printf_s("quat : %f %f %f %f, _velo : %f %f %f\n", _localQuaternion.x, _localQuaternion.y, _localQuaternion.z, _localQuaternion.w,
	_velocity.x, _velocity.y, _velocity.z);*/
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

// no velocity check
void RigidbodyComponent::updateLocalWithVelocityOrSpeed(float deltaTime)
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
	//_velocity =  _localRotateMatrix[2] * _speed;
	_velocity = _localQuaternion * glm::vec3(0.0f, 0.0f, _speed);
	for (int i = 0; i < 3; i++)
	{
		_localModelMatrix[3][i] += _velocity[i] * deltaTime;
	}

	return;
}

void RigidbodyComponent::updateWithDirtyParent(float deltaTime, glm::mat4 & parentWorldMat)
{
	if (!_bMove)
	{
		for (auto childRigidbodyComponentPtr : _childRigidbodyComponentPtrList)
		{
			childRigidbodyComponentPtr->updateWIthNoDirtyParent(deltaTime);
		}

		return;
	}

	_dirtyBit = true;

	//_worldTotalMatrix = parentWorldMat * (_localModelMatrix * glm::toMat4(testQuat) * _localScaleMatrix);
	calcWorldMatPriv(parentWorldMat);
	for (auto childRigidbodyComponentPtr : _childRigidbodyComponentPtrList)
	{
		childRigidbodyComponentPtr->updateWithDirtyParent(deltaTime, _worldTotalMatrix);
	}
}

void RigidbodyComponent::updateWIthNoDirtyParent(float deltaTime)
{
	if (!_bMove)
	{
		for (auto childRigidbodyComponentPtr : _childRigidbodyComponentPtrList)
		{
			childRigidbodyComponentPtr->updateWIthNoDirtyParent(deltaTime);
		}
		return;
	}

	if (_dirtyBit)	// this RigidbodyComponent is dirty.. dirty propagation.
	{
		bUpdateLocalWithVelocityOrSpeed(deltaTime);
		calcWorldMatPriv(_parentRigidbodyComponentPtr->getWorldMatRef());
		//_worldTotalMatrix = _parentRigidbodyComponentPtr->getWorldMatRef() * (_localModelMatrix * glm::toMat4(_localQuaternion) * _localScaleMatrix);

		for (auto childRigidbodyComponentPtr : _childRigidbodyComponentPtrList)
		{
			childRigidbodyComponentPtr->updateWithDirtyParent(deltaTime, _worldTotalMatrix);
		}
		return;
	}

	// no dirty.
	for (auto childRigidbodyComponentPtr : _childRigidbodyComponentPtrList)
	{
		childRigidbodyComponentPtr->updateWIthNoDirtyParent(deltaTime);
	}
}

void RigidbodyComponent::calcWorldMatWithLocalPriv()
{
	_worldTotalMatrix = glm::toMat4(_localQuaternion);

	for (int i = 0; i < 3; i++)
	{
		_worldTotalMatrix[i][0] *= _localScaleMatrix[i][i];
		_worldTotalMatrix[i][1] *= _localScaleMatrix[i][i];
		_worldTotalMatrix[i][2] *= _localScaleMatrix[i][i];
	}
	
	_worldTotalMatrix[3][0] = _localModelMatrix[3][0];
	_worldTotalMatrix[3][1] = _localModelMatrix[3][1];
	_worldTotalMatrix[3][2] = _localModelMatrix[3][2];

}

void RigidbodyComponent::calcWorldMatPriv(const glm::mat4 & parentMat)
{
	_worldTotalMatrix = glm::toMat4(_localQuaternion);

	for (int i = 0; i < 3; i++)
	{
		_worldTotalMatrix[i][0] *= _localScaleMatrix[i][i];
		_worldTotalMatrix[i][1] *= _localScaleMatrix[i][i];
		_worldTotalMatrix[i][2] *= _localScaleMatrix[i][i];
	}

	_worldTotalMatrix[3][0] = _localModelMatrix[3][0];
	_worldTotalMatrix[3][1] = _localModelMatrix[3][1];
	_worldTotalMatrix[3][2] = _localModelMatrix[3][2];

	_worldTotalMatrix = parentMat * _worldTotalMatrix;
}


