#include "./CameraManager.h"
#include "./Camera.h"
#include "./CameraBuilder.h"

CAMERA::CameraManager* GCameraManager = nullptr;

inline CAMERA::CameraManager::CameraManager()
{
	CameraBuilder camBuild(std::string("mainCam"), 0);
	_mainCamera = camBuild.constructPtr();
	AddCamera(_mainCamera);
}

inline CAMERA::CameraManager::~CameraManager()
{
	for (auto elem : _cameraMap)
	{
		delete elem.second;
	}
}

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