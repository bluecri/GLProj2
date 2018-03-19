#pragma once
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#include "./Entity.h"

/******************************************************
*  Document   :	Transform.h
*  Description: Transform
*******************************************************/

class Transform : public Entity {
public:
	glm::vec3 modelVec;
	bool isDrawableObjDelete = false;	//If true, will be deleted
	bool isDrawableObjDraw = true;	//If true, draw.

	Transform() : Entity() {	};
	Transform(int type) : Entity(type) {	};
	virtual ~Transform() {};
	
	// model mat
	virtual const glm::mat4& getModelMatrixConstRef() const;
	virtual glm::mat4 getModelMatrix() const;

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

private:
	glm::mat4 _modelMatrix;
	glm::mat4 _rotateMatrix;
	glm::mat4 _scaleMatrix;
	
	Transform(const glm::vec3 &modelVec, const glm::vec3 &angleVec, const glm::vec3 &scaleVec, const int &type) : Entity(type) {
		_modelMatrix	= glm::translate(glm::mat4(), modelVec);
		_rotateMatrix	= glm::toMat4(glm::quat(angleVec));
		_scaleMatrix	= glm::scale(glm::mat4(), scaleVec);
	}
};