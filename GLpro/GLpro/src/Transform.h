#pragma once

#include "../stdafx.h"

/******************************************************
*  Document   :	Transform.h
*  Description: Transform
*******************************************************/

class Transform {
public:
	bool isDrawableObjDelete = false;	//If true, will be deleted
	bool isDrawableObjDraw = true;	//If true, draw.

	Transform(int entityID, const glm::mat4 &modelMatrix = glm::mat4(), const glm::mat4 &rotateMatrix = glm::mat4(), const glm::mat4 &scaleMatrix = glm::mat4());

	virtual ~Transform() {};

	int GetEntityID() const {	return _entityID; };
	
	// model mat
	virtual const glm::mat4& getModelMatrixConstRef() const;
	virtual glm::mat4 getModelMatrix() const;
	virtual glm::vec3 getModelVec() const;

	virtual void setModelMatrix(const glm::mat4 &modelMat);
	virtual void setModelMatrix(const glm::vec3 &modelVec);

	virtual void accModelMatrix(const glm::mat4 &modelMat);
	virtual void accModelMatrix(const glm::vec3 &modelVec);

	// rotate mat
	virtual const glm::mat4& getRotationMatrixConstRef() const;
	virtual glm::mat4 getRotationMatrix() const;
	virtual glm::quat getQuarternion() const;

	virtual void setRotationMatrix(const glm::mat4 &rotateMat);
	virtual void setRotationMatrix(const glm::vec3 &rotateVec);
	virtual void setRotationMatrix(const glm::quat &quat);
	virtual void setMVMatrixLookat(const glm::vec3 & lookat, const glm::vec3 & up);

	virtual void accRotationMatrix(const glm::mat4 &rotateMat);
	virtual void accRotationMatrix(const float &degree, glm::vec3 &rotateAxis);
	virtual void accRotationMatrix(const glm::quat &quat);

	// scale mat
	virtual const glm::mat4& getScaleMatrixConstRef() const;
	virtual glm::mat4 getScaleMatrix() const;

	virtual void setScaleMatrix(const glm::mat4 &scaleMat);
	virtual void setScaleMatrix(const glm::vec3 &scaleVec);

	virtual void accScaleMatrix(const glm::mat4 &scaleMat);
	virtual void accScaleMatrix(const glm::vec3 &scaleVec);

	virtual Transform* getParentTransformPtr();
	virtual Transform* getChildTransformWithID(int id);

	virtual Transform* detachParentTransform();
	virtual Transform* detachChildTransformWithID(int idx);
	
	virtual void attachParentTransform(Transform* parentTransform);
	virtual void attachChildTransform(Transform* childTransform);

private:
	int _entityID;
	glm::mat4 _modelMatrix;
	glm::mat4 _rotateMatrix;
	glm::mat4 _scaleMatrix;
	Transform* _parentTransformPtr;
	std::list<Transform*> _childTransformPtrList;

	friend class Entity;
};
