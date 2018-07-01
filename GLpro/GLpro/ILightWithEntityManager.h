#pragma once

#include "stdafx.h"

class GameSession;
class LightWithEntity;

// todo : dirty bit �̿��Ͽ� ������ pos�� ���� (RigidbodyComponent todo �Ϸ��� ����)
class ILightWithEntityManager
{
public:
	ILightWithEntityManager();
	virtual void	initBuffer() = 0;
	virtual void	updateLights() = 0;
	virtual void	updateLightsToBufferData() = 0;
	virtual void	updateBufferToVGA() = 0;

	//virtual std::shared_ptr<LightWithEntity>	addNewLight(GameSession* gSession, glm::vec3& dir, glm::vec3& pos, glm::vec4& color, float lightPower) = 0;
	void	removeLightWithEntity(std::shared_ptr<LightWithEntity> dLight);

	GLuint	getUBO();

protected:
	GLuint	_ubo;
	std::vector<std::shared_ptr<LightWithEntity>>	_lightSharedVec;
};