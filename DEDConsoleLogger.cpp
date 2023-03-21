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
#include "DAQ.h"
// National Instruments DAQmx Libraries
#include "nisyscfg.h"
#include "NIDAQmx.h"

/***
 __   ___  __   __        __
|  \ |__  |  \ |  \  /\  /  \
|__/ |___ |__/ |__/ /~~\ \__X

Directed-Energy Deposition Data AcQuisition system.
Thomas Ales | Iowa State Univ
Visual Studio 2022 | C++20 Specification
***/

using namespace AVT::VmbAPI;
using namespace std::chrono_literals;
VimbaSystem& AVCameraSystem = VimbaSystem::GetInstance();

AVMonoCamera TestCamera;
bool CAMERA_TEST = true;
bool DAQ_TEST = false;
std::string RUN_PREFIX;

namespace misc
{
	std::string GenerateDTPrefix()
	{
		// Function-specific variables required.
		// Change _tdFormat if formatting requirements are 
		// different.
		const char _tdFormat[] = "%Y%m%d-%H%M%S";
		char _output[20];
		__time64_t _secondsTime;
		tm _timeStruct;

		// Get the current system time and convert it
		// into something useful for the collection run
		// Format is YYYYMMDD-HHMMSS
		_time64(&_secondsTime);
		localtime_s(&_timeStruct, &_secondsTime);
		// Convert and return as std::string
		strftime(_output, sizeof(_output), _tdFormat, &_timeStruct);
		return std::string(_output);
	}

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

};


void main()
{
	if (CAMERA_TEST) {
		std::vector<std::string> foundCameras;
		// startup the transport layer
		AVCameraSystem.Startup();
		misc::searchForCameras(AVCameraSystem, foundCameras);
		RUN_PREFIX = misc::GenerateDTPrefix();
		auto captureFilename = RUN_PREFIX + '-' + foundCameras[0];
		std::cout << "Using ID " << foundCameras[0] << " as TestCamera!" << std::endl;
		AVMonoCamera TestCamera = AVMonoCamera(foundCameras[0], AVCameraSystem, RUN_PREFIX);
		
		// setup worker thread using the streamWorker function
		std::thread TestWorkerThread([&TestCamera] { TestCamera.streamWorker(); });
		std::thread TestConsumerThread([&TestCamera] {TestCamera.ImageConsumer.Consumer(); });
		std::thread TestEncoderThread([&TestCamera, captureFilename] { 
			TestCamera.ImageConsumer.Encoder(captureFilename); });
		// Setup frame consumer object
		std::cout << "Thread launched. Main thread sleeping." << std::endl;
		std::this_thread::sleep_for(10s);
		std::cout << "Finished Sleeping" << std::endl;
		// Stop streaming
		std::cout << "There are " << TestCamera.ImageQueue.size() <<
			" frames in the queue that were exported from the Camera to main()" << std::endl;
		TestCamera.isStreaming = false;
		TestCamera.streamStopCV.notify_all();
		std::cout << "Attempted to set condition variable, waiting for thread to join" << std::endl;
		TestWorkerThread.join();
		AVCameraSystem.Shutdown();
		return;
	}
}
