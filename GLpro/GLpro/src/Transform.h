#pragma once
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#include <vector>

#include "./Entity.h"

/******************************************************
*  Document   :	Transform.h
*  Description: Transform
*******************************************************/

class Transform : public Entity {
public:
	class Builder;
	glm::vec3 modelVec;
	bool isDrawableObjDelete = false;	//If true, will be deleted
	bool isDrawableObjDraw = true;	//If true, draw.

	Transform(const glm::mat4 &modelMatrix, const glm::mat4 &rotateMatrix, const glm::mat4 &scaleMatrix, int &type, Transform *childTransform) : Entity(type) {
		_modelMatrix = modelMatrix;
		_rotateMatrix = rotateMatrix;
		_scaleMatrix = scaleMatrix;
	}

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
	Transform();
	glm::mat4 _modelMatrix;
	glm::mat4 _rotateMatrix;
	glm::mat4 _scaleMatrix;
	std::vector<Transform*> childTransformVec;
};

class Transform::Builder 
{
	public:
		Builder() {
			_modelMatrix = glm::mat4();
			_rotateMatrix = glm::mat4();
			_scaleMatrix = glm::mat4();
			_type = 0;
			_childTransform = NULL;
		}

		Transform& setModelMat(const glm::mat4 &modelMat) {
			_modelMatrix = modelMat;
		}

		Transform& setModelMat(const glm::vec3 &modelVec) {
			_modelMatrix = glm::translate(glm::mat4(), modelVec);
		}

		Transform& setRotMat(const glm::mat4 &rotateMatrix) {
			_rotateMatrix = rotateMatrix;
		}

		Transform& setRotMat(const glm::vec3 &angleVec) {
			_rotateMatrix = glm::toMat4(glm::quat(angleVec));
		}

		Transform& setScaleMat(const glm::mat4 &scaleMatrix) {
			_scaleMatrix = scaleMatrix;
		}

		Transform& setScaleMat(const glm::vec3 &scaleVec) {
			_scaleMatrix = glm::scale(glm::mat4(), scaleVec);
		}

		Transform& addChildTrnasform(Transform *childTransform) {
			_childTransformVec.push_back(childTransform);
		}

		Transform& setType(int type) {
			_type = type;
		}

		Transform* constructPtr() {
			return new Transform(_modelMatrix, _rotateMatrix, _scaleMatrix, _type, _childTransform);
		}

	private:
		glm::mat4 _modelMatrix;
		glm::mat4 _rotateMatrix;
		glm::mat4 _scaleMatrix;
		int _type;
		std::vector<Transform*> _childTransformVec;
		Transform* _parentTransform;
};