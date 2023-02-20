#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <thread>
#include <condition_variable>
#include "AVCameraObject.h"
#include "VimbaCPP/Include/VimbaSystem.h"
#include "AVFrameObserver.h"

using namespace AVT::VmbAPI;

class DAQRunner
{
public:
	DAQRunner();
	~DAQRunner();
	std::vector<std::string> AVCameraIDs;
	VimbaSystem& cameraSystem = VimbaSystem::GetInstance();

	int startupAVSystem();
	int configureMonochromeSettings();
	int setupCapture(int _ptsToCap, float _freqToCap);
	int startStreaming(std::string _camID);
	int shutdownAVSystem();
	
private:
	std::vector<Frame> framebuf1;
	std::vector<Frame> framebuf2;
};

