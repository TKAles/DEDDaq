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
#include "nisyscfg.h"
#include "NIDAQmx.h"
// Macros
#define DAQmxErrChk(functionCall) if( DAQmxFailed(error=(functionCall)) ) goto Error; else
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
	if (CAMERA_TEST) {
		std::vector<std::string> foundCameras;
		// startup the transport layer
		AVCameraSystem.Startup();
		searchForCameras(AVCameraSystem, foundCameras);
		std::cout << "Using ID " << foundCameras[0] << " as TestCamera!" << std::endl;
		AVMonoCamera TestCamera = AVMonoCamera(foundCameras[0], AVCameraSystem);
		// setup worker thread using the streamWorker function
		std::thread TestWorkerThread([&] { TestCamera.streamWorker();  });
		// setup thread to check the size of the queue in the testcamera object
		// this is to verify that the frames are being properly exported from
		// the avframeobserver object
		bool checkVectors = true;
		std::thread VectorCheckerThread([&] {
			while (checkVectors == true)
			{
				std::cout << "TestCamera.capturedFrameVector size is " <<
					TestCamera.ImageQueue.size() << " elements." << std::endl;
				std::this_thread::sleep_for(750ms);
			}
		});
		std::cout << "Thread launched. Main thread sleeping." << std::endl;
		std::this_thread::sleep_for(10s);
		std::cout << "Finished Sleeping" << std::endl;
		// Stop streaming
		std::cout << "There are " << TestCamera.ImageQueue.size() <<
			" frames in the queue that were exported from the Camera to main()" << std::endl;
		TestCamera.isStreaming = false;
		checkVectors = false;
		TestCamera.streamStopCV.notify_all();
		std::cout << "Attempted to set condition variable, waiting for thread to join" << std::endl;
		TestWorkerThread.join();
		VectorCheckerThread.join();
		AVCameraSystem.Shutdown();
		return;
	}
	
	if (DAQ_TEST)
	{
		
		// prototyping for DAQ connection
		// Get handle for DAQ. 0 = successful
		TaskHandle _daqitydaq;
		if(!DAQmxCreateTask("ClockSignal", &_daqitydaq))
		{
		std::cout << "Created test task ClockSignal" << std::endl;
		// Create a test clock signal
		char _outPort[] = "Dev1/ctr0";
		int32 _pulseUnits = DAQmx_Val_Hz;
		int32 _idlState = DAQmx_Val_High;
		float64 _initDelay = 0.0;
		float64 _freq = 80.0;
		float64 _duty = 0.5;
		// Returns zero if successful.
		// Positive return = Warning. Negative Return = Error.
		// TODO: Add DAQmx error codes!

		auto retval = DAQmxCreateCOPulseChanFreq(_daqitydaq, 
			_outPort,	 /* physical output port */
			"",			 /* friendly name */
			_pulseUnits, /* always Hertz */
			_idlState,   /* idle state of pin */
			_initDelay,  /* initial delay in seconds */
			_freq,		 /* frequency in hertz */
			_duty);		 /* duty cycle as fraction of 1.0 */
		if (retval == 0)
		{
			std::cout << "Port has been configured!" << std::endl;
			// Setup the internal timing (required)
			DAQmxCfgImplicitTiming(_daqitydaq, DAQmx_Val_ContSamps,
				1000);
			if (!DAQmxStartTask(_daqitydaq))
			{
				std::string bs;
				std::cout << "Press enter to stop";
				std::cin >> bs;
				DAQmxStopTask(_daqitydaq);
			}
			else {
				std::cout << "Something went wrong!" << std::endl;
			}

		}
		else if (retval > 0)
		{

			std::cout << "A warning was generated during trigger signal configuration!" << std::endl;
			std::cout << "Error Code was " << retval << "." << std::endl;
		}
		else if (retval < 0)
		{
			std::cout << "An error occurred during trigger signal configuration!" << std::endl;
			std::cout << "Error Code was " << retval << "." << std::endl;
			int _toBuffer = DAQmxGetErrorString(retval, NULL, 0);
			char* _errMsg = new char[_toBuffer];
			auto retcode = DAQmxGetErrorString(retval, _errMsg, _toBuffer);
			if (retcode == 0)
			{
				std::cout << "Error message is " << std::endl <<
					_errMsg << "." << std::endl;
			}

			
		}
		else {
			std::cout << "Something went wrong trying to create the trigger signal!" << std::endl;
		}
		}
		else {
			std::cout << "Something went wrong creating the ClockSignal task!" << std::endl;
			return;
		}

		
	}
}