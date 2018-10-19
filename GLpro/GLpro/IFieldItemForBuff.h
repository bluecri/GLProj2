#pragma once
#include "IFieldItem.h"

class IBuff;

enum ENUM_FieldItemForBuff_TYPE
{
	ENUM_FieldItemForBuff_UNIQUE_RESET_ALWAYS,				// always reset to new buff
	ENUM_FieldItemForBuff_UNIQUE_RESET_NOTMOREPOWERFUL,		// reset to new buff only more powerful buff
	ENUM_FieldItemForBuff_UNIQUE_NONRESET,					// no reset if has same kind buff
	ENUM_FieldItemForBuff_NONUNIQUE,						// always stack new buff
	ENUM_FieldItemForBuff_NUM
};


class IFieldItemForBuff : public IFieldItem
{
public:
	IFieldItemForBuff(ENUM_FieldItemForBuff_TYPE fieldItemBuffType, GameSession* gSession, RESOURCE::Model * model, RESOURCE::Texture * texture, SHADER::ShaderMain * shadermain);
	virtual ~IFieldItemForBuff();

	ENUM_FieldItemForBuff_TYPE		getFieldItemForBuffType();
	IBuff*			getCloneBuff();
	void			setBuff(IBuff* buff);

	// IFieldItem을(를) 통해 상속됨
	virtual void	doJobWithBeDeleted() override;
	virtual void	logicUpdate(float deltaTime, float acc) override;
	virtual void	collisionFunc(CollisionComponent * collisionComp) override;

private:
	ENUM_FieldItemForBuff_TYPE	_fieldItemBuffType;
	IBuff*						_Ibuff;
};