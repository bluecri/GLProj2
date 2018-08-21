#pragma once

#include "stdafx.h"

enum LINEOB_TYPE_ENUM {
	LINEOB_TYPE_INFINITE_ONE_DIRTECTION = 0,
	LINEOB_TYPE_INFINITE_TWO_DIRECTION,
	LINEOB_TYPE_LEN
};

class LineOb
{
public:
	LineOb(LINEOB_TYPE_ENUM type);
	void		setType(LINEOB_TYPE_ENUM type);
	void		setStartEndPos(glm::vec3& startPos, glm::vec3& endPos);
	void		setStartUnitLen(glm::vec3& startPos, glm::vec3& unitVec, float len);

	void		setStartPos(glm::vec3& startPos);
	void		setNormalizedUnitVec(glm::vec3& normalizedUnitVec);
	void		setLineLen(float& len);

	void		setNotNormalizedUnitVec(glm::vec3& notNormalizedUnitVec, bool isContainLen = false);

	glm::vec3	getStartPos();
	glm::vec3&	getStartPosRef();
	glm::vec3	getEndPos();
	glm::vec3&	getUnitVecRef();
	float&		getLenRef();
	LINEOB_TYPE_ENUM getType();

private:
	LINEOB_TYPE_ENUM _lineType;
	glm::vec3	_startPos;
	glm::vec3	_unitVec;
	float		_len;
};
