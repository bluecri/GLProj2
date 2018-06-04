#pragma once

#include "IFieldItem.h"

class ItemSpeedBuff : public IFieldItem
{
public:
	ItemSpeedBuff(GameSession * gSession, glm::vec3& genPos);
	//IFieldItemInit()
	void itemInit();


	// IFieldItem��(��) ���� ��ӵ�
	virtual void logicUpdate(float deltaTime, float acc) override;
	virtual void collisionFunc(CollisionComponent * collisionComp) override;
	virtual void doJobWithBeDeleted() override;

};