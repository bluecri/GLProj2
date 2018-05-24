#ifndef  __CAMERA_MANAGER_H__
#define __CAMERA_MANAGER_H__

#include <map>
#include <string>

namespace CAMERA
{
	class Camera;
}

namespace CAMERA
{
	class CameraManager
	{
	public:
		CameraManager();
		~CameraManager();

		bool AddCamera(Camera* cam);
		Camera** GetMainCamera() { return &_mainCamera; }
		void updateAllRecentMatrix();


		bool ChangeMainCamera(const std::string& cameraName);

	private:
		Camera* _mainCamera;
		std::map<std::string, Camera*> _cameraMap;
	};
}

extern CAMERA::CameraManager* GCameraManager;

#endif 