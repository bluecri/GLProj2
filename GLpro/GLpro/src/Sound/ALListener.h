#pragma once
#include <iostream>

#include <glm/glm.hpp>

class ALListener {
private:
	float listenerOrientationAtUp[6];
public:
	float listenerPos[3];
	float listenerVelocity[3];
	float *listenerOrientationAt;
	float *listenerOrientationUp;

	ALListener();
	void updateWithWorldMat(const glm::mat4 & worldMat);
	void modifyPos(const float pos[3]);
	void modifyPos(float x, float y, float z);
	void modifyPos(const glm::vec3& vec);

	void modifyVelocity(const float vel[3]);
	
	void modifyOrientationAt(const float orientationAt[3]);
	void modifyOrientationUp(const float orientationUp[3]);

	void modifyOrientationAtWithRotationMatrix(const glm::mat4& rMaxtrix);
	void modifyOrientationUpWithRotationMatrix(const glm::mat4& rMaxtrix);
	void modifyOrientationWithRotationMatrix(const glm::mat4& rMaxtrix);
	
};