// DEDLoggerConsole.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <string>
#include <iostream>
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
		_experimentCoordinator.setupCapture(CAPTURE_POINTS,
			CAPTURE_FREQUENCY);

	}

	
}
