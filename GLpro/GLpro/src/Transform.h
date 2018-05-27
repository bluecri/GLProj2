#pragma once

#include "../stdafx.h"

/******************************************************
*  Document   :	Transform.h
*  Description: Transform
*  physics loop
*  { 
*		rigidbody Comp���� �ֻ��� transform���� dirty bit�� �̿��� world matrix update �����ϸ�
*		delta ����� ������ dirty on( O(n) )	// transform �ڽ����� collision�� �־� collision update�� �ϸ鼭 ��ȸ���߿� dirty init �ϴ� ��� ����.
*		collision box update�� dirty & world matrix ���
*		collision event push & collision message ����
*		dirty bit init ( O(n) )
*  }
*  transform ���� ���۽�(logic update) dirty,
*  collision message�� logic update
*
*  set : local matrix	//	get : world matrix
*******************************************************/

class Transform {
public:
	bool isDrawableObjDelete = false;	//If true, will be deleted
	bool isDrawableObjDraw = true;	//If true, draw.

	Transform(int entityID, const glm::mat4 &modelMatrix = glm::mat4(), const glm::mat4 &rotateMatrix = glm::mat4(), const glm::mat4 &scaleMatrix = glm::mat4());

	virtual ~Transform() {};

	int GetEntityID() const {	return _entityID; };
	
	const glm::mat4& getWorldMatRef() const;
	glm::mat4 getWorldMat() const;
	glm::vec3 getWorldPosVec() const;
	//virtual const glm::vec3& getWorldPosVecRef() const;

	// model mat
	const glm::mat4& getModelMatrixConstRef() const;
	glm::mat4 getModelMatrix() const;
	glm::vec3 getModelVec() const;

	void setLocalMatWithWorldMat(glm::mat4& worldMat);

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
	float getMaxSpeed();
	void setMaxSpeed(float maxSpeed);

	// rotate mat
	const glm::mat4& getLocalRotationMatrixConstRef() const;
	glm::mat4 getLocalRotationMatrix() const;
	glm::quat getLocalQuarternion() const;

	void setRotationMatrix(const glm::mat4 &rotateMat);
	void setRotationMatrix(const glm::vec3 &rotateVec);
	void setRotationMatrix(const glm::quat &quat);
	void setVMatrixLookat(const glm::vec3 & lookat, const glm::vec3 & up);
	void setVMatrixLookat(const glm::vec3 & pos, const glm::vec3 & lookat, const glm::vec3 & up);

	void accRotationMatrix(const glm::mat4 &localAccRotateMat);
	void accRotationMatrix(const float &degree, glm::vec3 &rotateAxis);
	void accRotationMatrix(const glm::quat &quat);

	// scale mat
	const glm::mat4& getLocalScaleMatrixConstRef() const;
	glm::mat4 getLocalScaleMatrix() const;

	void setScaleMatrix(const glm::mat4 &localScaleMat);
	void setScaleMatrix(const glm::vec3 &localScaleVec);

	void accScaleMatrix(const glm::mat4 &scaleMat);
	void accScaleMatrix(const glm::vec3 &scaleVec);

	Transform* getParentTransformPtr();
	Transform* getChildTransformWithID(int id);

	Transform* detachParentTransform();
	Transform* detachChildTransformWithID(int idx);
	
	void attachParentTransform(Transform* parentTransform);
	void attachChildTransform(Transform* childTransform);

	void resetDirty();
	bool isDirty();
	void setDirty();
	void setMove(bool bMove);
	void setMass(float mass);
	float getMass();

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

private:
	int _entityID;
	glm::mat4 _localModelMatrix;
	glm::mat4 _localRotateMatrix;
	glm::mat4 _localScaleMatrix;

	bool _bMove;		// update move or not
	bool _bVelocity;	// update move with original glm::vec3 _velocity
	glm::vec3 _velocity;
	float _speed;
	float _maxZSpeed;
	float _mass;


	glm::mat4 _worldTotalMatrix;
	Transform* _parentTransformPtr;
	std::list<Transform*> _childTransformPtrList;

	bool _bDirty;

	friend class Entity;
};
