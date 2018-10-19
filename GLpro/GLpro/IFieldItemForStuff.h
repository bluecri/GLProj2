#pragma once
#include "IFieldItem.h"

enum ENUM_FieldItemForStuff_TYPE
{
	//ENUM_FieldItemForBuff_UNIQUE_RESET_ALWAYS,				// always reset to new buff
	//ENUM_FieldItemForBuff_UNIQUE_RESET_NOTMOREPOWERFUL,		// reset to new buff only more powerful buff
	//ENUM_FieldItemForBuff_UNIQUE_NONRESET,					// no reset if has same kind buff
	//ENUM_FieldItemForBuff_NONUNIQUE,						// always stack new buff
	//ENUM_FieldItemForBuff_NUM
};

class IFieldItemForStuff : public IFieldItem
{

};