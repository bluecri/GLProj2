#pragma once

#include "IGameObject.h"

class IMissile : public IGameObject
{
public:
	IMissile(ENUM_ENTITY_TYPE type, GameSession* gSession, Entity* fromEntity, RESOURCE::Model* model, RESOURCE::Texture * texture, SHADER::ShaderMain * shadermain);
	virtual ~IMissile();

public:
	Entity* _fromEntity;		// who shot
};