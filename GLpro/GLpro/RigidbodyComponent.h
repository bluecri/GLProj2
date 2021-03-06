#pragma once
#include "stdafx.h"
#include "ObjectPoolLikeVector.h"

#define DIRTY_MASK_RIGID		(1 << 4)
#define DIRTY_MASK_LOGIC		(1 << 1)
#define DIRTYBIT_PROPAGATION	(DIRTY_MASK_RIGID / DIRTY_MASK_LOGIC)

/******************************************************
*  Document   :	RigidbodyComponent.h
*  Description: RigidbodyComponent
*  physics loop
*  {
*		rigidbody Comp에서 최상위 Comp부터 dirty bit를 이용한 world matrix update 시작하며
*		delta 존재시 적용후 dirty on( O(n) )
*		collision box update에 dirty & world matrix 사용
*		collision event push & collision message 남김
*		dirty bit init ( O(n) )
*  }
*  RigidbodyComponent 개별 조작시(logic update) dirty,
*  collision message로 logic update
*
*  set : local matrix	//	get : world matrix
*******************************************************/

class Entity;

// todo : logic update에서 matrix modify시 child matrix modify가 1 frame 늦게 작동.
class RigidbodyComponent : public ObjectPoolLikeVector<RigidbodyComponent>
{
public:
	RigidbodyComponent(Entity * bindedEntity, const glm::mat4 &modelMatrix = glm::mat4(), const glm::mat4 &rotateMatrix = glm::mat4(), const glm::mat4 &scaleMatrix = glm::mat4());
	virtual ~RigidbodyComponent();

	const glm::mat4& getWorldMatRef() const;
	glm::mat4 getWorldMat() const;
	glm::vec3 getWorldPosVec() const;
	void setWorldMat(glm::mat4& inMat);
	void setWorldPosVec(glm::vec3& inPos);
	//virtual const glm::vec3& getWorldPosVecRef() const;

	// model mat
	const glm::mat4& getModelMatrixConstRef() const;
	glm::mat4 getModelMatrix() const;
	glm::vec3 getModelVec() const;

	void setLocalMatWithWorldMat(const glm::mat4& worldMat);

	void setModelMatrix(const glm::mat4 &localModelMatrix);
	void setModelMatrix(const glm::vec3 &localModelVec);

	void accModelMatrix(const glm::mat4 &localAccModelMatrix);
	void accModelMatrix(const glm::vec3 &localAccModelVec);

	void translateModelMatrix(const glm::vec3 & modelVec);
	void setVelocity(const glm::vec3& velocity);
	glm::vec3 getVelocity() const;
	glm::vec3& getVelocityRef();

	void speedAdd(float add);
	void speedSet(float speed);
	float getSpeed();

	//float getMaxSpeed();
	//void setMaxSpeed(float maxSpeed);

	float getMass();
	void setMass(float mass);

	// rotate mat
	//const glm::mat4& getLocalRotationMatrixConstRef() const;
	glm::mat4 getLocalRotationMatrix() const;
	glm::quat getLocalQuarternion() const;
	const glm::quat& getLocalQuarternionRef() const;

	void setQuaternion(const glm::mat4 &rotateMat);
	void setQuaternion(const glm::vec3 &rotateVec);
	void setQuaternion(const glm::quat &quat);
	void setVMatrixLookat(const glm::vec3 & lookat, const glm::vec3 & up);
	void setVMatrixLookat(const glm::vec3 & pos, const glm::vec3 & lookat, const glm::vec3 & up);

	void accQuaternion(const glm::mat4 &localAccRotateMat);
	void accQuaternion(const float &degree, glm::vec3 &rotateAxis);
	void accQuaternion(const glm::quat &quat);
	void accQuaternionYaw(const float &degree);
	void accQuaternionPitch(const float &degree);
	void accQuaternionRoll(const float &degree);

	void accQuaternionMix(RigidbodyComponent* targetRigidbodyComponent, float maxAngle, float deltaSpeed);

	// scale mat
	const glm::mat4& getLocalScaleMatrixConstRef() const;
	glm::mat4 getLocalScaleMatrix() const;

	void setScaleMatrix(const glm::mat4 &localScaleMat);
	void setScaleMatrix(const glm::vec3 &localScaleVec);

	void accScaleMatrix(const glm::mat4 &scaleMat);
	void accScaleMatrix(const glm::vec3 &scaleVec);

	RigidbodyComponent* getParentRigidbodyComponentPtr();
	RigidbodyComponent* getChildRigidbodyComponentWithID(int id);

	RigidbodyComponent* detachParentRigidbodyComponent();
	RigidbodyComponent* detachChildRigidbodyComponentWithID(int idx);

	void attachParentRigidbodyComponent(RigidbodyComponent* parentRigidbodyComponent);
	void attachChildRigidbodyComponent(RigidbodyComponent* childRigidbodyComponent);

	void		setDirty();
	static void changeSetDirtyBitToRigid();
	static void changeSetDirtyBitToLogic();

	bool		isDirtyByRigidLoopUpdate();
	bool		isDirtyByLogicLoopUpdate();
	bool		isDirtyAll();
	void		resetAndSwapDirty();

	bool		isDirtyForRender();
	void		setDirtyForRender(bool bDirty);

	void setMove(bool bMove);
	void setBTargetQuat(bool bTargetQuat);
	void setTargetQuat(glm::quat& targetQuat);

	Entity* getBindedEntity();

	void setBDeleted();
	bool isBDeleted();

	void updateWorldMatrix(float deltaTime);

	//for debug
	void printLocalModel();
	void printLocalRotMat();
	void printWorldMat();

protected:
	bool bUpdateLocalWithVelocityOrSpeed(float deltaTime);
	void updateLocalWithVelocityOrSpeed(float deltaTime);
	void updateWithDirtyParent(float deltaTime, glm::mat4& _parentWorldMat);
	void updateWIthNoDirtyParent(float deltaTime);
	void calcWorldMatWithLocalPriv();
	void calcWorldMatPriv(const glm::mat4& parentMat);

private:
	Entity* _bindedEntity;
	bool _bdoDelete;

	glm::mat4 _localModelMatrix;
	glm::quat _localQuaternion;
	glm::mat4 _localScaleMatrix;

	bool _bMove;		// update move or not
	bool _bVelocity;	// update move with original glm::vec3 _velocity
	glm::vec3 _velocity;

	bool _bTargetQuat;	// update with targetQuat or not
	glm::quat _targetQuat;

	float _speed;
	//float _maxZSpeed;
	float _mass;

	glm::mat4 _worldTotalMatrix;
	RigidbodyComponent* _parentRigidbodyComponentPtr;
	std::list<RigidbodyComponent*> _childRigidbodyComponentPtrList;

	int	 _dirtyBit;
	bool _bDirtyRender;

	static int _sMaskBDirty;
};

class CTBB_Rigid_ResetAndSwapDirtyAll_do {
public:
	void operator()(RigidbodyComponent* item) const;
};

class CTBB_Rigid_ResetRenderDirtyAll_do {
public:
	void operator()(RigidbodyComponent* item) const;
};