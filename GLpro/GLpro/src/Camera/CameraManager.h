#ifndef  __CAMERA_MANAGER_H__
#define __CAMERA_MANAGER_H__

#include "./Camera.h"

namespace CAMERA
{
	class CameraManager
	{
	public:
		CameraManager()
		{
			_mainCamera = nullptr;
		}
		~CameraManager() 
		{
			for (auto elem : _cameraMap)
			{
				delete elem.second;
			}
		}

		bool AddCamera(Camera* cam);
		Camera* GetMainCamera() { return _mainCamera; }

		bool ChangeMainCamera(const std::string& cameraName);

	private:
		Camera* _mainCamera;
		std::map<std::string, Camera*> _cameraMap;
	};
}

#endif // ! __CAMERA_MANAGER_H__

extern CAMERA::CameraManager* GCameraManager;