// DEDLoggerConsole.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <string>
#include <iostream>
#include "VimbaCPP/Include/VimbaCPP.h"
#include "VimbaCPP/Include/VimbaSystem.h"
#include "DAQRunner.h"

using namespace AVT::VmbAPI;


int main()
{
	DAQRunner _experimentCoordinator = DAQRunner();
	if (!_experimentCoordinator.startupAVSystem())
	{
		// Configure Monochrome Systems
		_experimentCoordinator.configureMonochromeSettings();
	}

	
}
