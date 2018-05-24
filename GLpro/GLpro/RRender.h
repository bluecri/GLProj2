#pragma once

namespace CAMERA
{
	class Camera;
}

class RRender
{
	virtual void update(CAMERA::Camera** cam) = 0;
	virtual void draw(float deltaTime) = 0;
};