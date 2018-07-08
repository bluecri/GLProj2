#pragma once

namespace CAMERA
{
	class Camera;
}

class RRender
{
	virtual void updateRRender() = 0;
	virtual void drawRRender(float deltaTime) = 0;
	virtual void updateTargetCamera(CAMERA::Camera** cam) = 0;
};