#include <chrono>
#include <iostream>
#include <thread>
#include "MonoCamProducer.h"
#include "VimbaCPP/Include/VimbaSystem.h"
#include "VimbaCPP/Include/VimbaCPPCommon.h"
#include "VimbaCPP/Include/VimbaCPP.h"

int timesRan = 0;
bool countCompleted;
CameraPtr sharedCameraPointer;

std::string cameraName;

using namespace std::chrono_literals;
using namespace AVT::VmbAPI;



void MonoCamProducer()
{
	countCompleted= false;
	timesRan = 0;
}

void MonoCamProducer::startupCamera()
{

}
void MonoCamProducer::incrementRun()
{
	int dumbCount = 10;
	while (countCompleted== false)
	{
		if (timesRan > dumbCount)
		{
			countCompleted= true;
			std::cout << "count completed.";
			return;
		}
		else {
			timesRan++;
			std::this_thread::sleep_for(
					std::chrono::milliseconds(250));
			std::cout << "Sleeping...\n";
		}
	}
}