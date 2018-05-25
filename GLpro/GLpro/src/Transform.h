#pragma once

#include "../stdafx.h"

/******************************************************
*  Document   :	Transform.h
*  Description: Transform
*  physics loop
*  { 
*		rigidbody Comp에서 최상위 transform부터 dirty bit를 이용한 world matrix update 시작하며
*		delta 존재시 적용후 dirty on( O(n) )	// transform 자식으로 collision을 넣어 collision update도 하면서 순회도중에 dirty init 하는 방법 존재.
*		collision box update에 dirty & world matrix 사용
*		collision event push & collision message 남김
*		dirty bit init ( O(n) )
*  }
*  transform 개별 조작시(logic update) dirty,
*  collision message로 logic update
*
*  set : local matrix	//	get : world matrix
*******************************************************/

class Transform {
public:
	bool isDrawableObjDelete = false;	//If true, will be deleted
	bool isDrawableObjDraw = true;	//If true, draw.

	Transform(int entityID, const glm::mat4 &modelMatrix = glm::mat4(), const glm::mat4 &rotateMatrix = glm::mat4(), const glm::mat4 &scaleMatrix = glm::mat4());

	virtual ~Transform() {};

	virtual int GetEntityID() const {	return _entityID; };
	
	virtual const glm::mat4& getWorldMatRef() const;
	virtual glm::mat4 getWorldMat() const;
	virtual glm::vec3 getWorldPosVec() const;
	//virtual const glm::vec3& getWorldPosVecRef() const;

	// model mat
	virtual const glm::mat4& getModelMatrixConstRef() const;
	virtual glm::mat4 getModelMatrix() const;
	virtual glm::vec3 getModelVec() const;

	virtual void setModelMatrix(const glm::mat4 &localModelMatrix);
	virtual void setModelMatrix(const glm::vec3 &localModelVec);

	virtual void accModelMatrix(const glm::mat4 &localAccModelMatrix);
	virtual void accModelMatrix(const glm::vec3 &localAccModelVec);

	virtual void translateModelMatrix(const glm::vec3 & modelVec);
	virtual void setVelocity(const glm::vec3& velocity);
	virtual glm::vec3 getVelocity() const;
	virtual glm::vec3& getVelocityRef();

	virtual void speedAdd(float add);
	virtual void speedSet(float speed);
	virtual float getSpeed();
	virtual float getMaxSpeed();
	virtual void setMaxSpeed(float maxSpeed);

	// rotate mat
	virtual const glm::mat4& getLocalRotationMatrixConstRef() const;
	virtual glm::mat4 getLocalRotationMatrix() const;
	virtual glm::quat getLocalQuarternion() const;

	virtual void setRotationMatrix(const glm::mat4 &rotateMat);
	virtual void setRotationMatrix(const glm::vec3 &rotateVec);
	virtual void setRotationMatrix(const glm::quat &quat);
	virtual void setVMatrixLookat(const glm::vec3 & lookat, const glm::vec3 & up);
	virtual void setVMatrixLookat(const glm::vec3 & pos, const glm::vec3 & lookat, const glm::vec3 & up);

	virtual void accRotationMatrix(const glm::mat4 &localAccRotateMat);
	virtual void accRotationMatrix(const float &degree, glm::vec3 &rotateAxis);
	virtual void accRotationMatrix(const glm::quat &quat);

	// scale mat
	virtual const glm::mat4& getLocalScaleMatrixConstRef() const;
	virtual glm::mat4 getLocalScaleMatrix() const;

	virtual void setScaleMatrix(const glm::mat4 &localScaleMat);
	virtual void setScaleMatrix(const glm::vec3 &localScaleVec);

	virtual void accScaleMatrix(const glm::mat4 &scaleMat);
	virtual void accScaleMatrix(const glm::vec3 &scaleVec);

	virtual Transform* getParentTransformPtr();
	virtual Transform* getChildTransformWithID(int id);

	virtual Transform* detachParentTransform();
	virtual Transform* detachChildTransformWithID(int idx);
	
	virtual void attachParentTransform(Transform* parentTransform);
	virtual void attachChildTransform(Transform* childTransform);

	virtual void resetDirty();
	virtual bool isDirty();
	virtual void setDirty();
	virtual void setMove(bool bMove);
	virtual void update(float deltaTime);

	//for debug
	virtual void printLocalModel();
	virtual void printLocalRotMat();
	virtual void printWorldMat();


protected:
	virtual bool bUpdateLocalWithVelocityOrSpeed(float deltaTime);
	virtual void updateLocalWithVelocityOrSpeed(float deltaTime);
	virtual void updateWithDirtyParent(float deltaTime, glm::mat4& _parentWorldMat);
	virtual void updateWIthNoDirtyParent(float deltaTime);

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


	glm::mat4 _worldTotalMatrix;
	Transform* _parentTransformPtr;
	std::list<Transform*> _childTransformPtrList;

	bool _bDirty;

	friend class Entity;
};
