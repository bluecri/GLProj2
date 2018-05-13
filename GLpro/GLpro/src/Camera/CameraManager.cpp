#include "./CameraManager.h"
#include "./Camera.h"

CAMERA::CameraManager* GCameraManager = nullptr;

bool CAMERA::CameraManager::AddCamera(Camera * cam)
{
	auto it = _cameraMap.find(cam->getNameRef());
	if (it == _cameraMap.end())
	{
		_cameraMap.insert(std::make_pair(cam->getNameRef(), cam));
		return true;
	}
	else
	{
		printf_s("[LOG] : CameraManager AddCamera already has same name camera\n");
		return false;
	}
}

bool CAMERA::CameraManager::ChangeMainCamera(const std::string & cameraName)
{
	auto it = _cameraMap.find(cameraName);
	if (it != _cameraMap.end())
	{
		_mainCamera = it->second;
		return true;
	}
	else
	{
		printf_s("[LOG] : CameraManager ChangeMainCamera no cameraName\n");
		return false;
	}
}