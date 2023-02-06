#pragma once
#include <mutex>
#include <chrono>
#include <thread>
#include "VimbaCPP/Include/VimbaSystem.h"
#include "VimbaCPP/Include/VimbaCPP.h"

using namespace AVT::VmbAPI;

class MonoCamProducer
{

public:
	int times_ran;
	bool countCompleted;
	void incrementRun();
	void startupCamera();

private:
	CameraPtr sharedCameraPointer;


};