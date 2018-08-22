#include "FrustumOb.h"

FrustumOb::FrustumOb()
{
	_frustum = std::vector<glm::vec4>(6, glm::vec4());
}

void FrustumOb::updateFrustumOb(glm::mat4 & persMat, glm::mat4 & viewMat)
{
	glm::mat4 _tempClip = persMat * viewMat;

	_tempClip = glm::transpose(_tempClip);

	/*
	_frustum[0] = _tempClip[3] + _tempClip[0];		// left
	_frustum[1] = _tempClip[3] - _tempClip[0];		// right
	_frustum[2] = _tempClip[3] + _tempClip[1];		// bottom
	_frustum[3] = _tempClip[3] - _tempClip[1];		// top
	_frustum[4] = _tempClip[3] + _tempClip[2];		// near
	_frustum[5] = _tempClip[3] - _tempClip[2];		// far
	*/

	// frustum normal : outside from inside
	_frustum[0] = -_tempClip[3] - _tempClip[0];		// left
	_frustum[1] = -_tempClip[3] + _tempClip[0];		// right
	_frustum[2] = -_tempClip[3] - _tempClip[1];		// bottom
	_frustum[3] = -_tempClip[3] + _tempClip[1];		// top
	_frustum[4] = -_tempClip[3] - _tempClip[2];		// near
	_frustum[5] = -_tempClip[3] + _tempClip[2];		// far

	//for (int i = 0; i < 6; i++)
		//_frustum[i] = glm::normalize(_frustum[i]);
}

const std::vector<glm::vec4>& FrustumOb::getFrustumConstRef() const
{
	return _frustum;
}
