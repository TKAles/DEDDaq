#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <thread>
#include <condition_variable>
#include <mutex>
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
	std::mutex stopStreamingMutex;
	std::condition_variable stopStreamingCV;

private:
	bool _killStream;
	VmbInt64_t _payloadSize;
	FramePtrVector _frameBuffer1;
};

