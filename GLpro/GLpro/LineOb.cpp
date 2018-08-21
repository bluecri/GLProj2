#include "LineOb.h"

LineOb::LineOb(LINEOB_TYPE_ENUM type)
{
	_lineType = type;
}

void LineOb::setType(LINEOB_TYPE_ENUM type)
{
	_lineType = type;
}

void LineOb::setStartEndPos(glm::vec3 & startPos, glm::vec3 & endPos)
{
	_startPos = startPos;
	_unitVec = glm::normalize(endPos - startPos);
	if (_lineType == LINEOB_TYPE_LEN)
		_len = glm::len(endPos - startPos);
}

void LineOb::setStartUnitLen(glm::vec3 & startPos, glm::vec3 & unitVec, float len)
{
	_startPos = startPos;
	_unitVec = unitVec;
	_len = len;
}

void LineOb::setStartPos(glm::vec3 & startPos)
{
	_startPos = startPos;
}

void LineOb::setNormalizedUnitVec(glm::vec3 & normalizedUnitVec)
{
	_unitVec = normalizedUnitVec;
}

void LineOb::setLineLen(float & len)
{
	_len = len;
}

void LineOb::setNotNormalizedUnitVec(glm::vec3 & notNormalizedUnitVec, bool isContainLen)
{
	_unitVec = glm::normalize(notNormalizedUnitVec);
	if(isContainLen)
		_len = glm::len(notNormalizedUnitVec)
}

glm::vec3 LineOb::getStartPos()
{
	return _startPos;
}

glm::vec3 & LineOb::getStartPosRef()
{
	return _startPos;
}

glm::vec3 LineOb::getEndPos()
{
	return _unitVec * _len + _startPos;
}

glm::vec3 & LineOb::getUnitVecRef()
{
	return _unitVec;
}

float & LineOb::getLenRef()
{
	return _len;
}

LINEOB_TYPE_ENUM LineOb::getType() {
	return _lineType;
}
