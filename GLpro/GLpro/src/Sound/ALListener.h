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
	void modifyPos(float pos[3]);
	void modifyPos(float x, float y, float z);
	void modifyPos(glm::vec3 vec);

	void modifyVelocity(float vel[3]);
	
	void modifyOrientationAt(float orientationAt[3]);
	void modifyOrientationUp(float orientationUp[3]);

	void modifyOrientationAtWithRotationMatrix(glm::mat4 rMaxtrix);
	void modifyOrientationUpWithRotationMatrix(glm::mat4 rMaxtrix);
	void modifyOrientationWithRotationMatrix(glm::mat4 rMaxtrix);
	
};