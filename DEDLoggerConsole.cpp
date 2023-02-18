// DEDLoggerConsole.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <string>
#include <iostream>
#include "VimbaCPP/Include/VimbaCPP.h"
#include "VimbaCPP/Include/VimbaSystem.h"
using namespace AVT::VmbAPI;


int main()
{
	std::string _camName;
	CameraPtrVector _camPtrVec;
	VimbaSystem& _monoSystem = VimbaSystem::GetInstance();
	
	if (VmbErrorSuccess == _monoSystem.Startup())
	{
		// Vimba initialized successfully
		// Try to get camera names
		if (VmbErrorSuccess == _monoSystem.GetCameras(_camPtrVec))
		{
			// Found cameras!
			// Create an iterator to grab all the names
			for (CameraPtrVector::iterator iter = _camPtrVec.begin();
				_camPtrVec.end() != iter; iter++)
			{
				if (VmbErrorSuccess == (*iter)->GetName(_camName))
				{
					std::string _camid;
					(*iter)->GetID(_camid);
					std::cout << "Found Device Type: " << _camName <<
						std::endl << "Device ID: " << _camid << 
						std::endl;
				}
				else {
					// Error on accessing camera name
					std::cout << "Problem accessing camera." << std::endl;
				}
			}
		}
		else {
			// Error on trying to get the list of cameras connected
			// to the system
			std::cout << "Could not get camera pointers!" << std::endl;
		}
	}
		/*
	*/
	else {
		// Error on trying to initialize the Vimba driver.
		std::cout << "Could not initialize the Vimba driver" << std::endl;
	}
	
	_monoSystem.Shutdown();
}
