#pragma once
#include "stdafx.h"
#include "CollisionComponent.h"

#include "OBBOb.h"
#include "AABBOb.h"

class DynamicCollisionSubCompMat;

class OBBCollisionComp : public CollisionComponent
{
public:
	OBBCollisionComp(RigidbodyComponent* rigidComp, glm::mat4& localMat, glm::vec3& axisLen);

	void	updateOBBOb(const glm::mat4 & worldMat, const glm::vec3 & halfAxisSize);
	void	modifyOBBObMat(const glm::mat4 & worldMat);
	void	updateOBBObAxis(const glm::vec3& halfAxisSize);
	void	modifyWorld(const glm::mat4& worldMat);

	void setLocalMat(glm::mat4& localMat);
	void setLocalOnlyRotation(glm::mat4& rotMat);
	void setLocalPos(glm::vec3& posVec);
	void setLocalPos(glm::mat4& posMat);

	const AABBOb& getAABBObConstRef() const;
	const OBBOb& getOBBObConstRef() const;
	DynamicCollisionSubCompMat* getDynamicSubCompMat();

	// CollisionComponent을(를) 통해 상속됨
	virtual void updateCollisionComp() override;
	virtual void updateDynamicLap() override;

	virtual bool collideStaticTestToOtherStatic(CollisionComponent * staticComp) override;
	virtual bool collideStaticTestToOtherDynamic(CollisionComponent * dynamicComp, float& collideTime) override;
	virtual bool collideDynamicTestToOtherDynamic(CollisionComponent * dynamicComp, float& collideTime) override;

	virtual bool lapStaticTestToOtherStatic(CollisionComponent * staticComp) override;
	virtual bool lapStaticTestToOtherDynamic(CollisionComponent * dynamicComp) override;
	virtual bool lapDynamicTestToOtherDynamic(CollisionComponent * dynamicComp) override;

	virtual void resolveStaticStaticCollide(CollisionComponent * staticComp) override;
	virtual void resolveDynamicStaticCollide(float time, CollisionComponent * staticComp) override;
	virtual void resolveDynamicDynamicCollide(float time, CollisionComponent * dynamicComp) override;

	virtual void saveDynamicRetCollideInfoToPrevInfo() override;
	virtual void resolveThisStatic(glm::quat & rotateQuat) override;
	virtual void resolveThisDynamic(const glm::vec3 & collidePosition, glm::quat & rotateQuat) override;

private:
	glm::mat4 getCollisionLocalMat();

private:
	AABBOb _aabb;	// 3 same axis
	OBBOb _obb;

	glm::mat4 _localMat;



};