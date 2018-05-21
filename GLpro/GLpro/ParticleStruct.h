#pragma once
#include "stdafx.h"

typedef struct _ParticleStruct ParticleStruct;

struct _ParticleStruct
{
	glm::vec3 _pos, _velocity;
	unsigned char _color[4];	// Color(rgba)
	float _size, _angle;
	float _life;				// Remaining life of the particle. if <0 : dead and unused.
	float _cameradistance;		// *Squared* distance to the camera. if dead : -1.0f

	bool operator<(const ParticleStruct& that) const {
		// Sort in reverse order : far particles drawn first.
		// TODO : 수명 종료된 경우 뒤로 보내기
		
		return this->_cameradistance > that._cameradistance;
	}

	void update(float deltaTime, const glm::vec3& cameraPos)
	{
		if (_life >= 0.0f)
		{
			_pos += _velocity * deltaTime;
			_cameradistance = glm::length2(_pos - cameraPos);	// not use sqrt
			_life -= deltaTime;
		}
	}
};