#pragma once

#include "IGameObject.h"

class IMissile : public IGameObject
{
public:
	IMissile(int type, GameSession* gSession, Entity* fromEntity, RESOURCE::Model* model, RESOURCE::Texture * texture, SHADER::ShaderMain * shadermain);
	virtual ~IMissile();

public:
	Entity* _fromEntity;		// who shot
};