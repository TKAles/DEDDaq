// DEDLoggerConsole.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <string>
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include "VimbaCPP/Include/VimbaCPP.h"
#include "VimbaCPP/Include/VimbaSystem.h"
#include "DAQRunner.h"

using namespace AVT::VmbAPI;

int CAPTURE_POINTS = 200;
float CAPTURE_FREQUENCY = 80.0;

int main()
{
	DAQRunner _experimentCoordinator = DAQRunner();
	if (!_experimentCoordinator.startupAVSystem())
	{
		// Configure Monochrome Systems
		_experimentCoordinator.configureMonochromeSettings();
		
		// Camera thread worker setup/teardown/signaling
		// test.
		std::thread _monoOneThread(
			_experimentCoordinator.startStreaming(
				_experimentCoordinator.AVCameraIDs[0])); 
		std::cout << "Launched Worker Thread!" << std::endl;
		std::cout << "Sleeping for 5s." << std::endl;
		std::this_thread::sleep_for(std::chrono::seconds(5));
		// Try to trip the condition variable.
		_experimentCoordinator.stopStreamingMutex
	}

	
}
