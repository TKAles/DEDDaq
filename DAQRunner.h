#pragma once
#include <string>
#include <vector>
#include "AVCameraObject.h"
#include "VimbaCPP/Include/VimbaSystem.h"

using namespace AVT::VmbAPI;

class DAQRunner
{
public:
	DAQRunner();
	~DAQRunner();
	std::vector<std::string> AVCameraIDs;
	std::vector<AVCameraObject> assocAVCameras;
	VimbaSystem& cameraSystem = VimbaSystem::GetInstance();
	int startupAVSystem();
	int configureMonochromeSettings();
	int shutdownAVSystem();
	int findAVCameras();
	int makeAVC(std::string _camID);
	int setupCapture(int _ptsToCap, float _freqToCap);
};

