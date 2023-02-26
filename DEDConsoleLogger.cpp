// Standard Includes
#include <iostream>
#include <string>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>
// Vimba (Allied Vision) Libraries
#include "VimbaCPP/Include/VimbaCPP.h"
// Internal Libraries
#include "AVMonoCamera.h"

/***
 __   ___  __   __        __
|  \ |__  |  \ |  \  /\  /  \
|__/ |___ |__/ |__/ /~~\ \__X

Directed-Energy Deposition Data AcQuisition system.
Thomas Ales | Iowa State Univ
Visual Studio 2022 | C++20 Specification
***/

using namespace AVT::VmbAPI;

VimbaSystem& AVCameraSystem = VimbaSystem::GetInstance();

AVMonoCamera TestCamera;

int searchForCameras(VimbaSystem& cameraSystem, std::vector<std::string>& _outValue)
{
	CameraPtrVector _foundCameras;
	std::vector<std::string> _foundCameraIDs;

	if (VmbErrorSuccess != cameraSystem.GetCameras(_foundCameras))
	{
		std::cout << "Problem finding cameras!" << std::endl;
		return -1;
	}
	for (CameraPtrVector::iterator cIter = _foundCameras.begin();
		cIter != _foundCameras.end(); cIter++)
	{
		std::string _tmpOut;
		if (VmbErrorSuccess != (*cIter)->GetID(_tmpOut))
		{
			std::cout << "Something went wrong asking the camera for it's ID!" << std::endl;
			return -2;
		}
		else {
			_foundCameraIDs.push_back(_tmpOut);
			std::cout << "Found device ID: " << _tmpOut << std::endl;
		}
	}
	_outValue = _foundCameraIDs;
	std::cout << "Passed back vector of size " << _foundCameraIDs.size() << " with found device IDs!" << std::endl;
	return 0;
}

void main()
{
	std::vector<std::string> foundCameras;
	
	// startup the transport layer
	AVCameraSystem.Startup();
	searchForCameras(AVCameraSystem, foundCameras);
	std::cout << "Using ID " << foundCameras[0] << " as TestCamera!" << std::endl;
	AVMonoCamera TestCamera = AVMonoCamera(foundCameras[0], AVCameraSystem);
	// setup worker thread using the streamWorker function
	std::thread TestWorkerThread([&] { TestCamera.streamWorker();  });
	std::cout << "Thread launched. Main thread sleeping." << std::endl;
	std::this_thread::sleep_for(std::chrono::seconds(1));
	std::cout << "Finished Sleeping" << std::endl;
	// Stop streaming
	TestCamera.isStreaming = false;
	TestCamera.streamStopCV.notify_all();
	std::cout << "Attempted to set condition variable, waiting for thread to join" << std::endl;
	TestWorkerThread.join();
	AVCameraSystem.Shutdown();
	return;
}