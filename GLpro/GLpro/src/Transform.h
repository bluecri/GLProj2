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

	int GetEntityID() const {	return _entityID; };
	
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

	void resetDirty()
	{
		_bDirty = false;
	}

	bool isDirty()
	{
		return _bDirty;
	}

	void setDirty()
	{
		_bDirty = true;
	}

	void update(float deltaTime)
	{
		if (_parentTransformPtr != nullptr)		// skip if child (will be visited by tree traversal.
		{
			return;
		}

		// do tre traversal update
		if (_bDirty)
		{
			if (_bVelocity)
			{
				glm::vec3 changedVelocity = _velocity * deltaTime;
				for (int i = 0; i < 3; i++)
				{
					_localModelMatrix[i] += changedVelocity[i];
				}
			}
			_worldTotalMatrix = (_localModelMatrix * _localRotateMatrix * _localScaleMatrix);
			for (auto childTransformPtr : _childTransformPtrList)
			{
				childTransformPtr->updateWithDirtyParent(deltaTime, _worldTotalMatrix);
			}
		}
		else
		{
			if (_bVelocity)
			{
				_bDirty = true;
				if (_bVelocity)
				{
					glm::vec3 changedVelocity = _velocity * deltaTime;
					for (int i = 0; i < 3; i++)
					{
						_localModelMatrix[i] += changedVelocity[i];
					}
				}
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
	}

protected:
	void updateWithDirtyParent(float deltaTime, glm::mat4& _parentWorldMat)
	{
		_bDirty = true;
		_worldTotalMatrix = _parentWorldMat * (_localModelMatrix * _localRotateMatrix * _localScaleMatrix);
		for (auto childTransformPtr : _childTransformPtrList)
		{
			childTransformPtr->updateWithDirtyParent(deltaTime, _worldTotalMatrix);
		}
	}

	void updateWIthNoDirtyParent(float deltaTime) 
	{
		if (_bDirty || _bVelocity)	// this transform is dirty.. dirty propagation.
		{
			_bDirty = true;
			if (_bVelocity)
			{
				glm::vec3 changedVelocity = _velocity * deltaTime;
				for (int i = 0; i < 3; i++)
				{
					_localModelMatrix[i] += changedVelocity[i];
				}
			}
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


private:
	int _entityID;
	glm::mat4 _localModelMatrix;
	glm::mat4 _localRotateMatrix;
	glm::mat4 _localScaleMatrix;

	bool _bVelocity;
	glm::vec3 _velocity;


	glm::mat4 _worldTotalMatrix;
	Transform* _parentTransformPtr;
	std::list<Transform*> _childTransformPtrList;

	bool _bDirty;

	friend class Entity;
};
